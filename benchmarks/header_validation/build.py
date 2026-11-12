import argparse,pathlib,subprocess
p=argparse.ArgumentParser();p.add_argument('--cxx',required=True);p.add_argument('--boost',type=pathlib.Path,required=True);p.add_argument('--out',type=pathlib.Path,required=True);a=p.parse_args()
r=pathlib.Path(__file__).resolve().parents[2]
cmd=[a.cxx,'-std=c++20','-O3','-DNDEBUG','-mavx2','-mbmi','-static','-pthread','-I'+str(r/'include'),'-I'+str(r/'tests'),'-I'+str(a.boost),str(pathlib.Path(__file__).with_name('benchmark.cpp')),'-o',str(a.out)]
print(cmd);subprocess.run(cmd,check=True)
