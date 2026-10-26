#!/usr/bin/env python3
"""Compile and run the independent finite-state check of the production core."""
import json
import pathlib
import subprocess
import tempfile

root=pathlib.Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix='smc-core-validation-') as temporary:
    binary=pathlib.Path(temporary)/'test_smc_core'
    subprocess.run(['g++','-std=c++17','-O3',str(root/'tests/test_smc_core.cpp'),'-o',str(binary)],check=True)
    run=subprocess.run([str(binary)],check=True,capture_output=True,text=True)
    result=json.loads(run.stdout)
assert len(result)==6
assert sum(len(x['checks']) for x in result)==18
assert all(y['pass'] for x in result for y in x['checks'])
assert all(x['zero_estimates']>0 for x in result if x['moves']==0)
(root/'review/smc_core_validation.json').write_text(json.dumps(result,indent=2)+'\n')
print('18 finite-state SMC checks passed; all zero runs retained.')
