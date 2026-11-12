import pathlib,json,hashlib,subprocess,random,time,sys
import numpy as np
R=pathlib.Path(__file__).resolve().parent;I=R/'inputs';design=json.loads((R/'design_preregistered.json').read_text(encoding='utf8'));real=json.loads((R/'input_manifest.json').read_text())['real'];sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
ALLMETHODS=['pdq','E_pdq','ABW_B','spread','CAFS_u32','CAFS_i32','counting']
S=R/'synthetic_manifest.json'
if not S.exists():
 synth=[]
 for n in design['regression']['sizes']:
  for kind in design['regression']['distributions']:
   name=f'synthetic_{n}_{kind}';p=I/(name+'.bin');e=I/(name+'.expected.bin');a=np.fromfile(p,dtype='<u4');expected=np.fromfile(e,dtype='<u4');assert len(a)==n and np.array_equal(np.sort(a),expected)
   synth.append(dict(name=name,n=n,kind=kind,group='synthetic_regression',seed=design['regression']['seed'],min=int(a.min()),max=int(a.max()),sha256=sha(p),expected_sha256=sha(e),counting_eligible=(int(a.max())-int(a.min())+1)*8<=67108864,cafs_i32_eligible=int(a.max())<=2147483647))
 S.write_text(json.dumps(synth,indent=2))
else:synth=json.loads(S.read_text())
cases=real+synth
for c in cases:
 c['methods']=ALLMETHODS.copy()
 if not c['counting_eligible']:c['methods'].remove('counting')
 if not c['cafs_i32_eligible']:c['methods'].remove('CAFS_i32')
manifest=R/'benchmark_manifest.json'
if not manifest.exists():manifest.write_text(json.dumps(dict(cases=cases,build=json.loads((R/'build.json').read_text()),preregistered_sha256=sha(R/'design_preregistered.json'),input_manifest_sha256=sha(R/'input_manifest.json'),source_hashes={str(p.relative_to(R)):sha(p) for p in [R/'benchmark.exe',R/'memory.exe',R/'benchmark.cpp',R/'vendor/cafs.hpp',R/'vendor/cafs2.hpp',R/'memory_profile.hpp']}),indent=2))
lock=json.loads(manifest.read_text())
def frozen():
 for p,h in lock['source_hashes'].items():assert sha(R/p)==h,p
 assert sha(R/'design_preregistered.json')==lock['preregistered_sha256']
def execute(mode,c,session=0,method=None):
 p=I/(c['name']+'.bin');e=I/(c['name']+'.expected.bin');methods=[method] if method else c['methods'];exe='memory.exe' if mode=='memory' else 'benchmark.exe'
 cp=subprocess.run([str(R/exe),mode,str(p),str(e),','.join(methods),str((0x3f17a985^((session+1)*0x85ebca6b))&0xffffffff)],capture_output=True,text=True)
 if cp.returncode:raise RuntimeError(c['name']+' '+mode+' '+cp.stderr)
 q=json.loads(cp.stdout)
 if mode!='audit':assert q['verified'] and all(x['verified'] for x in q['runs'])
 return dict(name=c['name'],group=c['group'],session=session,result=q)
mode=sys.argv[1] if len(sys.argv)>1 else 'bench'
if mode=='bench':
 frozen();start=time.time()
 for group in ['real','synthetic_regression']:
  subset=[c for c in cases if c['group']==group]
  for session in range(3):
   output=R/f'{group}_session{session}.jsonl';assert not output.exists(),str(output)
   jobs=subset.copy();random.Random(0x5ea218d9^session).shuffle(jobs)
   with output.open('x') as f:
    for i,c in enumerate(jobs):
     rr=execute('bench',c,session);f.write(json.dumps(rr)+'\n');f.flush();print(group,'S',session+1,i+1,len(jobs),c['name'],'elapsed',round(time.time()-start,1),flush=True)
 frozen()
elif mode=='memory':
 frozen();target=R/'memory.jsonl';assert not target.exists()
 with target.open('x') as f:
  for i,c in enumerate(cases):
   for method in c['methods']:
    r=execute('memory',c,method=method);assert r['result']['runs'][0]['heap_retained']==0,r;f.write(json.dumps(r)+'\n');f.flush()
   print('Memory',i+1,len(cases),c['name'],flush=True)
 with (R/'audits.jsonl').open('x') as f:
  for c in cases:f.write(json.dumps(execute('audit',c))+'\n')
 frozen()
else:raise ValueError(mode)
