#!/usr/bin/env python3
"""Independent reconstruction of the fixed uniform-anchor SIS production.

This file does not run new sampling or pool pilot draws into production.
"""
from fractions import Fraction
from hashlib import sha256
import json
import math
from pathlib import Path
import struct
import subprocess

from scipy.stats import chi2, norm, t

from production_review import ensure_checker
from review_seed_streams import worker_seed, GAMMA, INV_GAMMA, MOD, MASK

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
DATA = ROOT / 'rare_event/uniform_anchor'


def digest(path):
    return sha256(path.read_bytes()).hexdigest()


def close(a, b, label):
    assert math.isclose(a, b, rel_tol=3e-13, abs_tol=0), (label, a, b)


def remove_swap(values, value):
    index = values.index(value)
    values[index] = values[-1]
    values.pop()


def multiply(a, b):
    return tuple(sum(a[2*i+k] * b[2*k+j] for k in range(2)) % 19
                 for i in range(2) for j in range(2))


def power(a, exponent):
    answer = (1, 0, 0, 1)
    while exponent:
        if exponent & 1:
            answer = multiply(answer, a)
        a = multiply(a, a)
        exponent >>= 1
    return answer


def maxorder(a):
    identity = (1, 0, 0, 1)
    return power(a, 360) == identity and all(power(a, 360//p) != identity for p in (2, 3, 5))


def review_witness(draw, base, matching_count, patterns):
    ids = draw['ids']
    assert sorted(ids) == list(range(114))
    values = [base[i] for i in ids]
    F = [n for n, a in enumerate(values, 1) if (n+a) % 19 == 0]
    H = [n for n, a in enumerate(values, 1) if (n-a) % 19 == 0]
    E = [n for n, a in enumerate(values, 1) if (n+a) % 2 == 0]
    assert len(F) == 12 and len(H) == 7 and not set(F).intersection(H)
    assert sum(values[n-1] for n in F+H) % 19 == 0
    cells = [[] for _ in range(4)]
    for n in F:
        q = (n+values[n-1])//19
        cells[2*(q % 2)+n % 2].append(q)
    assert [sorted(c) for c in cells] == patterns[draw['pattern']]
    assert len(E) == 57 and sum(n+values[n-1] for n in E) == sum(values)
    en, ea = sum(E), sum(values[n-1] for n in E)
    fn, fa = sum(F), sum(values[n-1] for n in F)
    P = tuple(x % 19 for x in (en, ea, 6555-en, sum(values)-ea))
    R = tuple(x % 19 for x in (fn, fa, 6555-fn, sum(values)-fa))
    PR, RP = multiply(P, R), multiply(R, P)
    anti = tuple((a+b) % 19 for a, b in zip(PR, RP))
    comm = tuple((a-b) % 19 for a, b in zip(PR, RP))
    assert all(maxorder(m) for m in (P, R, anti))
    assert multiply(comm, comm) == (1, 0, 0, 1)

    # Reconstruct the recorded ordered trace, without RNG or sampler code.
    rows, cols = [[] for _ in range(19)], [[] for _ in range(19)]
    used = {ids[n-1] for n in F}
    for n in range(1, 115):
        if n not in F:
            rows[n % 19].append(n)
    for item in range(114):
        if item not in used:
            cols[base[item] % 19].append(item)
    h_order = draw['H_rows_in_sampling_order']
    assert sorted(h_order) == sorted(H)
    product, h_sum = 1, 0
    for step, n in enumerate(h_order):
        item, residue = ids[n-1], n % 19
        assert residue != 0 and base[item] % 19 == residue
        if step < 6:
            available = sum(len(rows[r])*len(cols[r]) for r in range(1, 19))
        else:
            required = (sum(F)-h_sum) % 19
            assert required == residue
            available = len(rows[required])*len(cols[required])
        assert available > 0
        product *= available
        remove_swap(rows[residue], n)
        remove_swap(cols[residue], item)
        h_sum += n
    row_colors, col_colors = [[] for _ in range(10)], [[] for _ in range(10)]
    for residue in range(19):
        group = min(residue, 19-residue)
        row_colors[group].extend(rows[residue])
        col_colors[group].extend(cols[residue])
    for remaining in range(95, 0, -1):
        group = max((g for g in range(10) if row_colors[g]),
                    key=lambda g: (len(row_colors[g])+len(col_colors[g]), -g))
        assert all(len(row_colors[g])+len(col_colors[g]) <= remaining for g in range(10))
        n = row_colors[group].pop()
        item = ids[n-1]
        color = min(base[item] % 19, 19-base[item] % 19)
        assert color != group and item in col_colors[color]
        product *= remaining-len(col_colors[group])
        remove_swap(col_colors[color], item)
    exact_weight = Fraction(matching_count*product, math.factorial(114)*math.factorial(7))
    close(draw['weight'], float(exact_weight), 'independent exact trace weight')
    return {'pattern': draw['pattern'], 'weight': draw['weight'],
            'trace_weight_reconstructed_with_7_factorial': True,
            'direct_natural_predicates_and_matrix_powers_passed': True}


def main():
    plan_path = DATA/'production_plan.json'
    plan = json.loads(plan_path.read_text())
    assert plan['n_total'] == 100000000 and plan['score_mode'] == 1
    source_checks = {name: digest(ROOT/name) == expected for name, expected in plan['source_sha256'].items()}
    assert all(source_checks.values()), source_checks
    base = list(map(int, (ROOT/'data/counts.txt').read_text().split()))
    base[8] = 127
    matching = json.loads((DATA/'F_matching_count_127.json').read_text())
    matching_count = int(matching['F_partial_matching_total'])
    records, workers, streams, witnesses = [], [], [], []
    binary_records = []
    for batch in plan['batches']:
        record = json.loads((DATA/batch['output']).read_text())
        assert record['n'] == batch['n'] and record['seed'] == batch['seed']
        assert record['threads'] == batch['threads'] == len(record['workers']) == 8
        assert record['method'] == 'sis' and record['variant_anchor'] == 127 and record['score_mode'] == 1
        assert record['successes']+sum(record['H_dead_prefix_counts']) == record['n']
        assert sum(record['pattern_counts']) == record['n']
        assert sum(w['processed'] for w in record['workers']) == record['n']
        assert record['F_partial_matching_count'] == matching_count
        close(record['F_marked_rejection_factor'],
              float(Fraction(matching_count*math.factorial(102), math.factorial(114))),
              'independent F rejection factor')
        for k, metric in enumerate(record['metrics']):
            for key in ('hits', 'weight_sum', 'weight_square_sum'):
                close(metric[key], math.fsum(w['metrics'][k][key] for w in record['workers']), 'worker aggregate '+key)
                close(metric[key], math.fsum(p['metrics'][k][key] for p in record['by_pattern']), 'bag aggregate '+key)
            close(metric['max_weight'], max(w['metrics'][k]['max_weight'] for w in record['workers']), 'worker max')
            close(metric['max_weight'], max(p['metrics'][k]['max_weight'] for p in record['by_pattern']), 'bag max')
            assert 0 <= metric['hits'] <= record['successes'] <= record['n']
        for i, worker in enumerate(record['workers']):
            assert worker['processed'] == 6250000
            assert sum(worker['pattern_counts']) == worker['processed']
            assert worker['seed'] == worker_seed(batch['seed'], i)
            assert (worker['seed']+GAMMA*worker['rng_calls']) & MASK == worker['final_rng_state']
            streams.append({'batch': batch['batch'], 'worker': i, 'seed': worker['seed'],
                            'calls': worker['rng_calls'], 'position': worker['seed']*INV_GAMMA % MOD})
            if worker['metrics'][2]['hits']:
                draw = worker['max_target_draw']
                close(draw['weight'], worker['metrics'][2]['max_weight'], 'maximum target witness')
                witnesses.append(review_witness(draw, base, matching_count, matching['patterns']))
                binary_records.append(struct.pack('<114i6d', *draw['ids'],
                                                 *(p['value'] for p in draw['scores']['policies'])))
            workers.append(worker)
        records.append(record)
    assert len(workers) == 16
    N = sum(r['n'] for r in records)
    assert N == plan['n_total']
    metric_summary = {}
    for k, label in enumerate(m['name'] for m in records[0]['metrics']):
        S = math.fsum(r['metrics'][k]['weight_sum'] for r in records)
        Q = math.fsum(r['metrics'][k]['weight_square_sum'] for r in records)
        mean = S/N
        se = math.sqrt(max(0, Q-S*S/N)/(N*(N-1)))
        means = [w['metrics'][k]['weight_sum']/w['processed'] for w in workers]
        close(mean, math.fsum(means)/16, 'worker mean equality')
        worker_se = math.sqrt(math.fsum((x-mean)**2 for x in means)/(16*15))
        zcrit, tcrit = float(norm.ppf(.975)), float(t.ppf(.975, 15))
        maximum = max(r['metrics'][k]['max_weight'] for r in records)
        metric_summary[label] = {
            'hits': sum(r['metrics'][k]['hits'] for r in records),
            'probability': mean, 'SE': se,
            'CI95_weighted_normal': [max(0, mean-zcrit*se), mean+zcrit*se],
            'worker_SE': worker_se,
            'CI95_worker_t': [max(0, mean-tcrit*worker_se), mean+tcrit*worker_se],
            'ESS': S*S/Q if Q else 0,
            'RSE': se/mean if mean else None,
            'max_draw_share': maximum/S if S else None,
            'nonzero_workers': sum(x > 0 for x in means),
            'worker_probabilities': means,
            'all_proposals_in_denominator': N,
        }
    primary = metric_summary['G127_Mview0']
    published = json.loads((DATA/'final_summary.json').read_text())
    assert published['n'] == N
    mapped_fields = {
        'hits': 'raw_hits', 'probability': 'estimate', 'SE': 'SE',
        'worker_SE': 'worker_batch_SE', 'ESS': 'target_ESS',
        'RSE': 'relative_standard_error',
        'max_draw_share': 'largest_draw_contribution_fraction',
        'nonzero_workers': 'nonzero_worker_batches',
    }
    for name, metric in metric_summary.items():
        other = published['metrics'][name]
        for independent, reported in mapped_fields.items():
            if metric[independent] is None:
                assert other[reported] is None
            else:
                close(metric[independent], other[reported], 'published '+name+' '+reported)
        for independent, reported in [('CI95_weighted_normal', 'normal_approx_95_interval'),
                                      ('CI95_worker_t', 'worker_batch_t_approx_95_interval')]:
            for a, b in zip(metric[independent], other[reported]):
                close(a, b, 'published '+name+' '+reported)
    gate = plan['precision_diagnostics_predeclared']
    gate_checks = {
        'target_ESS': primary['ESS'] >= gate['primary_target_ESS_min'],
        'relative_standard_error': primary['RSE'] is not None and primary['RSE'] <= gate['primary_relative_standard_error_max'],
        'largest_draw_contribution': primary['max_draw_share'] is not None and primary['max_draw_share'] <= gate['primary_largest_draw_contribution_max'],
        'nonzero_workers': primary['nonzero_workers'] >= gate['nonzero_worker_batches_min'],
    }
    ordered = sorted(streams, key=lambda x: x['position'])
    assert len({x['seed'] for x in ordered}) == 16
    for i, stream in enumerate(ordered):
        following = ordered[(i+1) % len(ordered)]
        gap = (following['position']-stream['position']) % MOD
        assert stream['calls'] < gap
        stream['forward_gap'] = gap
        stream['margin'] = gap-stream['calls']
    pilot_seed_overlap = []
    for path in list(DATA.glob('pilot*.json'))+[DATA/'instrumentation_replay.json']:
        record = json.loads(path.read_text())
        for worker in record.get('workers', []):
            if worker['seed'] in {s['seed'] for s in streams}:
                pilot_seed_overlap.append(str(path.relative_to(ROOT)))
    assert not pilot_seed_overlap
    pattern_counts = [sum(r['pattern_counts'][p] for r in records) for p in range(20)]
    expected_counts = [N*int(c)/matching_count for c in matching['F_partial_matching_counts_by_pattern']]
    chi_square = sum((o-e)**2/e for o, e in zip(pattern_counts, expected_counts))
    witness_path = HERE/'uniform_production_max_target_witnesses.bin'
    witness_path.write_bytes(b''.join(binary_records))
    checker = ensure_checker()
    checked = json.loads(subprocess.run([str(checker), str(ROOT), 'uniform', str(witness_path)],
                                        check=True, capture_output=True, text=True).stdout)
    assert checked['status'] == 'passed'
    assert checked['states'] == len(witnesses)
    for name, expected in plan['source_sha256'].items():
        assert digest(ROOT/name) == expected
    result = {
        'status': 'passed', 'scope': 'independent uniform natural127-anchor production audit',
        'production_plan_sha256': digest(plan_path), 'n': N,
        'source_hashes_all_match': source_checks,
        'all_worker_and_bag_aggregates_reconciled': True,
        'all_published_metric_estimates_diagnostics_and_intervals_reproduced': True,
        'published_summary_sha256': digest(DATA/'final_summary.json'),
        'deadends_retained_as_zero': N-sum(r['successes'] for r in records),
        'metrics': metric_summary, 'precision_gate_checks': gate_checks,
        'passes_predeclared_precision_diagnostics': all(gate_checks.values()),
        'all_16_recorded_RNG_final_states_match_exact_calls': True,
        'production_counter_windows_pairwise_disjoint': True,
        'RNG_streams': ordered,
        'pilot_worker_start_collisions': pilot_seed_overlap,
        'RNG_interpretation': 'Recorded 16 production counter windows are pairwise disjoint. Pilot start-state collisions were excluded; pilots with unrecorded lengths are not given fabricated exact windows. PRNG window disjointness does not prove mathematical independence.',
        'pattern_sampling_diagnostic': {'chi_square': chi_square, 'degrees_of_freedom': 19,
                                        'p_value': float(chi2.sf(chi_square, 19))},
        'max_target_witnesses': witnesses,
        'max_target_witness_scores_compared_bit_exactly': checked['score_values_compared_bit_exactly'],
        'interval_interpretation': 'Weighted normal and 16-worker Student-t intervals are asymptotic Monte Carlo diagnostics, not exact binomial intervals. Expected waiting 1/p is a parameter, not a prediction of actual waiting time.',
        'tail_scope': 'Tail metrics are CW127 intersect natural Geither intersect frozen full-score threshold; they are not the full unrestricted score-tail probability.',
        'input_hashes': {batch['output']: digest(DATA/batch['output']) for batch in plan['batches']},
    }
    path = HERE/'uniform_production_review.json'
    path.write_text(json.dumps(result, indent=2, allow_nan=False)+'\n')
    print(json.dumps({'status': result['status'], 'n': N, 'primary': primary,
                      'gate_checks': gate_checks, 'witnesses': len(witnesses)}, indent=2))


if __name__ == '__main__':
    main()
