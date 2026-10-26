#!/usr/bin/env python3
"""Analyze the fixed 100m uniform-127 anchor SIS budget, retaining all zeros.

The importance contributions are not Bernoulli observations. Both intervals
below are approximate Monte Carlo intervals, never exact binomial intervals.
"""
from fractions import Fraction
from hashlib import sha256
import json
import math
from pathlib import Path
from statistics import mean, stdev

from scipy.stats import beta, chisquare, norm, t

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
read = lambda name: json.loads((HERE / name).read_text())
Z = float(norm.ppf(0.975))


def summarize_metric(m, n):
    total, square = float(m['weight_sum']), float(m['weight_square_sum'])
    estimate = total / n
    variance = max(0., (square - total * total / n) / (n * (n - 1)))
    se = math.sqrt(variance)
    ci = [max(0., estimate - Z * se), estimate + Z * se]
    return {
        'raw_hits': m['hits'], 'estimate': estimate, 'SE': se,
        'variance_of_mean': variance,
        'relative_standard_error': se / estimate if estimate else None,
        'normal_approx_95_interval': ci,
        'target_ESS': total * total / square if square else 0.,
        'largest_draw_contribution_fraction': float(m['max_weight']) / total if total else None,
        'weight_sum': total, 'weight_square_sum': square,
        'max_weight': m['max_weight'],
        'expected_waiting_point': 1 / estimate if estimate else None,
        'expected_waiting_normal_approx_95_interval':
            [1 / ci[1], 1 / ci[0]] if ci[0] else None,
    }


def combine_metrics(metrics):
    return {
        'name': metrics[0]['name'],
        'hits': sum(m['hits'] for m in metrics),
        'weight_sum': math.fsum(float(m['weight_sum']) for m in metrics),
        'weight_square_sum': math.fsum(float(m['weight_square_sum']) for m in metrics),
        'max_weight': max(float(m['max_weight']) for m in metrics),
    }


def check_aggregation(record):
    workers, bags = record['workers'], record['by_pattern']
    assert record['n'] == sum(w['processed'] for w in workers)
    assert record['n'] == sum(b['n'] for b in bags)
    assert record['n'] == sum(record['pattern_counts'])
    assert record['successes'] == sum(w['success'] for w in workers)
    assert record['successes'] + sum(record['H_dead_prefix_counts']) == record['n']
    for i, m in enumerate(record['metrics']):
        for rows in (workers, bags):
            reconstructed = combine_metrics([x['metrics'][i] for x in rows])
            assert reconstructed['hits'] == m['hits']
            for key in ('weight_sum', 'weight_square_sum', 'max_weight'):
                assert math.isclose(reconstructed[key], float(m[key]), rel_tol=2e-13)
    return True


def stream_windows(records):
    period, increment = 1 << 64, 0x9e3779b97f4a7c15
    inverse = pow(increment, -1, period)
    intervals, windows = [], []
    for run in records:
        for i, worker in enumerate(run['workers']):
            seed, calls = worker['seed'], worker['rng_calls']
            assert worker['final_rng_state'] == (seed + increment * calls) % period
            start = (seed * inverse + 1) % period
            end = start + calls
            label = f"seed{run['seed']}_worker{i}"
            windows.append({'label': label, 'initial_state': seed, 'calls': calls,
                            'first_consumed_counter': start, 'final_state': worker['final_rng_state']})
            if end <= period:
                intervals.append((start, end, label))
            else:
                intervals.extend(((start, period, label), (0, end - period, label)))
    intervals.sort()
    for a, b in zip(intervals, intervals[1:]):
        assert a[1] <= b[0], ('RNG windows overlap', a, b)
    positions = sorted(w['first_consumed_counter'] for w in windows)
    gap = min([b - a for a, b in zip(positions, positions[1:])] + [period - positions[-1] + positions[0]])
    return {'status': 'passed', 'scope': 'all 16 production worker streams; actual consumed counter windows',
            'stream_count': len(windows), 'max_calls': max(w['calls'] for w in windows),
            'minimum_circular_start_distance': gap, 'windows': windows}


