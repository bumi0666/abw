const fs=require('node:fs'),os=require('node:os'),crypto=require('node:crypto');
const {performance}=require('node:perf_hooks');
const {make:oldMake,rng,decide,sampleD,chooseD,orderProbe}=require('../work/sort_E_legacy.cjs');
const U=4294967296;
const config={version:'E-frozen-1',sizes:[100000,1000000,10000000],seeds:[0x733fd19b,0xb41a07c5,0x2d58ee31],repetitions_per_seed:3,kinds:['uniform','normal','exponential','narrow','duplicates','duplicates_narrow','sorted','partial','reverse','upper_only','lower_only','discontinuous','extreme_skew','mixture','probe_trap'],methods:['native','fixed8','fixed12','C','D','E','active8','active12'],oracle_candidates:['native','fixed8','fixed12','active8','active12'],E:{order_probe:256,order_minor_direction_threshold:0.1,sample_count:'512 if n<1000000 else 2048',duplicate_threshold:0.5,mode_threshold:0.1,bits:'8 for high duplicate else 12',min_effective_entropy:'1 for high duplicate else 4',bit_scan:'full-array OR of XOR with first element, before any prefix-based partition',order:'order probe -> detailed sample -> full bit scan -> shifted sample entropy -> execute'},definitions:{normal:'clipped mean=2^31 sd=2^29',exponential:'clipped mean=2^29',narrow:'uint16 random',duplicates:'8 values spread across uint32',duplicates_narrow:'0..7',partial:'sorted ramp, n*0.01 random pair swaps',upper_only:'random upper 12 bits, fixed lower 20 bits=0x54321',lower_only:'fixed prefix 0xabc00000, random lower 20 bits',discontinuous:'0xa0000000 | (random & 0x00f00f0f)',extreme_skew:'99.9% value 0x80000000, 0.1% full-range random',mixture:'40% narrow-prefix cluster, 40% full-range uniform, 20% eight spread values',probe_trap:'uniform with ascending neighbors installed at all 256 fixed order-probe positions; adversarial diagnostic only'},independence:'New seeds and five new distribution families; probe_trap is a separate adversarial diagnostic, not an ordinary holdout. No policy retuning after first heldout run. Three repeated timings for each identical seed/input. Oracle is min of per-strategy 3-run medians per seed, then summarized over seeds.',timing:'Input clone excluded equally. Samples, decision, full bit scan, metadata allocation, scatter, native sorting and copy-back included. Separate phase times collected. E_without_analysis subtracts order/sample/decision and bit scan, retains distribution/allocation/sort/copy. Per-call timer overhead remains; JIT/GC and system noise not isolated.',memory:'Explicit extra buffer for distribution=4*n bytes plus ~12*bucketCount metadata bytes. Source/expected/input clone are harness buffers. RSS observations are sampled, not measured peak. No external optimized sorter baselines in this JS phase.'};
function median(x){return [...x].sort((a,b)=>a-b)[Math.floor(x.length/2)];}
function shuffle(a,r){for(let j=a.length-1;j>0;j--){const k=r()%(j+1);[a[j],a[k]]=[a[k],a[j]];}return a;}
function probeIndices(n){const m=Math.min(256,n-1),r=rng(0x12577abc),out=[];for(let j=0;j<m;j++){const lo=Math.floor(j*(n-1)/m),hi=Math.floor((j+1)*(n-1)/m);out.push(lo+r()%Math.max(1,hi-lo));}return out;}
function make(n,kind,seed){
 if(!['upper_only','lower_only','discontinuous','extreme_skew','mixture','probe_trap'].includes(kind))return oldMake(n,kind,seed);
 const a=new Uint32Array(n),r=rng(seed);
 for(let i=0;i<n;i++){
  let v;if(kind==='upper_only')v=((r()&4095)*1048576)+0x54321;
  else if(kind==='lower_only')v=(0xabc00000|(r()&0xfffff))>>>0;
  else if(kind==='discontinuous')v=(0xa0000000|(r()&0x00f00f0f))>>>0;
  else if(kind==='extreme_skew')v=r()%1000===0?r():0x80000000;
  else if(kind==='mixture'){const k=r()%10;v=k<4?((0xabc00000|(r()&0xfffff))>>>0):k<8?r():(r()%8)*Math.floor((U-1)/7);}
  else v=r();a[i]=v;
 }
 if(kind==='probe_trap')for(const i of probeIndices(n))if(a[i]>a[i+1])[a[i],a[i+1]]=[a[i+1],a[i]];
 return a;
}
function fullBits(a,bits){let diff=0;const first=a[0];for(let i=1;i<a.length;i++)diff|=a[i]^first;return {diff:diff>>>0,shift:Math.max(0,32-Math.clz32(diff)-bits)};}
function sampleE(a){
 const m=Math.min(a.length-1,a.length<1000000?512:2048),r=rng(0x55ab76c1),freq=new Map(),values=new Uint32Array(m);let maxCount=0;
 for(let j=0;j<m;j++){const lo=Math.floor(j*(a.length-1)/m),hi=Math.floor((j+1)*(a.length-1)/m),v=a[lo+r()%Math.max(1,hi-lo)];values[j]=v;const c=(freq.get(v)||0)+1;freq.set(v,c);maxCount=Math.max(maxCount,c);}
 return {values,m,unique_ratio:freq.size/m,duplicate_ratio:1-freq.size/m,mode_ratio:maxCount/m};
}
function shiftedEntropy(s,shift,bits){const counts=new Uint32Array(1<<bits),mask=(1<<bits)-1;for(const v of s.values)counts[(v>>>shift)&mask]++;let H=0;for(const c of counts)if(c){const p=c/s.m;H-=p*Math.log2(p);}return H;}
function distribute(a,bits,shift){
 const t=performance.now(),B=1<<bits,counts=new Uint32Array(B),starts=new Uint32Array(B+1),mask=B-1;
 for(let i=0;i<a.length;i++)counts[(a[i]>>>shift)&mask]++;
 for(let b=0;b<B;b++)starts[b+1]=starts[b]+counts[b];
 const cursor=starts.slice(0,B),tmp=new Uint32Array(a.length);
 for(let i=0;i<a.length;i++){const v=a[i];tmp[cursor[(v>>>shift)&mask]++]=v;}
 const t2=performance.now();for(let b=0;b<B;b++)if(counts[b]>1)tmp.subarray(starts[b],starts[b+1]).sort();a.set(tmp);
 return {distribution_ms:t2-t,sort_copy_ms:performance.now()-t2};
}
function run(src,method){
 const a=src.slice(),t0=performance.now();let selected=method,stats=null,bits=0,shift=0,probe_ms=0,bit_ms=0,distribution_ms=0,sort_copy_ms=0;
 if(method==='C'){const t=performance.now();stats=decide(a);selected=stats.apply?'fixed8':'native';probe_ms+=performance.now()-t;}
 else if(method==='D'){const t=performance.now();if(orderProbe(a))selected='native';else{stats=sampleD(a);const d=chooseD(stats,true);selected=d==='native'?'native':d==='b8'?'fixed8':'fixed12';}probe_ms+=performance.now()-t;}
 else if(method==='E'){
  const t=performance.now();if(a.length<100000||orderProbe(a))selected='native';else{stats=sampleE(a);selected='pending';}probe_ms+=performance.now()-t;
  if(selected==='pending'){
   const dup=stats.duplicate_ratio>=0.5||stats.mode_ratio>=0.1;bits=dup?8:12;
   const tb=performance.now(),full=fullBits(a,bits);bit_ms+=performance.now()-tb;shift=full.shift;
   const td=performance.now();stats.entropy=shiftedEntropy(stats,shift,bits);stats.bits=bits;stats.shift=shift;stats.diff=full.diff;
   selected=full.diff===0?'skip':stats.entropy>=(dup?1:4)?'active'+bits:'native';probe_ms+=performance.now()-td;
  }
 }
 if(selected==='active8'||selected==='active12'){
  bits=+selected.slice(6);if(method!=='E'){const t=performance.now(),full=fullBits(a,bits);bit_ms+=performance.now()-t;shift=full.shift;if(full.diff===0)selected='skip';}
 }else if(selected==='fixed8'||selected==='fixed12'){bits=+selected.slice(5);shift=32-bits;}
 if(selected==='native'){const t=performance.now();a.sort();sort_copy_ms=performance.now()-t;}
 else if(selected!=='skip'){const phases=distribute(a,bits,shift);distribution_ms=phases.distribution_ms;sort_copy_ms=phases.sort_copy_ms;}
 const total_ms=performance.now()-t0;
 let metrics=null;if(stats&&method==='E'){metrics={...stats};delete metrics.values;}
 return {a,total_ms,probe_ms,bit_ms,analysis_ms:probe_ms+bit_ms,execution_ms:total_ms-probe_ms-bit_ms,distribution_ms,sort_copy_ms,selected,metrics,buffer_bytes:selected!=='native'&&selected!=='skip'?4*a.length:0};
}
function verify(actual,expected,label){for(let i=0;i<actual.length;i++)if(actual[i]!==expected[i])throw Error('Mismatch '+label+' at '+i);}
function correctness(){
 const arrays=[new Uint32Array(),Uint32Array.of(0),Uint32Array.of(0xffffffff,0,0x80000000,1,0x7fffffff),Uint32Array.of(0xa0000001,0xa0000000,0xa000000f),Uint32Array.of(7,7,7)];
 for(let k=0;k<40;k++)arrays.push(make(10+k*7,['uniform','lower_only','discontinuous','duplicates_narrow'][k%4],k+743));
 for(const a of arrays){const exp=a.slice().sort();for(const m of config.methods)verify(run(a,m).a,exp,'small '+m);}
}
function main(){
 const codeHash=crypto.createHash('sha256').update(fs.readFileSync(__filename)).digest('hex'),legacyHash=crypto.createHash('sha256').update(fs.readFileSync(require.resolve('../work/sort_E_legacy.cjs'))).digest('hex');
 const lock={locked_at:new Date().toISOString(),code_sha256:codeHash,legacy_sha256:legacyHash,config};
 if(fs.existsSync('outputs/sort_experiment_E_policy.json'))throw Error('Policy lock exists; refusing to overwrite frozen run.');
 fs.writeFileSync('outputs/sort_experiment_E_policy.json',JSON.stringify(lock,null,2));
 correctness();console.log('Policy frozen; correctness edge cases passed.');
 for(let i=0;i<8;i++)for(const k of ['uniform','narrow','duplicates','sorted']){const a=make(100000,k,123+i);for(const m of config.methods)run(a,m);}
 const records=[],jobs=[];for(const n of config.sizes)for(const kind of config.kinds)for(const seed of config.seeds)jobs.push({n,kind,seed});
 shuffle(jobs,rng(0x9c29b711));fs.writeFileSync('outputs/sort_experiment_E_raw.jsonl','');
 let maxObservedRss=0;
 for(let j=0;j<jobs.length;j++){
  const {n,kind,seed}=jobs[j],src=make(n,kind,seed),expected=src.slice().sort(),runs=Object.fromEntries(config.methods.map(m=>[m,[]]));
  for(let rep=0;rep<config.repetitions_per_seed;rep++){
   const order=shuffle([...config.methods],rng((seed^n^((rep+1)*7919))>>>0));
   for(const m of order){const out=run(src,m);verify(out.a,expected,n+' '+kind+' '+seed+' '+rep+' '+m);delete out.a;runs[m].push(out);maxObservedRss=Math.max(maxObservedRss,process.memoryUsage().rss);}
  }
  const medians={};for(const m of config.methods){const d={};for(const key of ['total_ms','probe_ms','bit_ms','analysis_ms','execution_ms','distribution_ms','sort_copy_ms'])d[key]=median(runs[m].map(x=>x[key]));d.selected=[...new Set(runs[m].map(x=>x.selected))].join('|');d.metrics=runs[m][0].metrics;medians[m]=d;}
  const oracle=config.oracle_candidates.reduce((best,m)=>medians[m].total_ms<medians[best].total_ms?m:best,config.oracle_candidates[0]);
  const oracleExec=config.oracle_candidates.reduce((best,m)=>medians[m].execution_ms<medians[best].execution_ms?m:best,config.oracle_candidates[0]);
  const record={n,kind,seed,medians,oracle,oracle_total_ms:medians[oracle].total_ms,oracle_execution_only:oracleExec,oracle_execution_ms:medians[oracleExec].execution_ms,runs};records.push(record);fs.appendFileSync('outputs/sort_experiment_E_raw.jsonl',JSON.stringify(record)+'\n');
  console.log(JSON.stringify({done:j+1,jobs:jobs.length,n,kind,seed,native_ms:+medians.native.total_ms.toFixed(2),E_ms:+medians.E.total_ms.toFixed(2),selected:medians.E.selected,oracle}));
 }
 const result={...lock,node:process.version,cpu:os.cpus()[0].model,max_observed_process_rss_bytes:maxObservedRss,records};fs.writeFileSync('outputs/sort_experiment_E_results.json',JSON.stringify(result,null,2));console.log('COMPLETE all jobs and exact validations.');
}
if(require.main===module)main();module.exports={config,run,make,median};
