from pathlib import Path
import json,hashlib,urllib.request,tarfile,subprocess,shutil,os,platform,random,datetime,statistics,math,zipfile
import numpy as np
root=Path.cwd();ref=root/'reference';w=root/'work/abw_new_real_v012';o=root/'results';w.mkdir(parents=True,exist_ok=True);o.mkdir(exist_ok=True)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for name,h in json.loads((ref/'package_manifest.json').read_text()).items():assert sha(ref/name)==h,name
frozen=json.loads((ref/'input_manifest.json').read_text());sources=frozen['sources'];d=w/'downloads';d.mkdir(exist_ok=True)
for s in sources:
 p=d/s['file'];print('Download',s['id'],flush=True)
 if not p.exists():
  with urllib.request.urlopen(s['url'],timeout=120) as f,p.open('wb') as out:shutil.copyfileobj(f,out)
 assert sha(p)==s['sha256'],s['id']
for name in ['sources.json','design.json']:shutil.copyfile(ref/name,w/name)
# Use the exact original extraction logic, stopping before its Windows build.
extract=(ref/'prepare.py').read_text().split('# Frozen headers:')[0]
extract=extract.replace("Path('outputs/abw_new_real_v012')","Path('results')")
exec(compile(extract,'original_exact_extraction','exec'))
fresh=json.loads((w/'input_manifest.json').read_text())
assert {c['name']:c['sha256'] for c in fresh['cases']}=={c['name']:c['sha256'] for c in frozen['cases']}
assert {c['name']:c['expected_sha256'] for c in fresh['cases']}=={c['name']:c['expected_sha256'] for c in frozen['cases']}
cases=frozen['cases'];(o/'input_manifest.json').write_text(json.dumps(frozen,indent=2))
cpuinfo=Path('/proc/cpuinfo').read_text();model=next(l.split(':',1)[1].strip() for l in cpuinfo.splitlines() if l.startswith('model name'));flags=next(l for l in cpuinfo.splitlines() if l.startswith('flags')).split()
assert 'avx2' in flags and 'bmi1' in flags,'AVX2/BMI1 required for existing comparison implementations'
allowed=sorted(os.sched_getaffinity(0));os.sched_setaffinity(0,{allowed[0]})
boost=root/'boost_1_90_0';archive=root/'boost.tar.gz'
if not boost.exists():
 print('Download verified Boost 1.90.0',flush=True)
 with urllib.request.urlopen('https://archives.boost.io/release/1.90.0/source/boost_1_90_0.tar.gz',timeout=120) as f,archive.open('wb') as out:shutil.copyfileobj(f,out)
 assert sha(archive)=='5e93d582aff26868d581a52ae78c7d8edf3f3064742c6e77901a1f18a437eea9'
 with tarfile.open(archive) as tf:tf.extractall(root,members=[m for m in tf.getmembers() if m.name.startswith('boost_1_90_0/boost/')],filter='data')
