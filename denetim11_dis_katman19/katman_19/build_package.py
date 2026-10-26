"""Package the complete reproducible study, excluding caches and cloned git data."""
from pathlib import Path
import datetime
import hashlib
import json
import zipfile

ROOT = Path(__file__).resolve().parent
TOP_FILES = [
    'README.md','FROZEN_PROTOCOL.md','protocol.json','requirements.txt',
    'run_study.sh','analyze.py','observed_structure.py','build_notebook.py',
    'build_package.py','katman_19_raporu.md','katman_19_analizi.ipynb','katman_19_analizi.html',
]
DIRECTORIES = ['data','src','tests','semantic','results','review','figures']
SUFFIXES = {'.md','.json','.txt','.py','.cpp','.hpp','.h','.csv','.tsv','.bin','.npz','.png','.svg'}


def main():
    paths = [ROOT/name for name in TOP_FILES]
    for directory in DIRECTORIES:
        paths += [p for p in (ROOT/directory).rglob('*') if p.is_file() and p.suffix in SUFFIXES and '__pycache__' not in p.parts]
    paths += [ROOT/'sources'/name for name in ['repo.txt','readme.txt','original_quran_meta.json']]
    paths = sorted(set(paths))
    assert all(p.is_file() for p in paths)
    assert hashlib.sha256((ROOT/'protocol.json').read_bytes()).hexdigest() == 'e956d7b71df9185c1fa23a3733d27f05762a7567a06ee534070279c0b9be9768'
    assert hashlib.sha256((ROOT/'src/study_engine.cpp').read_bytes()).hexdigest() == '81cfc2ac3f40cd6d2859317832de2bd7671416320908737038a2a26c8b093cc2'
    manifest = {'created_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),
                'study':'joint-grammar-v1',
                'source_commit':'51fe1c0c36f2d481369480800aa84582e0a099e8',
                'file_count_excluding_manifest':len(paths),
                'files':[{'path':str(p.relative_to(ROOT)),'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in paths]}
    manifest_path=ROOT/'MANIFEST.json'
    manifest_path.write_text(json.dumps(manifest,indent=2)+'\n')
    paths.append(manifest_path)
    target=ROOT.parent/'katman_19_yeniden_uretim.zip'
    with zipfile.ZipFile(target,'w',compression=zipfile.ZIP_DEFLATED,compresslevel=9) as archive:
        for p in paths:
            archive.write(p,arcname='joint_study/'+str(p.relative_to(ROOT)))
    with zipfile.ZipFile(target) as archive:
        assert archive.testzip() is None
        for entry in manifest['files']:
            data=archive.read('joint_study/'+entry['path'])
            assert len(data)==entry['bytes']
            assert hashlib.sha256(data).hexdigest()==entry['sha256']
    print(json.dumps({'status':'verified','file_count':len(paths),'uncompressed_bytes':sum(p.stat().st_size for p in paths),'zip_bytes':target.stat().st_size,'zip_sha256':hashlib.sha256(target.read_bytes()).hexdigest(),'path':str(target)},indent=2))


if __name__=='__main__':
    main()
