from pathlib import Path
import json,statistics as st,math,csv
import argparse
p=argparse.ArgumentParser();p.add_argument('results',type=Path);a=p.parse_args();r=a.results;records=[json.loads(l) for p in sorted(r.glob('session*.jsonl')) for l in p.read_text().splitlines()]
rows=[]
for x in records:
 v={m:st.median(t['ms'] for t in x['result']['runs'] if t['method']==m) for m in ['pdq','frozen_B','public_header']}
 rows.append(dict(name=x['name'],group=x['group'],n=x['n'],session=x['session'],**v,ratio=v['public_header']/v['frozen_B']))
with (r/'session_medians.csv').open('w',newline='') as f:
 w=csv.DictWriter(f,fieldnames=list(rows[0]));w.writeheader();w.writerows(rows)
summary=[]
for name in sorted({x['name'] for x in rows}):
 s=[x for x in rows if x['name']==name];q=dict(name=name,group=s[0]['group'],n=s[0]['n'],sessions=len(s))
 for m in ['pdq','frozen_B','public_header','ratio']:q[m]=st.median(x[m] for x in s)
 q['min_ratio']=min(x['ratio'] for x in s);q['max_ratio']=max(x['ratio'] for x in s);summary.append(q)
with (r/'input_results.csv').open('w',newline='') as f:
 w=csv.DictWriter(f,fieldnames=list(summary[0]));w.writeheader();w.writerows(summary)
print('sessions',len(records),'sorts',sum(len(x['result']['runs']) for x in records))
for group in ['real','synthetic_regression']:
 s=[x for x in summary if x['group']==group];print(group,'GM header/B',math.exp(st.mean(math.log(x['ratio']) for x in s)))
for x in summary:
 if x['group']=='real' or not .95<=x['ratio']<=1.05:print(x)
