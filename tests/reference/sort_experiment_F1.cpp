#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <boost/sort/spreadsort/spreadsort.hpp>
#include <boost/sort/sample_sort/sample_sort.hpp>
using U=uint32_t;using V=std::vector<U>;using Clock=std::chrono::steady_clock;
struct RNG{U x;explicit RNG(U s):x(s){} U next(){x^=x<<13;x^=x>>17;x^=x<<5;return x;}};
struct Out{std::string selected;double total_ms=0,analysis_ms=0;};
double elapsed(Clock::time_point t){return std::chrono::duration<double,std::milli>(Clock::now()-t).count();}
bool ordered_probe(const V&a){size_t m=std::min<size_t>(256,a.size()-1);RNG r(0x12577abc);size_t up=0,down=0;for(size_t j=0;j<m;j++){size_t lo=j*(a.size()-1)/m,hi=(j+1)*(a.size()-1)/m,i=lo+r.next()%std::max<size_t>(1,hi-lo);up+=a[i]<a[i+1];down+=a[i]>a[i+1];}return double(up)/m<=.1||double(down)/m<=.1;}
void bits_bucket(V&a,unsigned bits,unsigned shift){size_t B=size_t(1)<<bits;U mask=U(B-1);std::vector<size_t>counts(B),starts(B+1);for(U v:a)counts[(v>>shift)&mask]++;for(size_t b=0;b<B;b++)starts[b+1]=starts[b]+counts[b];auto cursor=starts;V tmp(a.size());for(U v:a)tmp[cursor[(v>>shift)&mask]++]=v;for(size_t b=0;b<B;b++)if(counts[b]>1)std::sort(tmp.begin()+starts[b],tmp.begin()+starts[b+1]);std::copy(tmp.begin(),tmp.end(),a.begin());}
Out Ecpp(V&a){Out o;o.selected="native";auto t=Clock::now();if(a.size()<100000||ordered_probe(a)){o.analysis_ms=elapsed(t);std::sort(a.begin(),a.end());return o;}
 size_t m=std::min<size_t>(a.size()-1,a.size()<1000000?512:2048);V values(m);std::unordered_map<U,size_t>freq;RNG r(0x55ab76c1);size_t mode=0;
 for(size_t j=0;j<m;j++){size_t lo=j*(a.size()-1)/m,hi=(j+1)*(a.size()-1)/m;U v=a[lo+r.next()%std::max<size_t>(1,hi-lo)];values[j]=v;mode=std::max(mode,++freq[v]);}
 bool dup=1-double(freq.size())/m>=.5||double(mode)/m>=.1;unsigned bits=dup?8:12;U diff=0;for(size_t i=1;i<a.size();i++)diff|=a[i]^a[0];if(!diff){o.selected="skip";o.analysis_ms=elapsed(t);return o;}
 unsigned width=32-__builtin_clz(diff),shift=width>bits?width-bits:0,mask=(1u<<bits)-1;std::vector<size_t>counts(1u<<bits);for(U v:values)counts[(v>>shift)&mask]++;double H=0;for(auto c:counts)if(c){double p=double(c)/m;H-=p*std::log2(p);}o.analysis_ms=elapsed(t);
 if(H>=(dup?1:4)){o.selected=dup?"active8":"active12";bits_bucket(a,bits,shift);}else std::sort(a.begin(),a.end());return o;
}
// Reference implementation of the 2002 presubmission Fig.1 / 2003 wrapper.
// Sampling WITH replacement as in the paper's practical implementation, upper_bound for ties.
// Splitter formula is our frozen extrapolation matching all 7 published default sizes.
void randomized_wrapper(V&a,U seed){size_t n=a.size();if(n<2)return;double L=std::log2(double(n));size_t splits=std::max<size_t>(1,size_t(2.2*n/std::pow(L,2.5))),k=splits+1,s=std::max<size_t>(1,size_t(L));k=std::min(k,n);size_t sm=k*s-1;V sample(sm),splitters(k-1);RNG r(seed);for(auto &v:sample)v=a[r.next()%n];std::sort(sample.begin(),sample.end());for(size_t i=1;i<k;i++)splitters[i-1]=sample[i*s-1];
 V ids(n);std::vector<size_t>counts(k),ends(k+1);for(size_t i=0;i<n;i++){size_t b=std::upper_bound(splitters.begin(),splitters.end(),a[i])-splitters.begin();ids[i]=b;counts[b]++;}for(size_t b=0;b<k;b++)ends[b+1]=ends[b]+counts[b];auto cursor=ends;V out(n);for(size_t i=0;i<n;i++)out[cursor[ids[i]]++]=a[i];for(size_t b=0;b<k;b++)std::sort(out.begin()+ends[b],out.begin()+ends[b+1]);std::copy(out.begin(),out.end(),a.begin());
}
// Explicitly labelled reference recursive comparison samplesort; no claim of optimized S4o/IPS4o.
void sample_ref_range(V&a,size_t lo,size_t hi,RNG&r,unsigned depth){size_t n=hi-lo;if(n<=4096||depth>=4){std::sort(a.begin()+lo,a.begin()+hi);return;}size_t B=std::min<size_t>(256,n/256),s=16;V sample(B*s-1),splitters;for(auto&v:sample)v=a[lo+r.next()%n];std::sort(sample.begin(),sample.end());for(size_t j=1;j<B;j++)splitters.push_back(sample[j*s-1]);splitters.erase(std::unique(splitters.begin(),splitters.end()),splitters.end());B=splitters.size()+1;V ids(n);std::vector<size_t>counts(B),starts(B+1);for(size_t i=0;i<n;i++){size_t b=std::upper_bound(splitters.begin(),splitters.end(),a[lo+i])-splitters.begin();ids[i]=b;counts[b]++;}if(*std::max_element(counts.begin(),counts.end())==n){std::sort(a.begin()+lo,a.begin()+hi);return;}for(size_t b=0;b<B;b++)starts[b+1]=starts[b]+counts[b];auto cursor=starts;V out(n);for(size_t i=0;i<n;i++)out[cursor[ids[i]]++]=a[lo+i];std::copy(out.begin(),out.end(),a.begin()+lo);for(size_t b=0;b<B;b++)if(counts[b]>1)sample_ref_range(a,lo+starts[b],lo+starts[b+1],r,depth+1);}
Out execute(V&a,const std::string&m,U seed){auto t=Clock::now();Out o;o.selected=m;if(m=="std")std::sort(a.begin(),a.end());else if(m=="spread")boost::sort::spreadsort::integer_sort(a.begin(),a.end());else if(m=="boost_sample_1t")boost::sort::sample_sort(a.begin(),a.end(),1u);else if(m=="boost_sample_4t")boost::sort::sample_sort(a.begin(),a.end(),4u);else if(m=="sample_ref_1t"){RNG r(seed);sample_ref_range(a,0,a.size(),r,0);}else if(m=="wrapper")randomized_wrapper(a,seed);else if(m=="E_cpp")o=Ecpp(a);else throw std::runtime_error("unknown method");o.total_ms=elapsed(t);return o;}
V load(const std::string&p){std::ifstream f(p,std::ios::binary|std::ios::ate);if(!f)throw std::runtime_error("cannot open input");auto bytes=f.tellg();f.seekg(0);V a(size_t(bytes)/4);f.read(reinterpret_cast<char*>(a.data()),bytes);if(!f)throw std::runtime_error("read failed");return a;}
int main(){try{std::vector<std::string>methods={"std","spread","boost_sample_1t","boost_sample_4t","sample_ref_1t","wrapper","E_cpp"};std::string path,expected_path;U seed;size_t reps;while(std::cin>>path>>expected_path>>seed>>reps){V src=load(path),expected=load(expected_path);if(src.size()!=expected.size())throw std::runtime_error("size mismatch");V warm(src.begin(),src.begin()+std::min<size_t>(100000,src.size()));for(const auto&m:methods){auto w=warm;execute(w,m,seed);}
 std::cout<<"{\"runs\":[";bool first=true;for(size_t rep=0;rep<reps;rep++){auto order=methods;std::mt19937 r(seed+U(rep*7919));std::shuffle(order.begin(),order.end(),r);for(const auto&m:order){auto a=src;Out o=execute(a,m,seed^U((rep+1)*0x9e3779b9));if(a!=expected)throw std::runtime_error("wrong result "+m);if(!first)std::cout<<",";first=false;std::cout<<"{\"method\":\""<<m<<"\",\"rep\":"<<rep<<",\"ms\":"<<o.total_ms<<",\"analysis_ms\":"<<o.analysis_ms<<",\"selected\":\""<<o.selected<<"\"}";}}
 std::cout<<"],\"verified\":true}"<<std::endl;
 }}catch(const std::exception&e){std::cerr<<e.what()<<std::endl;return 1;}return 0;}
