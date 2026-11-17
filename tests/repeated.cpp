#include <abw/abw.hpp>
#include "reference/v013.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>
#include <stdexcept>
using V=std::vector<std::uint32_t>;using Clock=std::chrono::steady_clock;
template<class F>double measure(F fn,const V&input,const V&expected,V&out){std::copy(input.begin(),input.end(),out.begin());auto start=Clock::now();fn(out);double ms=std::chrono::duration<double,std::milli>(Clock::now()-start).count();if(out!=expected)throw std::runtime_error("repeated output");return ms;}
int main(){try{std::cout<<std::setprecision(12);
 for(int form=0;form<3;++form){const size_t n=1000000;V input(n),out(n);std::mt19937 rng(12937+form);for(auto&v:input)v=form==0?rng():form==1?(rng()&0xfffffu):rng()%8;auto expected=input;std::sort(expected.begin(),expected.end());abw::workspace ws;
  auto old=baseline::detail::choose(input);auto now=abw::detail::choose(input);if(int(old.selected)!=int(now.selected)||old.bits!=now.bits||old.shift!=now.shift)throw std::runtime_error("repeated route");
  double first=measure([&](V&a){abw::sort(a,ws);},input,expected,out);auto retained=ws.retained_bytes();double base=0,def=0,reuse=0;
  for(int repeat=0;repeat<8;++repeat){if(repeat%2){reuse+=measure([&](V&a){abw::sort(a,ws);},input,expected,out);def+=measure([](V&a){abw::sort(a);},input,expected,out);base+=measure([](V&a){baseline::sort(a);},input,expected,out);}else{base+=measure([](V&a){baseline::sort(a);},input,expected,out);def+=measure([](V&a){abw::sort(a);},input,expected,out);reuse+=measure([&](V&a){abw::sort(a,ws);},input,expected,out);}if(ws.retained_bytes()!=retained)throw std::runtime_error("repeated capacity changed");}
  ws.release();if(ws.capacity())throw std::runtime_error("repeated release");
  std::cout<<"{\"form\":"<<form<<",\"n\":"<<n<<",\"repeats\":8,\"first_workspace_ms\":"<<first<<",\"v013_mean_ms\":"<<base/8<<",\"candidate_default_mean_ms\":"<<def/8<<",\"workspace_mean_ms\":"<<reuse/8<<",\"retained_bytes\":"<<retained<<",\"verified\":true,\"released\":true}\n";
 }
}catch(const std::exception&e){std::cerr<<e.what();return 1;}}
