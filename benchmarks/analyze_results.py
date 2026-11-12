import pathlib,json,statistics as st,math,csv,hashlib,collections,datetime
import numpy as np
R=pathlib.Path(__file__).resolve().parent;W=R;O=R/'review';O.mkdir(exist_ok=True)
manifest=json.loads((R/'benchmark_manifest.json').read_text());cases=manifest['cases'];C={c['name']:c for c in cases};records=[json.loads(x) for p in sorted(R.glob('*_session*.jsonl')) for x in p.read_text().splitlines()];memory=[json.loads(x) for x in (R/'memory.jsonl').read_text().splitlines()];audits={x['name']:x['result'] for x in [json.loads(x) for x in (R/'audits.jsonl').read_text().splitlines()]};build=json.loads((R/'build.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert len(cases)==51 and len(records)==153 and len({(r['name'],r['session']) for r in records})==153 and len(audits)==51
for p,h in manifest['source_hashes'].items():assert sha(R/p)==h,p
assert sha(R/'design_preregistered.json')==manifest['preregistered_sha256']
# No post-run input changes. Numpy reference is regenerated independently here.
for c in cases:
 p=R/'inputs'/(c['name']+'.bin');e=R/'inputs'/(c['name']+'.expected.bin');assert sha(p)==c['sha256'] and sha(e)==c['expected_sha256']
 a=np.fromfile(p,dtype='<u4');expected=np.fromfile(e,dtype='<u4');assert len(a)==c['n'] and np.array_equal(np.sort(a),expected)
rows=[];sessionrows=[];totals=collections.Counter();lookup={}
for r in records:
 c=C[r['name']];a=audits[r['name']];rr=r['result'];assert rr['verified'] and rr['n']==c['n'];M=len(c['methods']);assert len(rr['runs'])==M*M
 for rep in range(M):assert {x['position'] for x in rr['runs'] if x['rep']==rep}==set(range(M))
 for m in c['methods']:
  xs=[x for x in rr['runs'] if x['method']==m];assert len(xs)==M and {x['rep'] for x in xs}==set(range(M)) and {x['position'] for x in xs}==set(range(M))
  assert all(x['verified'] and math.isfinite(x['ms']) and x['ms']>0 for x in xs)
  if m in ['E_pdq','ABW_B']:assert all(x['selected']==a['selected'] for x in xs)
  assert all(x['B_direct']==(m=='ABW_B' and a['B_direct_expected']) for x in xs)
  v=dict(name=c['name'],group=c['group'],n=c['n'],session=r['session']+1,method=m,median_ms=st.median(x['ms'] for x in xs),min_ms=min(x['ms'] for x in xs),max_ms=max(x['ms'] for x in xs),B_direct=a['B_direct_expected'],selected=a['selected'])
  sessionrows.append(v);lookup[c['name'],r['session'],m]=v['median_ms'];totals[c['group']]+=M
  for x in xs:rows.append(dict(name=c['name'],group=c['group'],n=c['n'],session=r['session']+1,**x))
mem={}
for r in memory:
 c=C[r['name']];assert len(r['result']['runs'])==1;x=r['result']['runs'][0];assert x['verified'] and x['heap_retained']==0
 assert x['B_direct']==(x['method']=='ABW_B' and audits[r['name']]['B_direct_expected']);mem[c['name'],x['method']]=x
assert len(mem)==sum(len(c['methods']) for c in cases)
inputrows=[];byinput={}
for c in cases:
 name=c['name'];res={m:st.median(lookup[name,s,m] for s in range(3)) for m in c['methods']};q=dict(name=name,group=c['group'],n=c['n'],selected=audits[name]['selected'],B_direct=audits[name]['B_direct_expected'],first_mixed=audits[name]['first_mixed'],best_method=min(res,key=res.get),best_ms=min(res.values()),B_vs_best=res['ABW_B']/min(res.values()))
 for m in c['methods']:q[m+'_ms']=res[m];q[m+'_heap_peak']=mem[name,m]['heap_peak']
 q['pdq_over_B']=st.median(lookup[name,s,'pdq']/lookup[name,s,'ABW_B'] for s in range(3));q['E_over_B']=st.median(lookup[name,s,'E_pdq']/lookup[name,s,'ABW_B'] for s in range(3));q['B_minus_E_ms']=st.median(lookup[name,s,'ABW_B']-lookup[name,s,'E_pdq'] for s in range(3));q['B_vs_E_pct']=(st.median(lookup[name,s,'ABW_B']/lookup[name,s,'E_pdq'] for s in range(3))-1)*100
 inputrows.append(q);byinput[name]=q
real=[x for x in inputrows if x['group']=='real'];synthetic=[x for x in inputrows if x['group']!='real'];GM=lambda xs:math.exp(st.mean(math.log(x) for x in xs))
comparison=[]
for method in ['pdq','E_pdq','spread','CAFS_u32','CAFS_i32','counting']:
 ratios=[st.median(lookup[x['name'],s,'ABW_B']/lookup[x['name'],s,method] for s in range(3)) for x in real]
 comparison.append(dict(baseline=method,B_faster_gt5pct=sum(v<.95 for v in ratios),near_tie_5pct=sum(.95<=v<=1.05 for v in ratios),B_slower_gt5pct=sum(v>1.05 for v in ratios),GM_speedup=GM(1/v for v in ratios),ratios_B_over_baseline=ratios))
strictwins=[]
for x in real:
 name=x['name'];wins=[]
 for s in range(3):
  wins.append(all(lookup[name,s,'ABW_B']<.95*lookup[name,s,m] for m in C[name]['methods'] if m!='ABW_B'))
 if all(wins):strictwins.append(name)
reg=[]
for n in [100000,1000000,10000000]:
 keys=[x['name'] for x in synthetic if x['n']==n];g=[]
 for s in range(3):g.append(dict(session=s+1,n=n,B_over_pdq_speedup=GM(lookup[k,s,'pdq']/lookup[k,s,'ABW_B'] for k in keys),E_over_pdq_speedup=GM(lookup[k,s,'pdq']/lookup[k,s,'E_pdq'] for k in keys)))
 reg+=g
maxima={}
for group in ['real','synthetic_regression']:
 rr=[x for x in sessionrows if x['group']==group and x['method']=='ABW_B'];v=max(rr,key=lambda x:x['median_ms']/lookup[x['name'],x['session']-1,'pdq']);s=v['session']-1
 maxima[group]=dict(name=v['name'],session=v['session'],B_ms=v['median_ms'],pdq_ms=lookup[v['name'],s,'pdq'],ratio_B_over_pdq=v['median_ms']/lookup[v['name'],s,'pdq'],B_direct=v['B_direct'],selected=v['selected'])
validation=dict(passed=True,real_inputs=len(real),public_dataset_sources=3,synthetic_regression_inputs=len(synthetic),fresh_timing_processes=len(records),timed_sorts=sum(totals.values()),timed_sorts_by_group=dict(totals),memory_processes=len(mem),exact_output_checks_passed=True,input_hashes_verified=True,independent_numpy_reference_verified=True,method_order_balanced=True,all_E_B_choices_match=True,all_B_flags_match_audit=True,production_no_heap_instrumentation=True,profile_heap_returns_to_zero=True,no_ABW_modification=True,preregistered_protocol_hash_verified=True,second_real_hardware_test=False)
# Time and memory summaries retain all per-input values, no outlier filtering.
def csvwrite(name,data):
 fields=list(dict.fromkeys(k for x in data for k in x))
 with (O/name).open('w',newline='',encoding='utf8') as f:w=csv.DictWriter(f,fieldnames=fields);w.writeheader();w.writerows(data)
csvwrite('raw_timings.csv',rows);csvwrite('session_medians.csv',sessionrows);csvwrite('input_results.csv',inputrows);csvwrite('real_comparison.csv',comparison);csvwrite('synthetic_summary.csv',reg)
csvwrite('memory.csv',[dict(name=r['name'],group=r['group'],**r['result']['runs'][0]) for r in memory]);csvwrite('real_input_metadata.csv',json.loads((R/'input_manifest.json').read_text())['real'])
result=dict(validation=validation,real_results=real,real_comparisons=comparison,strict_real_wins_all_sessions_gt5pct=strictwins,direct_inputs=sum(x['B_direct'] for x in real),direct_timed_calls=sum(x['group']=='real' and x['method']=='ABW_B' and x['B_direct'] for x in rows),synthetic_summary=reg,maxima=maxima,decision='not_automatically_assessed',decision_text='수치만 재집계한다. 최종 판정은 해당 실행의 원시자료와 보고서를 검토해 별도로 내린다.')
(O/'summary.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf8');(O/'validation.json').write_text(json.dumps(validation,indent=2))
print(json.dumps({k:v for k,v in result.items() if k not in ['real_results']},ensure_ascii=False,indent=2));print('REAL_RESULTS',json.dumps(real,ensure_ascii=False,indent=2))
