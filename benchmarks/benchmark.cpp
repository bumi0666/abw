#include "memory_profile.hpp"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"
#define main f1_unused_main
#include "sort_experiment_F1.cpp"
#undef main
#pragma clang diagnostic pop
#include <iomanip>
#include <boost/sort/pdqsort/pdqsort.hpp>
#include <boost/sort/spinsort/spinsort.hpp>
struct Decision{std::string selected="native";unsigned bits=0,shift=0;double analysis_ms=0;};
Decision pickE(const V&a){Decision d;auto t=Clock::now();if(a.size()<100000||ordered_probe(a)){d.analysis_ms=elapsed(t);return d;}size_t m=std::min<size_t>(a.size()-1,a.size()<1000000?512:2048);V values(m);std::unordered_map<U,size_t>freq;RNG r(0x55ab76c1);size_t mode=0;for(size_t j=0;j<m;j++){size_t lo=j*(a.size()-1)/m,hi=(j+1)*(a.size()-1)/m;U v=a[lo+r.next()%std::max<size_t>(1,hi-lo)];values[j]=v;mode=std::max(mode,++freq[v]);}bool dup=1-double(freq.size())/m>=.5||double(mode)/m>=.1;d.bits=dup?8:12;U diff=0;for(size_t i=1;i<a.size();i++)diff|=a[i]^a[0];if(!diff){d.selected="skip";d.analysis_ms=elapsed(t);return d;}unsigned width=32-__builtin_clz(diff);d.shift=width>d.bits?width-d.bits:0;U mask=(1u<<d.bits)-1;std::vector<size_t>counts(1u<<d.bits);for(U v:values)counts[(v>>d.shift)&mask]++;double H=0;for(auto c:counts)if(c){double p=double(c)/m;H-=p*std::log2(p);}if(H>=(dup?1:4))d.selected=dup?"active8":"active12";d.analysis_ms=elapsed(t);return d;}
void bucketPdq(V&a,unsigned bits,unsigned shift){size_t B=size_t(1)<<bits;U mask=U(B-1);std::vector<size_t>counts(B),starts(B+1);for(U v:a)counts[(v>>shift)&mask]++;for(size_t b=0;b<B;b++)starts[b+1]=starts[b]+counts[b];auto cursor=starts;V tmp(a.size());for(U v:a)tmp[cursor[(v>>shift)&mask]++]=v;for(size_t b=0;b<B;b++)if(counts[b]>1)boost::sort::pdqsort(tmp.begin()+starts[b],tmp.begin()+starts[b+1]);std::copy(tmp.begin(),tmp.end(),a.begin());}
Out executeM(V&a,const std::string&m,U seed){Out o;o.selected=m;auto t=Clock::now();if(m=="std")std::sort(a.begin(),a.end());else if(m=="pdq")boost::sort::pdqsort(a.begin(),a.end());else if(m=="spread")boost::sort::spreadsort::integer_sort(a.begin(),a.end());else if(m=="spinsort")boost::sort::spinsort(a.begin(),a.end());else if(m=="sample4")boost::sort::sample_sort(a.begin(),a.end(),4u);else if(m=="E_cpp")o=Ecpp(a);else if(m=="E_pdq"){auto d=pickE(a);o.selected=d.selected;o.analysis_ms=d.analysis_ms;if(d.selected=="native")boost::sort::pdqsort(a.begin(),a.end());else if(d.selected!="skip")bucketPdq(a,d.bits,d.shift);}
else{unsigned bits=m=="active8_cpp"?8:12,shift=32-bits;if(m!="fixed12_cpp"){auto ts=Clock::now();U diff=0;for(size_t i=1;i<a.size();i++)diff|=a[i]^a[0];if(!diff){o.selected="skip";o.analysis_ms=elapsed(ts);o.total_ms=elapsed(t);return o;}unsigned width=32-__builtin_clz(diff);shift=width>bits?width-bits:0;o.analysis_ms=elapsed(ts);}if(m=="active12_pdq")bucketPdq(a,bits,shift);else bits_bucket(a,bits,shift);}o.total_ms=elapsed(t);return o;}
std::vector<std::string>methodsM={"std","pdq","spread","spinsort","sample4","fixed12_cpp","active8_cpp","active12_cpp","E_cpp","E_pdq","active12_pdq"};

