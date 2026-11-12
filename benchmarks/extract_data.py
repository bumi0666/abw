import pathlib,csv,gzip,zipfile,io,array,json,hashlib,time,re,calendar
import numpy as np
R=pathlib.Path(__file__).resolve().parent;O=R/'inputs';O.mkdir(exist_ok=True)
manifest=[];t=time.time();u32=lambda:array.array('I')
def save(name,values,source,column,raw_rows,excluded,meaning):
 a=np.asarray(values,dtype=np.uint32);p=O/(name+'.bin');a.astype('<u4').tofile(p);ordered=np.sort(a);ordered.astype('<u4').tofile(O/(name+'.expected.bin'));k=len(np.unique(a));record=dict(name=name,group='real',source=source,column=column,meaning=meaning,n=len(a),unique=k,min=int(a.min()),max=int(a.max()),duplicate_fraction=1-k/len(a),raw_rows=raw_rows,excluded_rows=excluded,bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest(),expected_sha256=hashlib.sha256((O/(name+'.expected.bin')).read_bytes()).hexdigest(),replicated=False,order='source file order, no shuffle',counting_eligible=(int(a.max())-int(a.min())+1)*8<=67108864,cafs_i32_eligible=int(a.max())<=2147483647)
 manifest.append(record);print(record,flush=True)
status,stamp,byte=u32(),u32(),u32();raw=0;bad_s=bad_t=bad_b=0
pat=re.compile(rb'"\s+(\d{3})\s+(\d+|-)\s*$');date=re.compile(rb'\[(\d{2})/Jul/1995:(\d{2}):(\d{2}):(\d{2}) -0400\]');epoch=calendar.timegm((1995,7,1,0,0,0))+14400
with gzip.open(R/'downloads/NASA_access_log_Jul95.gz','rb') as f:
 for line in f:
  raw+=1;m=pat.search(line);d=date.search(line)
  if m:
   status.append(int(m[1]))
   if m[2]!=b'-':
    v=int(m[2]);assert 0<=v<=0xffffffff;byte.append(v)
   else:bad_b+=1
  else:bad_s+=1;bad_b+=1
  if d:
   day,h,mi,s=map(int,d.groups());stamp.append(epoch+(day-1)*86400+h*3600+mi*60+s)
  else:bad_t+=1
save('nasa_status',status,'NASA_July1995','HTTP response status',raw,bad_s,'Original numeric HTTP response code, all parseable records')
save('nasa_timestamp',stamp,'NASA_July1995','request timestamp',raw,bad_t,'Exact Unix seconds from published timezone -0400, 1-second source resolution')
save('nasa_bytes',byte,'NASA_July1995','response bytes',raw,bad_b,'Original response byte count; missing dash omitted, no imputation')
with zipfile.ZipFile(R/'downloads/covertype.zip') as z:
 cols=[u32(),u32(),u32()];raw=0
 with gzip.GzipFile(fileobj=io.BytesIO(z.read('covtype.data.gz'))) as f:
  for row in csv.reader(io.TextIOWrapper(f)):
   raw+=1
   for a,i in zip(cols,[54,0,5]):v=int(row[i]);assert 0<=v<=0xffffffff;a.append(v)
 for name,a,col,meaning in zip(['covtype_cover','covtype_elevation','covtype_road_distance'],cols,[54,0,5],['USFS forest cover label 1..7','Elevation in metres','Horizontal distance to roadways in metres']):save(name,a,'UCI_Covertype',col,raw,0,meaning)
with zipfile.ZipFile(R/'downloads/us_census_1990.zip') as z:
 cols=[u32(),u32(),u32()];raw=0
 with z.open('USCensus1990.data.txt') as b:
  reader=csv.reader(io.TextIOWrapper(b));header=next(reader);idx=[header.index(c) for c in ['caseid','dIndustry','iSex']]
  for row in reader:
   raw+=1
   for a,i in zip(cols,idx):v=int(row[i]);assert 0<=v<=0xffffffff;a.append(v)
 for name,a,col,meaning in zip(['census_caseid','census_industry','census_sex'],cols,['caseid','dIndustry','iSex'],['Publisher case ID, not a person identifier for inference','Publisher coarsened industry code','Publisher sex category code']):save(name,a,'UCI_Census1990_processed',col,raw,0,meaning)
sourcefiles={p.name:dict(bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in (R/'downloads').iterdir()}
(R/'input_manifest.json').write_text(json.dumps(dict(real=manifest,source_files=sourcefiles,ingest_seconds=time.time()-t,transformation_time_excluded_from_sort=True),ensure_ascii=False,indent=2),encoding='utf8')
print('INGEST_SECONDS',time.time()-t,flush=True)
