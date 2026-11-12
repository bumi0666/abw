#include <abw/abw.hpp>
#include "shift0.hpp"
#include "v010.hpp"
#include "reference_compat.hpp"
#include <windows.h>
const char* label(abw::detail::strategy s) {
 switch(s){case abw::detail::strategy::native:return "native";case abw::detail::strategy::skip:return "skip";case abw::detail::strategy::active8:return "active8";default:return "active12";}
}
int main(int argc,char**argv){try{
 if(argc!=5)throw std::runtime_error("input expected seed audit|bench");
 DWORD_PTR pm,sm;if(!GetProcessAffinityMask(GetCurrentProcess(),&pm,&sm)||!SetProcessAffinityMask(GetCurrentProcess(),pm&(~pm+1)))throw std::runtime_error("affinity");
 V src=load(argv[1]),expected=load(argv[2]);
 if(std::string(argv[4])=="audit"){
 auto old=pickE(src);auto now=abw::detail::choose(src);
 if(old.selected!=label(now.selected)||old.bits!=now.bits||old.shift!=now.shift)throw std::runtime_error("dispatch mismatch");
 bool direct=false; auto a=src,b=src;
 if(old.selected=="active8"||old.selected=="active12"){
 direct=bucketSingleS<false>(a,old.bits,old.shift);
 if(direct!=abw_shift0::detail::partition_and_sort(b,now.bits,now.shift)||a!=expected||b!=expected)throw std::runtime_error("partition mismatch");}
 a=src;abw_shift0::sort(a);if(a!=expected)throw std::runtime_error("public mismatch");
 std::cout<<"{\"selected\":\""<<old.selected<<"\",\"bits\":"<<old.bits<<",\"shift\":"<<old.shift<<",\"direct\":"<<(direct?"true":"false")<<",\"verified\":true}";return 0;}
 auto run=[&](int method){V a=src;auto start=Clock::now();
 if(method==0)abw_v010::sort(a);
 else if(method==1)abw::sort(a);
 else abw_shift0::sort(a);
 double ms=elapsed(start);if(a!=expected)throw std::runtime_error("incorrect result");return ms;};
 for(int m=0;m<3;m++)run(m);
 std::vector<int>order={0,1,2};std::mt19937 r(unsigned(std::stoul(argv[3])));std::shuffle(order.begin(),order.end(),r);
 const char* names[]={"v010","public_v011","measured_candidate"};std::cout<<std::setprecision(15)<<"{\"verified\":true,\"runs\":[";
 for(int rep=0;rep<3;rep++)for(int pos=0;pos<3;pos++){int m=order[(pos+rep)%3];double ms=run(m);if(rep||pos)std::cout<<",";std::cout<<"{\"method\":\""<<names[m]<<"\",\"rep\":"<<rep<<",\"position\":"<<pos<<",\"ms\":"<<ms<<"}";}
 std::cout<<"]}";
 }catch(const std::exception&e){std::cerr<<e.what();return 1;}return 0;}