def main():
    plan = read('production_plan.json')
    runs = [read(p['output']) for p in plan['batches']]
    for r, p in zip(runs, plan['batches']):
        assert r['n'] == p['n'] and r['seed'] == p['seed'] and r['threads'] == p['threads']
        assert r['phase'] == 'fixed_production' and r['score_mode'] == 1
        assert r['variant_anchor'] == 127
        check_aggregation(r)
    source_checks = {f: sha256((ROOT / f).read_bytes()).hexdigest() == h
                     for f, h in plan['source_sha256'].items()}
    assert all(source_checks.values())
    n = sum(r['n'] for r in runs)
    assert n == plan['n_total'] == 100_000_000
    workers = [w for r in runs for w in r['workers']]
    assert len(workers) == 16 and len(set(w['processed'] for w in workers)) == 1

    metrics = {}
    for i, m in enumerate(runs[0]['metrics']):
        result = summarize_metric(combine_metrics([r['metrics'][i] for r in runs]), n)
        worker_means = [float(w['metrics'][i]['weight_sum']) / w['processed'] for w in workers]
        assert math.isclose(mean(worker_means), result['estimate'], rel_tol=2e-14)
        worker_se = stdev(worker_means) / math.sqrt(len(workers))
        radius = float(t.ppf(.975, len(workers) - 1)) * worker_se
        worker_ci = [max(0., result['estimate'] - radius), result['estimate'] + radius]
        result['worker_batch_t_approx_95_interval'] = worker_ci
        result['worker_batch_SE'] = worker_se
        result['worker_batch_means'] = worker_means
        result['worker_batch_raw_hits'] = [w['metrics'][i]['hits'] for w in workers]
        result['nonzero_worker_batches'] = sum(v > 0 for v in worker_means)
        result['largest_worker_contribution_fraction'] = max(worker_means) / sum(worker_means) if sum(worker_means) else None
        result['worker_batch_SE_to_iid_SE_ratio'] = worker_se / result['SE'] if result['SE'] else None
        batch_results = [summarize_metric(r['metrics'][i], r['n']) for r in runs]
        denominator = math.sqrt(sum(r['variance_of_mean'] for r in batch_results))
        result['two_batch_standardized_difference'] = ((batch_results[0]['estimate'] - batch_results[1]['estimate']) / denominator
                                                       if denominator else None)
        result['batches'] = [{k: b[k] for k in ('raw_hits', 'estimate', 'SE', 'target_ESS', 'largest_draw_contribution_fraction')}
                             for b in batch_results]
        metrics[m['name']] = result

    f = read('F_matching_count_127.json')
    mf = int(f['F_partial_matching_total'])
    pattern_counts = [sum(r['pattern_counts'][i] for r in runs) for i in range(20)]
    expected = [n * int(c) / mf for c in f['F_partial_matching_counts_by_pattern']]
    chi = chisquare(pattern_counts, expected)
    by_pattern = []
    for i in range(20):
        by_pattern.append({'pattern': i, 'bag': f['patterns'][i], 'proposal_count': pattern_counts[i],
                           'proposal_probability_exact': str(Fraction(int(f['F_partial_matching_counts_by_pattern'][i]), mf)),
                           'metrics': {m['name']: summarize_metric(combine_metrics([r['by_pattern'][i]['metrics'][j] for r in runs]), n)
                                       for j, m in enumerate(runs[0]['metrics'])}})
    # The independently designed marked-F rejection run is a pilot validation;
    # it is not pooled with SIS production or used to choose a stopping time.
    rejection = read('pilot_rejection.json')
    factor = Fraction(mf, math.prod(range(103, 115)))
    h, nr = rejection['successes'], rejection['n']
    rejection_ci = [float(factor) * float(beta.ppf(.025, h, nr-h+1)),
                    float(factor) * float(beta.ppf(.975, h+1, nr-h))]
    rejection_estimate = float(factor) * h / nr
    rejection_se = float(factor) * math.sqrt((h / nr) * (1 - h / nr) / nr)
    anchor = metrics['anchor_C15_W_127']
    primary = metrics['G127_Mview0']
    conditional_steps = []
    for numerator_name, denominator_name in [('G127', 'anchor_C15_W_127'), ('G127_Mview0', 'G127')]:
        numerator, denominator = metrics[numerator_name], metrics[denominator_name]
        ratio = numerator['weight_sum'] / denominator['weight_sum']
        # Nested indicators imply cross-product sum equals numerator square sum.
        residual_squares = ((1 - 2 * ratio) * numerator['weight_square_sum']
                            + ratio * ratio * denominator['weight_square_sum'])
        variance = n * residual_squares / ((n - 1) * denominator['weight_sum']**2)
        se = math.sqrt(max(0., variance))
        ci = [max(0., ratio - Z * se), min(1., ratio + Z * se)]
        conditional_steps.append({'numerator': numerator_name, 'denominator': denominator_name,
                                  'conditional_probability': ratio, 'delta_method_SE': se,
                                  'delta_method_approx_95_interval': ci,
                                  'inverse_conditional_probability': 1 / ratio,
                                  'scope': 'Weighted nested-event ratio; covariance retained, approximate delta-method uncertainty'})
    gates = plan['precision_diagnostics_predeclared']
    precision = {
        'target_ESS': primary['target_ESS'] >= gates['primary_target_ESS_min'],
        'relative_standard_error': primary['relative_standard_error'] is not None and primary['relative_standard_error'] <= gates['primary_relative_standard_error_max'],
        'largest_draw_contribution': primary['largest_draw_contribution_fraction'] is not None and primary['largest_draw_contribution_fraction'] <= gates['primary_largest_draw_contribution_max'],
        'nonzero_worker_batches': primary['nonzero_worker_batches'] >= gates['nonzero_worker_batches_min'],
    }
    output = {
        'scope': 'Uniform labeled permutation null; fixed natural form(1,1), prime19, variant127, natural matrix view0. The full frozen score tail is a distinct larger event.',
        'n': n, 'independent_worker_batches': len(workers),
        'all_proposals_in_denominator': True, 'pilots_excluded': True,
        'primary_event': 'C15 & W3 & G3 & M15(view0), natural form/19, variant127',
        'primary_metric': 'G127_Mview0', 'primary': primary,
        'metrics': metrics,
        'conditional_layer_steps': conditional_steps,
        'full_score_component_scope': 'C15W3(127) intersect (G3 natural127 or G3 natural129) intersect frozen full score tail. No completeness claim outside this intersection.',
        'weight_formula': 'M_F * product(H available edge counts) * product(completion available label counts) / (114! * 7!)',
        'F_partial_matching_count_exact': str(mf),
        'precision_checks': precision, 'passes_predeclared_precision_checks': all(precision.values()),
        'uncertainty': 'Reported 95% intervals are approximate Monte Carlo intervals from iid importance-weight moments and a 16-worker Student t check. They are NOT exact binomial intervals or bounds on unenumerated hypothesis-selection bias. Waiting intervals quantify uncertainty in the expected waiting parameter 1/p, not realized waiting times.',
        'bag_support_check': {'chi2': float(chi.statistic), 'df': 19, 'p': float(chi.pvalue), 'all20bags_observed': all(pattern_counts), 'counts': pattern_counts},
        'independent_anchor_comparison': {'rejection_phase': 'pilot comparator, excluded from production estimate',
            'rejection_n': nr, 'rejection_hits': h, 'marked_factor_exact': str(factor),
            'rejection_estimate': rejection_estimate, 'scaled_exact_CP95': rejection_ci,
            'SIS_estimate_inside_rejection_CP95': rejection_ci[0] <= anchor['estimate'] <= rejection_ci[1],
            'standardized_difference': (anchor['estimate'] - rejection_estimate) / math.sqrt(anchor['SE']**2 + rejection_se**2)},
        'source_hash_checks': source_checks,
        'direct_feature_validations': sum(r['validations'] for r in runs),
        'max_direct_product_vs_log_weight_error': max(r['max_direct_product_vs_log_weight_error'] for r in runs),
        'full_score_evaluations': sum(r['full_score_evaluations'] for r in runs),
        'completed_draws': sum(r['successes'] for r in runs),
        'H_deadend_counts': [sum(r['H_dead_prefix_counts'][i] for r in runs) for i in range(8)],
        'seconds_by_batch': [r['seconds'] for r in runs],
        'rng_window_audit': stream_windows(runs),
        'by_pattern': by_pattern,
        'input_sha256': {p['output']: sha256((HERE / p['output']).read_bytes()).hexdigest() for p in plan['batches']},
    }
    (HERE / 'final_summary.json').write_text(json.dumps(output, indent=2) + '\n')
    rows = []
    for name in ['anchor_C15_W_127', 'G127', 'G127_Mview0', 'G127_Manyview', 'tail_mixture']:
        m = metrics[name]
        rows.append(f"| {name} | {m['raw_hits']} | {m['estimate']:.12g} | [{m['normal_approx_95_interval'][0]:.12g}, {m['normal_approx_95_interval'][1]:.12g}] | {m['target_ESS']:.3f} |")
    ci = primary['normal_approx_95_interval']
    waiting = primary['expected_waiting_normal_approx_95_interval']
    report = f'''# Uniform natural-19 anchor: fixed production result

This calculation targets the **fixed natural form `(1,1)`, prime 19, variant 127,
and natural matrix view** under uniform labeled permutations of all 114 IDs.
It does not estimate the entire frozen score tail.

The predetermined production budget was **{n:,} proposals** in two independently
seeded 50-million runs and 16 worker streams. Every proposal is retained in the
denominator, including H deadends. All pilots are excluded.

## Primary result

For `C15 & W3 & G3 & M15(view0)`, the estimate is
**p = {primary['estimate']:.12g}**, with approximate weighted-moment 95% Monte Carlo
interval **[{ci[0]:.12g}, {ci[1]:.12g}]**.

The expected waiting parameter is **1/p = {primary['expected_waiting_point']:.12g}**,
with inverse-endpoint interval **[{waiting[0]:.12g}, {waiting[1]:.12g}]**.
This is uncertainty in the expected waiting parameter, not a prediction interval
for an actual geometrically distributed waiting time.

There were {primary['raw_hits']} positive importance contributions, target ESS
{primary['target_ESS']:.3f}, relative standard error
{primary['relative_standard_error']:.5%}, largest individual contribution
{primary['largest_draw_contribution_fraction']:.5%}, and
{primary['nonzero_worker_batches']}/16 positive worker batches.
The predeclared precision diagnostics all passed: **{all(precision.values())}**.

The independent-worker Student t check gives
{primary['worker_batch_t_approx_95_interval']}; its standard error is
{primary['worker_batch_SE_to_iid_SE_ratio']:.4f} times the iid weighted-moment SE.
The two 50-million estimates differ by
{primary['two_batch_standardized_difference']:.4f} estimated standard errors.
Both interval methods are approximations. **Neither is a Clopper–Pearson interval.**

## Event-specific estimates

All these are unconditional probabilities under the uniform permutation null,
even though the efficient proposal concentrates on the C/W anchor.

| Event | Positive contributions | Probability | Approximate 95% MC interval | Target ESS |
|---|---:|---:|---|---:|
{chr(10).join(rows)}

`tail_mixture` is only the intersection of the C15/W3 variant-127 anchor, natural
G3 in either variant, and the complete frozen score exceeding its observed
threshold. It is a component of the full tail. Equal point estimates for this
component and the natural event, if observed, are not an equivalence proof.

## Proposal and checks

Exactly **{mf}** eligible labeled F partial matchings were counted with integer
dynamic programming. The 20 coefficient bags were independently enumerated and
their counts verified by a separate forward algorithm. Exact integer alias
tables preserve that distribution. Seven H edges are sampled in order, with
the last edge enforcing the residue sum. A deterministic Hall-preserving row
choice then fills the remaining 95 positions with uniformly chosen eligible
labels. A final valid permutation has one F matching and 7! ordered H traces.

For each successful trace the weight is

`M_F × product(H edge counts) × product(completion label counts) / (114! × 7!)`.

The production F-bag count check has chi-square {float(chi.statistic):.5f},
19 degrees of freedom, p={float(chi.pvalue):.5f}. Every bag was sampled.
An independently constructed marked-F rejection pilot gives anchor probability
{rejection_estimate:.12g}, exact scaled binomial 95% interval {rejection_ci}; the
production SIS anchor estimate lies inside this interval:
{output['independent_anchor_comparison']['SIS_estimate_inside_rejection_CP95']}.
The comparator is retained only as validation and is not pooled with production.

All frozen source hashes match, all aggregate worker/bag moments reconcile,
and all 16 measured production random-number counter windows are disjoint.
There were {output['direct_feature_validations']} direct feature checks and
{output['full_score_evaluations']} full score evaluations. All retained maximum
target witnesses include their ordered H rows for independent trace replay.

The calculation concerns these exact predicates and null model. It does not
correct for every historical choice that led to proposing them, it does not
assign a probability to the textual position or meaning of the number 19, and
it does not turn any score in bits into a p-value.
'''
    (HERE / 'final_summary.md').write_text(report)
    print(json.dumps({k: output[k] for k in ['n', 'primary', 'precision_checks', 'passes_predeclared_precision_checks', 'bag_support_check', 'independent_anchor_comparison']}, indent=2))


if __name__ == '__main__':
    main()