shutil.copytree(ref/'include',w/'include',dirs_exist_ok=True);shutil.copyfile(ref/'v011.hpp',w/'v011.hpp')
src=(ref/'benchmark.cpp').read_text().replace('#include <windows.h>','#include <sched.h>')
old='DWORD_PTR p,s;if(!GetProcessAffinityMask(GetCurrentProcess(),&p,&s)||!SetProcessAffinityMask(GetCurrentProcess(),p&(~p+1)))throw std::runtime_error("affinity");'
new='cpu_set_t allowed;CPU_ZERO(&allowed);if(sched_getaffinity(0,sizeof(allowed),&allowed))throw std::runtime_error("affinity");int cpu=0;while(cpu<CPU_SETSIZE&&!CPU_ISSET(cpu,&allowed))++cpu;cpu_set_t selected;CPU_ZERO(&selected);CPU_SET(cpu,&selected);if(sched_setaffinity(0,sizeof(selected),&selected))throw std::runtime_error("affinity set");'
assert old in src;src=src.replace(old,new);(w/'benchmark_linux.cpp').write_text(src)
compiler=shutil.which('g++');assert compiler
cmd=[compiler,'-std=c++20','-O3','-DNDEBUG','-mavx2','-mbmi','-pthread','-I'+str(w/'include'),'-I'+str(boost),'-I'+str(ref/'vendor'),str(w/'benchmark_linux.cpp'),'-o',str(w/'benchmark')]
q=subprocess.run(cmd,capture_output=True,text=True);(o/'build.json').write_text(json.dumps(dict(command=cmd,returncode=q.returncode,stdout=q.stdout,stderr=q.stderr),indent=2));assert q.returncode==0,q.stderr
env=dict(cpu_model=model,cpuinfo=cpuinfo,platform=platform.platform(),python=platform.python_version(),numpy=np.__version__,pyarrow=__import__('pyarrow').__version__,compiler=subprocess.check_output([compiler,'--version'],text=True),allowed_cpus=allowed,selected_cpu=allowed[0],meminfo=Path('/proc/meminfo').read_text(),utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),same_model_as_local='i5-14500' in model,virtualized_host=True,scope='Different Colab runtime CPU/OS/compiler; compare algorithms within each machine, no attribution solely to microarchitecture',public_header_sha256=sha(w/'include/abw/abw.hpp'),reference_public_header_sha256=sha(ref/'include/abw/abw.hpp'))
(o/'environment.json').write_text(json.dumps(env,indent=2));print('CPU',model,'compiler',env['compiler'].splitlines()[0],flush=True)
local=[json.loads(l) for l in (ref/'session0.jsonl').read_text().splitlines()];route={c['name']:c['result'] for c in local};total=0
for session in range(3):
 jobs=cases.copy();random.Random(527813+session).shuffle(jobs)
 with (o/f'session{session}.jsonl').open('x') as f:
  for i,c in enumerate(jobs):
   q=subprocess.run([str(w/'benchmark'),str(w/'inputs'/(c['name']+'.bin')),str(w/'inputs'/(c['name']+'.expected.bin')),str(624581+session),str(int(c['cafs_i32_eligible'])),str(int(c['counting_eligible']))],capture_output=True,text=True,check=True);v=json.loads(q.stdout);assert v['verified'] and len(v['runs'])==(len(c['methods'])-1)*16
   for key in ['baseline_route','candidate_route','candidate_bits','candidate_shift','direct_output']:assert v[key]==route[c['name']][key],(c['name'],key)
   total+=len(v['runs']);f.write(json.dumps(dict(name=c['name'],group=c['group'],n=c['n'],session=session,result=v))+'\n');f.flush();print(session+1,i+1,'/9',c['name'],flush=True)
for c in cases:
 assert sha(w/'inputs'/(c['name']+'.bin'))==c['sha256'];assert sha(w/'inputs'/(c['name']+'.expected.bin'))==c['expected_sha256']
# Same estimator and CSV construction, without the historical Windows report text.
analysis=(ref/'analyze.py').read_text().split("text='''")[0].replace("Path('outputs/abw_new_real_v012')","Path('results')")
exec(compile(analysis,'same_estimator','exec'))
assert total==2544
(o/'validation.json').write_text(json.dumps(dict(correctness=True,processes=27,timed_sorts=total,input_hashes_match=True,release_header_unchanged=env['public_header_sha256']==env['reference_public_header_sha256'],all_route_fields_match=True,cpu_model=model,same_cpu_model_as_local=env['same_model_as_local'],github_unchanged=True),indent=2))
for name in ['benchmark_linux.cpp','v011.hpp']:shutil.copyfile(w/name,o/name)
shutil.copyfile(w/'include/abw/abw.hpp',o/'v012.hpp')
report=f"# ABW v0.1.2 Colab CPU validation\n\nCPU: {model}\n\nCompiler: {env['compiler'].splitlines()[0]}\n\n27 processes, 2,544 sorts, every full-array check passed. All source input and expected hashes and routes match the Windows run. This is a hosted/virtualized runtime with a different OS/compiler; no hardware-only causal claim. No policy changes.\n\n| Method | N | GM speedup vs pdq | v0.1.2/method time |\n|---|---:|---:|---:|\n"
for name,v in metrics.items():report+=f"| {name} | {v['eligible']} | {v['gm_speedup_pdq']:.4f} | {v['gm_v012_over_method']:.4f} |\n"
report+='\n| Input | v0.1.2 ms | v0.1.1 ms | pdq ms | CAFS u32 ms | CAFS i32 ms | v0.1.2/v0.1.1 | Direct |\n|---|---:|---:|---:|---:|---:|---:|---|\n'
for x in rows:report+=f"| {x['name']} | {x['v012']:.5f} | {x['v011']:.5f} | {x['pdq']:.5f} | {x['CAFS_u32']:.5f} | {x['CAFS_i32']:.5f} | {x['v012_over_v011']:.4f} | {x['direct']} |\n"
(o/'report.md').write_text(report);print(report,flush=True)
hashes={p.name:sha(p) for p in o.iterdir() if p.is_file()};(o/'file_hashes.json').write_text(json.dumps(hashes,indent=2))
out=root/'abw_v012_colab_cpu_results.zip'
with zipfile.ZipFile(out,'w',zipfile.ZIP_DEFLATED) as z:
 for p in o.iterdir():
  if p.is_file():z.write(p,p.name)
print('RESULT_ZIP',out,flush=True)
