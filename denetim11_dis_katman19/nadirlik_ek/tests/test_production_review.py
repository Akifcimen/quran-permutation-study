#!/usr/bin/env python3
"""Targeted tests of zero-inclusive replicate summaries and saved-state checks."""
import json
import math
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'rare_event/review'))
from production_review import ensure_checker,summarize,weighted_event

def main():
    gate={'minimum_nonzero_replicates':6,'maximum_empirical_relative_standard_error':0.35,'maximum_single_replicate_share':0.35}
    equal=summarize([1.0]*8,gate,8)
    assert equal['arithmetic_mean_including_zeros']==1
    assert equal['between_replicate_standard_error']==0
    assert equal['maximum_single_replicate_share']==1/8
    assert equal['passes_precision_gate']
    dominated=summarize([0.0]*7+[8.0],gate,8)
    assert dominated['arithmetic_mean_including_zeros']==1
    assert dominated['between_replicate_standard_error']==1
    assert dominated['empirical_relative_standard_error']==1
    assert dominated['maximum_single_replicate_share']==1
    assert dominated['nonzero_replicates']==1 and not dominated['passes_precision_gate']
    empty=summarize([0.0]*8,gate,8)
    assert empty['arithmetic_mean_including_zeros']==empty['between_replicate_standard_error']==0
    assert empty['empirical_relative_standard_error'] is None
    assert not empty['passes_precision_gate']
    partial=summarize([1.0,1.0],gate,8)
    assert not partial['gate_checks']['all_planned_replicates_complete']
    assert not partial['passes_precision_gate']
    known=summarize([0.0,1.0,2.0,3.0],gate,4)
    assert known['arithmetic_mean_including_zeros']==1.5
    assert math.isclose(known['between_replicate_standard_error'],math.sqrt(5/12),rel_tol=1e-15)
    assert math.isclose(weighted_event(math.log(.1),[0.,math.log(4)],4),.125,rel_tol=1e-15)
    assert weighted_event(-1000,[],4)==0
    assert math.isclose(weighted_event(-1000,[900],4),math.exp(-100)/4,rel_tol=1e-13)

    checker=ensure_checker()
    original=(ROOT/'rare_event/pilot/guided_block6_states0.bin').read_bytes()[:504]
    identity_bytes=114*4
    bad_duplicate=bytearray(original)
    bad_duplicate[4:8]=bad_duplicate[:4]
    bad_block=bytearray(original)
    bad_block[:4],bad_block[24:28]=bad_block[24:28],bad_block[:4]
    bad_score=bytearray(original)
    old=struct.unpack_from('=d',bad_score,identity_bytes)[0]
    struct.pack_into('=d',bad_score,identity_bytes,old+.125)
    bad_cases={'duplicate_identity':bad_duplicate,'out_of_block':bad_block,'changed_saved_score':bad_score,'truncated_record':original[:-1]}
    messages={}
    with tempfile.TemporaryDirectory(prefix='saved-state-review-check-') as directory:
        for name,content in bad_cases.items():
            path=Path(directory)/f'{name}.bin'
            path.write_bytes(content)
            process=subprocess.run([str(checker),str(ROOT),'block6',str(path)],capture_output=True,text=True)
            assert process.returncode!=0,f'Invalid saved state accepted: {name}'
            assert 'Saved-state review failed:' in process.stderr
            messages[name]=process.stderr.strip()
    result={'status':'passed','replicate_summary_examples':5,'inverse_gamma_examples':3,'invalid_state_cases_rejected':messages,
            'scope':'Deterministic aggregation and error detection; empirical pilot-state validation is recorded separately.'}
    (ROOT/'rare_event/tests/production_reviewer_validation.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))

if __name__=='__main__':
    main()
