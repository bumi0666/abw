import json,csv,pathlib,subprocess,statistics as st
import argparse
ap=argparse.ArgumentParser();ap.add_argument('--results',type=pathlib.Path,required=True);ap.add_argument('--inputs',type=pathlib.Path,required=True);ap.add_argument('--exe',type=pathlib.Path,required=True);args=ap.parse_args();r=args.results; rows=list(csv.DictReader((r/'session_medians.csv').open())); names=sorted({x['name'] for x in rows if not .95<=float(x['ratio'])<=1.05})
p=args.inputs.resolve();exe=args.exe.resolve()
with (r/'diagnostic.jsonl').open('x') as f:
 for session in range(3):
  for name in names:
   q=subprocess.run([str(exe),str(p/(name+'.bin')),str(p/(name+'.expected.bin')),str(772920+session),'bench'],capture_output=True,text=True,check=True)
   v=json.loads(q.stdout);f.write(json.dumps(dict(name=name,session=session,result=v))+'\n');f.flush()
   med={m:st.median(x['ms'] for x in v['runs'] if x['method']==m) for m in ['frozen_B','public_header']};print(name,session,med['public_header']/med['frozen_B'],flush=True)
