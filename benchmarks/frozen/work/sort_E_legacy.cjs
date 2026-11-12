const {performance}=require('node:perf_hooks');
const fs=require('node:fs'),os=require('node:os');
const U=4294967296, bitsList=[4,6,8,10,12];
function rng(seed){let x=seed>>>0;return()=>{x^=x<<13;x^=x>>>17;x^=x<<5;return (x>>>0);};}
function make(n,kind,seed){
 const r=rng(seed), a=new Uint32Array(n), unit=()=> (r()+0.5)/U;
 for(let i=0;i<n;i++) {
  let v;
  if(kind==='normal') v=U*(0.5+Math.sqrt(-2*Math.log(unit()))*Math.cos(2*Math.PI*unit())/8);
  else if(kind==='exponential') v=-Math.log(unit())*U/8;
  else if(kind==='narrow') v=r()&65535;
  else if(kind==='duplicates_narrow') v=r()%8;
  else if(kind==='reverse') v=Math.floor((n-1-i)*U/n);
  else if(kind==='duplicates') v=(r()%8)*Math.floor((U-1)/7);
  else if(kind==='sorted'||kind==='partial') v=Math.floor(i*U/n);
  else v=r();
  a[i]=Math.min(U-1,Math.max(0,Math.floor(v)));
 }
 if(kind==='partial') for(let j=0;j<Math.floor(n*0.01);j++){let i=r()%n,k=r()%n;[a[i],a[k]]=[a[k],a[i]];}
 return a;
}
function bucket(a,bits){
 const B=1<<bits,shift=32-bits,counts=new Uint32Array(B),starts=new Uint32Array(B+1);
 for(let i=0;i<a.length;i++)counts[a[i]>>>shift]++;
 for(let b=0;b<B;b++)starts[b+1]=starts[b]+counts[b];
 const cursor=starts.slice(0,B),tmp=new Uint32Array(a.length);
 for(let i=0;i<a.length;i++){const v=a[i];tmp[cursor[v>>>shift]++]=v;}
 for(let b=0;b<B;b++)if(counts[b]>1)tmp.subarray(starts[b],starts[b+1]).sort();
 a.set(tmp);
}
// Fixed before measurement: sample 2048 positions, use top-byte entropy and adjacent order.
// H>=4 means >=16 effective buckets. Both adjacent directions must occur >10%.
function decide(a){
 const counts=new Uint32Array(256),r=rng(0x41c6ce57);let up=0,down=0;
 const m=Math.min(2048,a.length-1);
 for(let j=0;j<m;j++) {const i=r()%(a.length-1);counts[a[i]>>>24]++;if(a[i]<a[i+1])up++;else if(a[i]>a[i+1])down++;}
 let H=0;for(const c of counts)if(c){const p=c/m;H-=p*Math.log2(p);}
 return {apply:H>=4&&up/m>0.1&&down/m>0.1,entropy:H,up:up/m,down:down/m};
}
function median(x){return [...x].sort((a,b)=>a-b)[Math.floor(x.length/2)];}
function entropy(counts,total){let h=0;for(const c of counts)if(c){const p=c/total;h-=p*Math.log2(p);}return h;}
function sampleD(a){
 const m=Math.min(2048,a.length-1),r=rng(0x55ab76c1),freq=new Map(),h8=new Uint32Array(256),h12=new Uint32Array(4096);let up=0,down=0,maxFreq=0;
 for(let j=0;j<m;j++){
  const lo=Math.floor(j*(a.length-1)/m),hi=Math.floor((j+1)*(a.length-1)/m),i=lo+r()%Math.max(1,hi-lo),v=a[i];
  const c=(freq.get(v)||0)+1;freq.set(v,c);maxFreq=Math.max(maxFreq,c);h8[v>>>24]++;h12[v>>>20]++;if(v<a[i+1])up++;else if(v>a[i+1])down++;
 }
 const within8=new Float64Array(256);
 for(const [v,c]of freq){const b=v>>>24,p=c/h8[b];within8[b]-=p*Math.log2(p);}
 return {sample_size:m,unique_ratio:freq.size/m,duplicate_ratio:1-freq.size/m,mode_ratio:maxFreq/m,H8:entropy(h8,m),H12:entropy(h12,m),up:up/m,down:down/m,within_bucket_H8:Array.from(within8),bucket_sample_counts8:Array.from(h8)};
}
// Preregistered thresholds, no timing-based fitting: order first; duplicate>=50% or mode>=10%; high H8>=4.
function chooseD(s,guard){
 if(s.up<=0.1||s.down<=0.1)return 'native';
 const duplicates=s.duplicate_ratio>=0.5||s.mode_ratio>=0.1;
 if(duplicates)return guard&&s.H8<1?'native':'b8';
 return s.H8>=4?'b12':'native';
}
// Find highest differing bit by a full scan: all more significant bits are identical.
// Buckets on the next up-to-12 bits are therefore globally ordered. Cost included.
function active12(a){
 let diff=0;const first=a[0];for(let i=1;i<a.length;i++)diff|=a[i]^first;
 if(!diff)return;
 const shift=Math.max(0,32-Math.clz32(diff)-12),B=4096,counts=new Uint32Array(B),starts=new Uint32Array(B+1);
 for(let i=0;i<a.length;i++)counts[(a[i]>>>shift)&4095]++;
 for(let b=0;b<B;b++)starts[b+1]=starts[b]+counts[b];
 const cursor=starts.slice(0,B),tmp=new Uint32Array(a.length);
 for(let i=0;i<a.length;i++){const v=a[i];tmp[cursor[(v>>>shift)&4095]++]=v;}
 for(let b=0;b<B;b++)if(counts[b]>1)tmp.subarray(starts[b],starts[b+1]).sort();a.set(tmp);
}
function orderProbe(a){
 const m=Math.min(256,a.length-1),r=rng(0x12577abc);let up=0,down=0;
 for(let j=0;j<m;j++){const lo=Math.floor(j*(a.length-1)/m),hi=Math.floor((j+1)*(a.length-1)/m),i=lo+r()%Math.max(1,hi-lo);if(a[i]<a[i+1])up++;else if(a[i]>a[i+1])down++;}
 return up/m<=0.1||down/m<=0.1;
}

module.exports={make,rng,decide,sampleD,chooseD,orderProbe};