#include <memory>
#include <type_traits>
#include <stdexcept>
#include <limits>
struct EmptyWriteS{explicit EmptyWriteS(size_t){}};
struct WriteS{std::vector<unsigned char>seen;size_t writes=0;explicit WriteS(size_t n):seen(n,0){}void put(size_t p,size_t lo,size_t hi){if(p<lo||p>=hi||p>=seen.size())throw std::runtime_error("write outside bucket range");if(seen[p])throw std::runtime_error("duplicate temporary/output write");seen[p]=1;writes++;}void finish(){if(writes!=seen.size()||std::find(seen.begin(),seen.end(),0)!=seen.end())throw std::runtime_error("unwritten temporary/output slot");}};
struct EvidenceS{size_t writes=0;bool all_once=false,cursors_end=false,direct=false;size_t first_mixed=std::numeric_limits<size_t>::max(),n_temp_allocs=0;std::vector<size_t>counts;};
template<bool Zero,bool Debug=false>std::conditional_t<Debug,EvidenceS,void> bucketPointerS(V&a,unsigned bits,unsigned shift){size_t B=size_t(1)<<bits;U mask=U(B-1);std::vector<size_t>counts(B),starts(B+1);for(U v:a)counts[(v>>shift)&mask]++;for(size_t b=0;b<B;b++)starts[b+1]=starts[b]+counts[b];auto cursor=starts;std::unique_ptr<U[]>tmp;if constexpr(Zero)tmp.reset(new U[a.size()]());else tmp.reset(new U[a.size()]);std::conditional_t<Debug,WriteS,EmptyWriteS>check(a.size());
 for(U v:a){size_t b=(v>>shift)&mask,p=cursor[b]++;if constexpr(Debug)check.put(p,starts[b],starts[b+1]);tmp[p]=v;}
 if constexpr(Debug){if(starts[B]!=a.size())throw std::runtime_error("prefix sum !=n");check.finish();for(size_t b=0;b<B;b++)if(cursor[b]!=starts[b+1])throw std::runtime_error("cursor end mismatch");}
 for(size_t b=0;b<B;b++)if(counts[b]>1)boost::sort::pdqsort(tmp.get()+starts[b],tmp.get()+starts[b+1]);std::copy(tmp.get(),tmp.get()+a.size(),a.begin());
 if constexpr(Debug)return EvidenceS{check.writes,true,true,false,std::numeric_limits<size_t>::max(),1,std::move(counts)};
}
void bucketPtrZeroS(V&a,unsigned bits,unsigned shift){bucketPointerS<true>(a,bits,shift);}void bucketUninitS(V&a,unsigned bits,unsigned shift){bucketPointerS<false>(a,bits,shift);}
template<bool Debug=false>std::conditional_t<Debug,EvidenceS,bool> bucketSingleS(V&a,unsigned bits,unsigned shift){size_t B=size_t(1)<<bits;U mask=U(B-1);std::vector<size_t>counts(B),starts(B+1);V representative(B);bool single=true;size_t i=0;std::conditional_t<Debug,WriteS,EmptyWriteS>check(a.size());size_t firstMixed=std::numeric_limits<size_t>::max();
 for(;i<a.size();i++){U v=a[i];size_t b=(v>>shift)&mask;size_t old=counts[b]++;if(!old)representative[b]=v;else if(representative[b]!=v){single=false;if constexpr(Debug)firstMixed=i;i++;break;}}
 for(;i<a.size();i++)counts[(a[i]>>shift)&mask]++;
 if(single){size_t p=0;for(size_t b=0;b<B;b++)if(counts[b]){size_t end=p+counts[b];if constexpr(Debug){for(size_t j=p;j<end;j++){check.put(j,p,end);a[j]=representative[b];}}else std::fill_n(a.begin()+p,counts[b],representative[b]);p=end;}if constexpr(Debug){if(p!=a.size())throw std::runtime_error("direct fill !=n");check.finish();return EvidenceS{check.writes,true,true,true,firstMixed,0,std::move(counts)};}else return true;}
 for(size_t b=0;b<B;b++)starts[b+1]=starts[b]+counts[b];auto cursor=starts;V tmp(a.size());for(U v:a){size_t b=(v>>shift)&mask,p=cursor[b]++;if constexpr(Debug)check.put(p,starts[b],starts[b+1]);tmp[p]=v;}
 if constexpr(Debug){if(starts[B]!=a.size())throw std::runtime_error("B prefix !=n");check.finish();for(size_t b=0;b<B;b++)if(cursor[b]!=starts[b+1])throw std::runtime_error("B cursor end");}
 for(size_t b=0;b<B;b++)if(counts[b]>1)boost::sort::pdqsort(tmp.begin()+starts[b],tmp.begin()+starts[b+1]);std::copy(tmp.begin(),tmp.end(),a.begin());if constexpr(Debug)return EvidenceS{check.writes,true,true,false,firstMixed,1,std::move(counts)};else return false;
}
thread_local bool lastSingleRouteS=false;
Out executePtrZeroS(V&a,const std::string&m,U seed){Out o;o.selected=m;auto t=Clock::now();if(m=="std")std::sort(a.begin(),a.end());else if(m=="pdq")boost::sort::pdqsort(a.begin(),a.end());else if(m=="spread")boost::sort::spreadsort::integer_sort(a.begin(),a.end());else if(m=="spinsort")boost::sort::spinsort(a.begin(),a.end());else if(m=="sample4")boost::sort::sample_sort(a.begin(),a.end(),4u);else if(m=="E_cpp")o=Ecpp(a);else if(m=="E_pdq"){auto d=pickE(a);o.selected=d.selected;o.analysis_ms=d.analysis_ms;if(d.selected=="native")boost::sort::pdqsort(a.begin(),a.end());else if(d.selected!="skip")bucketPtrZeroS(a,d.bits,d.shift);}
else{unsigned bits=m=="active8_cpp"?8:12,shift=32-bits;if(m!="fixed12_cpp"){auto ts=Clock::now();U diff=0;for(size_t i=1;i<a.size();i++)diff|=a[i]^a[0];if(!diff){o.selected="skip";o.analysis_ms=elapsed(ts);o.total_ms=elapsed(t);return o;}unsigned width=32-__builtin_clz(diff);shift=width>bits?width-bits:0;o.analysis_ms=elapsed(ts);}if(m=="active12_pdq")bucketPdq(a,bits,shift);else bits_bucket(a,bits,shift);}o.total_ms=elapsed(t);return o;}
Out executeUninitS(V&a,const std::string&m,U seed){Out o;o.selected=m;auto t=Clock::now();if(m=="std")std::sort(a.begin(),a.end());else if(m=="pdq")boost::sort::pdqsort(a.begin(),a.end());else if(m=="spread")boost::sort::spreadsort::integer_sort(a.begin(),a.end());else if(m=="spinsort")boost::sort::spinsort(a.begin(),a.end());else if(m=="sample4")boost::sort::sample_sort(a.begin(),a.end(),4u);else if(m=="E_cpp")o=Ecpp(a);else if(m=="E_pdq"){auto d=pickE(a);o.selected=d.selected;o.analysis_ms=d.analysis_ms;if(d.selected=="native")boost::sort::pdqsort(a.begin(),a.end());else if(d.selected!="skip")bucketUninitS(a,d.bits,d.shift);}
else{unsigned bits=m=="active8_cpp"?8:12,shift=32-bits;if(m!="fixed12_cpp"){auto ts=Clock::now();U diff=0;for(size_t i=1;i<a.size();i++)diff|=a[i]^a[0];if(!diff){o.selected="skip";o.analysis_ms=elapsed(ts);o.total_ms=elapsed(t);return o;}unsigned width=32-__builtin_clz(diff);shift=width>bits?width-bits:0;o.analysis_ms=elapsed(ts);}if(m=="active12_pdq")bucketPdq(a,bits,shift);else bits_bucket(a,bits,shift);}o.total_ms=elapsed(t);return o;}
Out executeSingleS(V&a,const std::string&m,U seed){bool usedDirect=false;Out o;o.selected=m;auto t=Clock::now();if(m=="std")std::sort(a.begin(),a.end());else if(m=="pdq")boost::sort::pdqsort(a.begin(),a.end());else if(m=="spread")boost::sort::spreadsort::integer_sort(a.begin(),a.end());else if(m=="spinsort")boost::sort::spinsort(a.begin(),a.end());else if(m=="sample4")boost::sort::sample_sort(a.begin(),a.end(),4u);else if(m=="E_cpp")o=Ecpp(a);else if(m=="E_pdq"){auto d=pickE(a);o.selected=d.selected;o.analysis_ms=d.analysis_ms;if(d.selected=="native")boost::sort::pdqsort(a.begin(),a.end());else if(d.selected!="skip")usedDirect=bucketSingleS(a,d.bits,d.shift);}
else{unsigned bits=m=="active8_cpp"?8:12,shift=32-bits;if(m!="fixed12_cpp"){auto ts=Clock::now();U diff=0;for(size_t i=1;i<a.size();i++)diff|=a[i]^a[0];if(!diff){o.selected="skip";o.analysis_ms=elapsed(ts);o.total_ms=elapsed(t);return o;}unsigned width=32-__builtin_clz(diff);shift=width>bits?width-bits:0;o.analysis_ms=elapsed(ts);}if(m=="active12_pdq")bucketPdq(a,bits,shift);else bits_bucket(a,bits,shift);}o.total_ms=elapsed(t);lastSingleRouteS=usedDirect;return o;}
#ifdef ABW_MEMPROFILE
#define _aligned_malloc abw_aligned_malloc
#define _aligned_free abw_aligned_free
#endif
#include "cafs2.hpp"
#ifdef ABW_MEMPROFILE
#undef _aligned_malloc
#undef _aligned_free
#endif
#include <windows.h>
#include <psapi.h>
#include <limits>
#include <sstream>
struct ZR {Out out;bool direct=false;size_t heap_peak=0,heap_total=0,heap_calls=0,heap_retained=0,ws_before=0,ws_peak_before=0,ws_peak_after=0;};
PROCESS_MEMORY_COUNTERS_EX pm(){PROCESS_MEMORY_COUNTERS_EX p{};p.cb=sizeof(p);if(!GetProcessMemoryInfo(GetCurrentProcess(),reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&p),sizeof(p)))throw std::runtime_error("process memory");return p;}
void countReal(V&a){if(a.empty())return;auto mm=std::minmax_element(a.begin(),a.end());U lo=*mm.first,hi=*mm.second;uint64_t range=uint64_t(hi)-lo+1;if(range*8>67108864)throw std::runtime_error("counting cap");std::vector<uint64_t>counts(size_t(range),0);for(U v:a)counts[size_t(v-lo)]++;auto it=a.begin();for(size_t j=0;j<counts.size();j++){std::fill_n(it,counts[j],U(uint64_t(lo)+j));it+=counts[j];}}
ZR runReal(const V&src,const V&expected,const std::string&method,bool profile){
 ZR r;V a;std::vector<int32_t>signed_a;if(method=="CAFS_i32"){for(U x:src)if(x>uint32_t(INT32_MAX))throw std::runtime_error("i32 not representable");signed_a.assign(src.begin(),src.end());}else a=src;
 if(profile){auto p=pm();r.ws_before=p.WorkingSetSize;r.ws_peak_before=p.PeakWorkingSetSize;}
#ifdef ABW_MEMPROFILE
 if(profile)abwmem::begin();
#endif
 lastSingleRouteS=false;
 if(method=="pdq")r.out=executeM(a,"pdq",0);else if(method=="E_pdq")r.out=executeM(a,"E_pdq",0);else if(method=="ABW_B"){r.out=executeSingleS(a,"E_pdq",0);r.direct=lastSingleRouteS;}else if(method=="spread")r.out=executeM(a,"spread",0);
 else {auto t=Clock::now();if(method=="CAFS_u32")cafs2::cafs_sort(a,[](auto&v){boost::sort::pdqsort(v.begin(),v.end());});else if(method=="CAFS_i32")cafs2::cafs_sort(signed_a,[](auto&v){boost::sort::pdqsort(v.begin(),v.end());});else if(method=="counting")countReal(a);else throw std::runtime_error("method");r.out.total_ms=elapsed(t);}
#ifdef ABW_MEMPROFILE
 if(profile){abwmem::end();r.heap_peak=abwmem::peak;r.heap_total=abwmem::total;r.heap_calls=abwmem::calls;r.heap_retained=abwmem::live;}
#endif
 if(profile)r.ws_peak_after=pm().PeakWorkingSetSize;
 if(method=="CAFS_i32"){if(signed_a.size()!=expected.size())throw std::runtime_error("size");for(size_t i=0;i<expected.size();i++)if(uint32_t(signed_a[i])!=expected[i])throw std::runtime_error("signed wrong");}else if(a!=expected)throw std::runtime_error("wrong "+method);
 return r;
}
void pinReal(){DWORD_PTR processMask,systemMask;if(!GetProcessAffinityMask(GetCurrentProcess(),&processMask,&systemMask))throw std::runtime_error("affinity read");DWORD_PTR one=processMask&(~processMask+1);if(!SetProcessAffinityMask(GetCurrentProcess(),one))throw std::runtime_error("affinity set");}
void warmReal(const std::vector<std::string>&methods){for(unsigned kind=0;kind<3;kind++){V v(100000);RNG rr(183+kind);for(size_t i=0;i<v.size();i++)v[i]=kind==0?rr.next()%1024:kind==1?U(i):rr.next()%8;auto expected=v;std::sort(expected.begin(),expected.end());for(const auto&m:methods)runReal(v,expected,m,false);}}
void auditReal(const V&a,const V&expected){auto d=pickE(a);bool active=d.selected=="active8"||d.selected=="active12";size_t occupied=0,pure=0;bool all=false;size_t first=SIZE_MAX;
 if(active){size_t B=size_t(1)<<d.bits;U mask=U(B-1);std::vector<size_t>counts(B);V reps(B);std::vector<bool>mixed(B);for(size_t i=0;i<a.size();i++){size_t b=(a[i]>>d.shift)&mask;if(counts[b]++==0)reps[b]=a[i];else if(reps[b]!=a[i]){mixed[b]=true;if(first==SIZE_MAX)first=i;}}for(size_t b=0;b<B;b++){occupied+=counts[b]>0;pure+=counts[b]>0&&!mixed[b];}all=occupied==pure;size_t previous=0;for(U v:expected){size_t b=(v>>d.shift)&mask;if(b<previous)throw std::runtime_error("bucket order");previous=b;}}
 std::cout<<"{\"selected\":\""<<d.selected<<"\",\"bits\":"<<d.bits<<",\"shift\":"<<d.shift<<",\"active\":"<<(active?"true":"false")<<",\"occupied\":"<<occupied<<",\"pure\":"<<pure<<",\"B_direct_expected\":"<<(active&&all?"true":"false")<<",\"first_mixed\":";if(first==SIZE_MAX)std::cout<<"null";else std::cout<<first;std::cout<<"}"<<std::endl;}
