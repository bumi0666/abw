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
