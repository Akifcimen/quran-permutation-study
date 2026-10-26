"""Archive the unchanged original release and the complete rare-event extension."""
from pathlib import Path
import hashlib
import json
import zipfile

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / 'rare_event'
ORIGINAL = ROOT.parent / 'katman_19_yeniden_uretim.zip'
OUTPUT = ROOT.parent / 'katman_19_nadirlik_ek_calismasi.zip'
ALLOWED = {'.py', '.cpp', '.hpp', '.h', '.md', '.json', '.txt', '.csv',
           '.bin', '.png', '.svg', '.ipynb', '.html'}

def digest(data):
    return hashlib.sha256(data).hexdigest()

def main():
    assert ORIGINAL.is_file(), 'The immutable original release archive is required.'
    paths = sorted(p for p in BASE.rglob('*') if p.is_file()
                   and p.suffix in ALLOWED
                   and '__pycache__' not in p.parts
                   and 'rerun' not in p.relative_to(BASE).parts
                   and p.name != 'package_manifest.json')
    records = []
    for p in paths:
        data = p.read_bytes()
        assert not data.startswith(b'\x7fELF'), f'Unexpected compiled executable: {p}'
        records.append({'path': str(p.relative_to(ROOT)), 'bytes': len(data),
                        'sha256': digest(data)})
    manifest = {'scope': 'Rare-event extension; this manifest excludes itself.',
                'original_archive_sha256': digest(ORIGINAL.read_bytes()),
                'files': records}
    manifest_path = BASE / 'package_manifest.json'
    manifest_path.write_text(json.dumps(manifest, indent=2, ensure_ascii=False)+'\n')
    paths.append(manifest_path)
    entries = []
    with zipfile.ZipFile(ORIGINAL) as source, zipfile.ZipFile(
            OUTPUT, 'w', zipfile.ZIP_DEFLATED, compresslevel=9) as target:
        for info in source.infolist():
            data = source.read(info.filename)
            target.writestr(info.filename, data)
            entries.append({'path': info.filename, 'bytes': len(data),
                            'sha256': digest(data), 'source': 'unchanged original release'})
        for p in paths:
            name = 'joint_study/' + str(p.relative_to(ROOT))
            data = p.read_bytes()
            target.writestr(name, data)
            entries.append({'path': name, 'bytes': len(data), 'sha256': digest(data),
                            'source': 'rare-event extension'})
        target.writestr('archive_manifest.json', json.dumps(
            {'scope': 'All archive entries except this manifest', 'files': entries},
            indent=2, ensure_ascii=False)+'\n')
    with zipfile.ZipFile(OUTPUT) as archive:
        assert archive.testzip() is None
        assert len(archive.namelist()) == len(set(archive.namelist()))
        for record in entries:
            assert digest(archive.read(record['path'])) == record['sha256']
    print(json.dumps({'archive': str(OUTPUT), 'file_count': len(entries)+1,
                      'bytes': OUTPUT.stat().st_size,
                      'uncompressed_bytes': sum(r['bytes'] for r in entries),
                      'sha256': digest(OUTPUT.read_bytes()),
                      'all_entry_hashes_verified': True}, indent=2))

if __name__ == '__main__':
    main()
