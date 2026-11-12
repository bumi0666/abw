import pathlib,json,hashlib,subprocess,random,sys,time
root=pathlib.Path(__file__).resolve().parents[2]
import argparse
ap=argparse.ArgumentParser();ap.add_argument('--inputs',type=pathlib.Path,required=True);ap.add_argument('--manifest',type=pathlib.Path,required=True);ap.add_argument('--exe',type=pathlib.Path,required=True);ap.add_argument('--out',type=pathlib.Path,required=True);a=ap.parse_args();a.out.mkdir(parents=True,exist_ok=True)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
cases=json.loads(a.manifest.read_text())['cases']; assert len(cases)==51
sources={str(p.relative_to(root)).replace('\\','/'):sha(p) for p in [root/'include/abw/abw.hpp',root/'tests/reference/frozen_prefix.hpp',root/'tests/reference/sort_experiment_F1.cpp',pathlib.Path(__file__),pathlib.Path(__file__).with_name('benchmark.cpp')]}
protocol=dict(sessions=3,repeats=9,methods=['pdq','frozen_B','public_header'],inputs=cases,source_hashes=sources,binary_sha256=sha(a.exe),timing='outer wall clock including original frozen internal clocks; copy, audit and verification excluded',equivalence='descriptive +/-5% per-input band; not a statistical equivalence proof',filtering='none')
(a.out/'protocol.json').write_text(json.dumps(protocol,indent=2))
for c in cases:
 for suffix,key in [('.bin','sha256'),('.expected.bin','expected_sha256')]:assert sha(a.inputs/(c['name']+suffix))==c[key]
with (a.out/'audits.jsonl').open('x') as f:
 for c in cases:
  cmd=[str(a.exe.resolve()),str((a.inputs/(c['name']+'.bin')).resolve()),str((a.inputs/(c['name']+'.expected.bin')).resolve()),'123','audit']
  q=subprocess.run(cmd,capture_output=True,text=True,check=True);f.write(json.dumps(dict(name=c['name'],result=json.loads(q.stdout)))+'\n')
for session in range(3):
 jobs=cases.copy();random.Random(68172+session).shuffle(jobs)
 with (a.out/f'session{session}.jsonl').open('x') as f:
  for i,c in enumerate(jobs):
   q=subprocess.run([str(a.exe.resolve()),str((a.inputs/(c['name']+'.bin')).resolve()),str((a.inputs/(c['name']+'.expected.bin')).resolve()),str(128379+session),'bench'],capture_output=True,text=True,check=True)
   f.write(json.dumps(dict(name=c['name'],group=c['group'],n=c['n'],session=session,result=json.loads(q.stdout)))+'\n');f.flush()
   print(session,i+1,51,c['name'],flush=True)
assert all(sha(root/p)==h for p,h in sources.items())

