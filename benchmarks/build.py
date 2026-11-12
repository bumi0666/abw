import argparse,pathlib,json,hashlib,subprocess,shutil,sys,os,platform
if sys.platform!='win32':raise SystemExit('The frozen performance harness is Windows-only. The public ABW header does not depend on Windows APIs.')
import ctypes,winreg
R=pathlib.Path(__file__).resolve().parent
ap=argparse.ArgumentParser();ap.add_argument('--cxx',default=os.environ.get('CXX','clang++'));ap.add_argument('--boost',required=True);a=ap.parse_args();compiler=shutil.which(a.cxx) or a.cxx;boost=pathlib.Path(a.boost).resolve();assert (boost/'boost/sort/pdqsort/pdqsort.hpp').is_file()
provenance=json.loads((R/'results/provenance.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for name,h in provenance['measured_source_hashes'].items():
 p=R/name
 if p.is_file():assert sha(p)==h, name
cmd=[compiler,'-std=c++20','-O3','-DNDEBUG','-mavx2','-mbmi','-static','-pthread','-I'+str(boost),'-I'+str(R/'frozen/outputs'),'-I'+str(R/'vendor'),str(R/'benchmark.cpp'),'-lpsapi'];commands=[]
for exe,flags in [('benchmark.exe',[]),('memory.exe',['-DABW_MEMPROFILE'])]:
 command=cmd+flags+['-o',str(R/exe)];subprocess.run(command,check=True);commands.append(command)
(R/'build.json').write_text(json.dumps(dict(compiler=compiler,commands=commands,frozen_prefix_sha256=provenance['algorithm_prefix_sha256'],unchanged_B_prefix=True),indent=2))
K=ctypes.windll.kernel32;K.GetCurrentProcess.restype=ctypes.c_void_p;K.GetProcessAffinityMask.argtypes=[ctypes.c_void_p,ctypes.POINTER(ctypes.c_size_t),ctypes.POINTER(ctypes.c_size_t)];mask=ctypes.c_size_t();system=ctypes.c_size_t();assert K.GetProcessAffinityMask(K.GetCurrentProcess(),ctypes.byref(mask),ctypes.byref(system))
class MS(ctypes.Structure):_fields_=[('length',ctypes.c_uint32),('load',ctypes.c_uint32)]+[(n,ctypes.c_uint64) for n in ['total_phys','avail_phys','total_page','avail_page','total_virtual','avail_virtual','avail_extended']]
m=MS();m.length=ctypes.sizeof(m);assert K.GlobalMemoryStatusEx(ctypes.byref(m))
with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE,r'HARDWARE\DESCRIPTION\System\CentralProcessor\0') as k:cpu=winreg.QueryValueEx(k,'ProcessorNameString')[0]
(R/'environment.json').write_text(json.dumps(dict(platform=platform.platform(),cpu=cpu,logical_cpus=os.cpu_count(),physical_memory_bytes=m.total_phys,allowed_affinity_mask=mask.value,selected_affinity_mask=mask.value&-mask.value,compiler=subprocess.check_output([compiler,'--version'],text=True),python=platform.python_version(),node=subprocess.check_output(['node','--version'],text=True).strip(),second_hardware_run=False),indent=2))