int main(int argc,char**argv){try{std::cout<<std::setprecision(15);pinReal();if(argc<5)throw std::runtime_error("usage: bench|memory|audit input expected methods_csv [seed]");V src=load(argv[2]),expected=load(argv[3]);if(src.size()!=expected.size())throw std::runtime_error("input sizes");if(std::string(argv[1])=="audit"){auditReal(src,expected);return 0;}
 std::vector<std::string>methods;std::istringstream ss(argv[4]);std::string m;while(std::getline(ss,m,','))methods.push_back(m);bool memory=std::string(argv[1])=="memory";
#ifdef ABW_MEMPROFILE
 if(!memory)throw std::runtime_error("profile binary only for memory");
#else
 if(memory)throw std::runtime_error("uninstrumented binary cannot profile");
#endif
 if(!memory)warmReal(methods);U seed=argc>5?U(std::stoul(argv[5])):123;auto order=methods;std::mt19937 rr(seed);std::shuffle(order.begin(),order.end(),rr);size_t repeats=memory?1:methods.size();std::cout<<"{\"n\":"<<src.size()<<",\"verified\":true,\"runs\":[";bool first=true;
 for(size_t rep=0;rep<repeats;rep++){auto cycle=order;std::rotate(cycle.begin(),cycle.begin()+rep%cycle.size(),cycle.end());for(size_t pos=0;pos<cycle.size();pos++){auto z=runReal(src,expected,cycle[pos],memory);if(!first)std::cout<<",";first=false;std::cout<<"{\"method\":\""<<cycle[pos]<<"\",\"rep\":"<<rep<<",\"position\":"<<pos<<",\"ms\":"<<z.out.total_ms<<",\"analysis_ms\":"<<z.out.analysis_ms<<",\"selected\":\""<<z.out.selected<<"\",\"B_direct\":"<<(z.direct?"true":"false")<<",\"heap_peak\":"<<z.heap_peak<<",\"heap_total\":"<<z.heap_total<<",\"heap_calls\":"<<z.heap_calls<<",\"heap_retained\":"<<z.heap_retained<<",\"ws_before\":"<<z.ws_before<<",\"ws_peak_before\":"<<z.ws_peak_before<<",\"ws_peak_after\":"<<z.ws_peak_after<<",\"verified\":true}";}}
 std::cout<<"]}"<<std::endl;
}catch(const std::exception&e){std::cerr<<e.what()<<std::endl;return 1;}return 0;}
