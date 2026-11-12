import pathlib,json,urllib.request,hashlib
R=pathlib.Path(__file__).resolve().parent;downloads=R/'downloads';downloads.mkdir(exist_ok=True);vendor=R/'vendor';vendor.mkdir(exist_ok=True)
sources=json.loads((R/'input_manifest.json').read_text())['source_files'];targets={'NASA_access_log_Jul95.gz':'https://ita.ee.lbl.gov/traces/NASA_access_log_Jul95.gz','covertype.zip':'https://archive.ics.uci.edu/static/public/31/covertype.zip','us_census_1990.zip':'https://archive.ics.uci.edu/static/public/116/us+census+data+1990.zip'}
def fetch(url,path,expected=None):
 if not path.exists():
  with urllib.request.urlopen(url,timeout=120) as src,path.open('wb') as out:
   while True:
    block=src.read(1048576)
    if not block:break
    out.write(block)
 actual=hashlib.sha256(path.read_bytes()).hexdigest()
 if expected and actual!=expected:raise RuntimeError('Hash mismatch: '+str(path))
 print(path.name,path.stat().st_size,actual)
for name,url in targets.items():fetch(url,downloads/name,sources[name]['sha256'])
commit='13488afef446ae88e7ad631ec6f862def896094a';lock=json.loads((R/'results/provenance.json').read_text())['measured_source_hashes']
for name in ['cafs.hpp','cafs2.hpp']:fetch('https://raw.githubusercontent.com/kexibq-official/cafs-lib/'+commit+'/include/'+name,vendor/name,lock['vendor/'+name])
fetch('https://raw.githubusercontent.com/kexibq-official/cafs-lib/'+commit+'/LICENSE',vendor/'CAFS_LICENSE')
