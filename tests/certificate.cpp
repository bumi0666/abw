#include "reference/v013.hpp"
#include <abw/abw.hpp>
#include <iostream>
#include <random>
#include <stdexcept>
using V=std::vector<uint32_t>;
size_t checked=0,witnesses=0,misses=0;
void check(const V& a){
 auto x=baseline::detail::choose(a);auto y=abw::detail::choose(a);
 if(int(x.selected)!=int(y.selected)||x.bits!=y.bits||x.shift!=y.shift)throw std::runtime_error("decision mismatch");
 if(a.size()>=100000){auto p=baseline::detail::probe(a);uint32_t difference=0;for(auto v:a)difference|=v^a[0];
  bool omit=!p.ordered&&(p.variation&0x80000000u);witnesses+=omit;
  if(omit&&!(difference&0x80000000u))throw std::runtime_error("invalid witness");
  misses+=!p.ordered&&!(p.variation&0x80000000u)&&(difference&0x80000000u);
 }
 auto e=a,b=a,c=a;std::sort(e.begin(),e.end());baseline::sort(b);abw::sort(c);if(b!=e||c!=e)throw std::runtime_error("sort mismatch");
 if(x.selected==baseline::detail::strategy::active8||x.selected==baseline::detail::strategy::active12){b=a;c=a;bool bx=baseline::detail::partition_and_sort(b,x.bits,x.shift),cy=abw::detail::partition_and_sort(c,y.bits,y.shift);if(bx!=cy||b!=e||c!=e)throw std::runtime_error("direct/mixed mismatch");}
 ++checked;
}
int main(){try{
 check({});check({0});check({UINT32_MAX});check({UINT32_MAX,0,0x80000000u,0});std::mt19937 random(508321);
 for(size_t n:{99999u,100000u,100001u,100003u,1000000u}){
  V a(n);for(auto&v:a)v=random();check(a);std::sort(a.begin(),a.end());check(a);std::reverse(a.begin(),a.end());check(a);
  for(uint32_t base:{0u,0x80000000u,0xffffff00u}){for(auto&v:a)v=base|(random()%8);check(a);std::fill(a.begin(),a.end(),base);check(a);}
 }
 for(unsigned width=1;width<=32;++width){V a(100003);uint32_t mask=width==32?UINT32_MAX:(uint32_t(1)<<width)-1;for(auto&v:a)v=random()&mask;check(a);}
 // A rare high bit absent from all probe reads still requires the original scan.
 for(size_t n:{100003u,1000000u}){std::vector<bool>seen(n);seen[0]=true;baseline::detail::rng pos(0x12577abc);for(size_t j=0;j<256;++j){size_t lo=j*(n-1)/256,hi=(j+1)*(n-1)/256,i=lo+pos.next()%std::max<size_t>(1,hi-lo);seen[i]=seen[i+1]=true;}
  V a(n);for(auto&v:a)v=random()%128;size_t late=n-1;while(seen[late])--late;a[late]=0x80000000u;check(a);
  if(baseline::detail::probe(a).variation&0x80000000u)throw std::runtime_error("counterexample sampled");
  a[late]=0;size_t found=1;while(!seen[found])++found;a[found]=UINT32_MAX;check(a);
 }
 if(!witnesses||!misses)throw std::runtime_error("witness/miss coverage");
 std::cout<<"PASS cases="<<checked<<" scan_omitted="<<witnesses<<" unsampled_highbit="<<misses<<"\n";
}catch(const std::exception&e){std::cerr<<e.what();return 1;}}
