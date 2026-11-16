#include "reference/v013.hpp"
#include <abw/abw.hpp>
#include <atomic>
#include <cstdlib>
#include <new>
#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <type_traits>
#include <stdexcept>
std::atomic<bool> fail_allocation{false};
std::size_t failure_minimum=0;
void* operator new(std::size_t n){if(fail_allocation.load()&&n>=failure_minimum){fail_allocation=false;throw std::bad_alloc();}if(void*p=std::malloc(n?n:1))return p;throw std::bad_alloc();}
void operator delete(void*p)noexcept{std::free(p);}void operator delete(void*p,std::size_t)noexcept{std::free(p);}
void* operator new[](std::size_t n){return ::operator new(n);}void operator delete[](void*p)noexcept{std::free(p);}void operator delete[](void*p,std::size_t)noexcept{std::free(p);}
using V=std::vector<uint32_t>;
V load(const char*p){std::ifstream f(p,std::ios::ate|std::ios::binary);if(!f)throw std::runtime_error("load");auto n=f.tellg();if(n<0||n%4)throw std::runtime_error("size");V a(size_t(n)/4);f.seekg(0);f.read(reinterpret_cast<char*>(a.data()),n);if(!f)throw std::runtime_error("read");return a;}
size_t checked=0,mixed=0,direct=0;
abw::workspace persistent;
void check(const V& a){
 auto old=baseline::detail::choose(a);auto now=abw::detail::choose(a);if(int(old.selected)!=int(now.selected)||old.bits!=now.bits||old.shift!=now.shift)throw std::runtime_error("decision");
 auto expected=a,b=a,c=a;std::sort(expected.begin(),expected.end());abw::sort(b);abw::sort(c,persistent);if(b!=expected||c!=expected)throw std::runtime_error("default/workspace result");
 if(old.selected==baseline::detail::strategy::active8||old.selected==baseline::detail::strategy::active12){b=a;c=a;auto bytes=persistent.retained_bytes();bool x=baseline::detail::partition_and_sort(b,old.bits,old.shift),y=abw::detail::partition_and_sort_workspace(c,now.bits,now.shift,persistent);if(x!=y||b!=expected||c!=expected)throw std::runtime_error("direct/mixed");if(y&&bytes!=persistent.retained_bytes())throw std::runtime_error("direct path changed capacity");mixed+=!y;direct+=y;}
 ++checked;
}
int main(int argc,char**argv){try{
 static_assert(!std::is_copy_constructible<abw::workspace>::value,"no accidental cache copy");static_assert(std::is_nothrow_move_constructible<abw::workspace>::value,"movable");static_assert(noexcept(persistent.release()),"release noexcept");
 if(persistent.capacity()!=0||persistent.retained_bytes()!=0)throw std::runtime_error("initial capacity");
 check({});check({0});check({UINT32_MAX});std::mt19937 rng(508321);
 for(size_t n:{99999u,100003u,1000000u,100001u,100000u})for(unsigned width:{8u,12u,20u,32u}){V a(n);uint32_t mask=width==32?UINT32_MAX:(uint32_t(1)<<width)-1;for(auto&v:a)v=rng()&mask;check(a);std::fill(a.begin(),a.end(),UINT32_MAX);check(a);}
 for(int i=1;i<argc;++i)check(load(argv[i]));
 auto retained=persistent.retained_bytes();abw::workspace moved(std::move(persistent));if(moved.retained_bytes()!=retained)throw std::runtime_error("move storage");persistent.release();persistent=std::move(moved);moved.release();
 // Establish a small buffer, then fail the next large temporary growth.
 persistent.release();V small(100003);for(auto&v:small)v=rng();abw::sort(small,persistent);auto old_capacity=persistent.capacity();
 V input(1000000);for(auto&v:input)v=rng();auto before=input;failure_minimum=input.size()*4;fail_allocation=true;bool threw=false;
 try{abw::sort(input,persistent);}catch(const std::bad_alloc&){threw=true;}fail_allocation=false;
 if(!threw||input!=before||persistent.capacity()!=old_capacity)throw std::runtime_error("allocation-failure state");
 auto expected=before;std::sort(expected.begin(),expected.end());abw::sort(input,persistent);if(input!=expected)throw std::runtime_error("reuse after failure");
 persistent.release();persistent.release();if(persistent.capacity()!=0||persistent.retained_bytes()!=0)throw std::runtime_error("release");
 // Separate caller-owned workspaces may run concurrently.
 std::atomic<bool> threads_ok{true};std::vector<std::thread> workers;
 for(unsigned t=0;t<3;++t)workers.emplace_back([t,&threads_ok]{try{abw::workspace ws;std::mt19937 r(72983+t);V x(100003);for(auto&v:x)v=r();auto e=x;std::sort(e.begin(),e.end());abw::sort(x,ws);if(x!=e)threads_ok=false;}catch(...){threads_ok=false;}});
 for(auto&t:workers)t.join();if(!threads_ok)throw std::runtime_error("independent workspace threads");
 if(!mixed||!direct)throw std::runtime_error("coverage");
 std::cout<<"PASS cases="<<checked<<" mixed="<<mixed<<" direct="<<direct<<" grow_shrink=yes move_release=yes injected_bad_alloc=yes separate_threads=3\n";
}catch(const std::exception&e){std::cerr<<e.what();return 1;}}
