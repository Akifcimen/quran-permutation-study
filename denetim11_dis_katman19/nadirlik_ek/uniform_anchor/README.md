# Uniform natural-127 anchor calculation

This directory estimates a fixed event under uniform permutations of 114
**labeled** surah IDs: natural form `(1,1)`, prime 19, count variant 127,
`C15 & W3 & G3 & M15` in natural matrix view 0. The original frozen features,
score grammar and training data are unchanged.

The event allows all coefficient bags satisfying the frozen C predicates;
it does not require every literal coefficient to match the observed bag.
There are exactly 20 eligible F bags in this uniform model (as independently
verified), compared with the two accessible bags in the block6 model.

## Main files

- `production_plan.json`: fixed pre-production sample budget, seeds, source
  hashes, exact event scope and precision diagnostics.
- `production_batch0.json`, `production_batch1.json`: full production moments,
  including all 100 million proposals, 16 worker streams, per-bag moments,
  random-number call counts and maximum-weight target witnesses.
- `final_summary.json`, `final_summary.md`: estimates and uncertainty.
- `analyze_production.py`: reconstructs summaries, verifies aggregation and
  frozen hashes, checks measured production RNG counter windows, and computes
  weighted uncertainty. Uses NumPy/SciPy dependencies installed for the study.
- `f_matchings.py`: exact labeled partial-matching dynamic program and sampler
  table exporter.
- `F_conditional_sampler_127.bin`: exact integer dynamic-programming sampler
  table; arithmetic alias tables are constructed and checked at runtime.
- `F_matching_count_127.json`, `F_forward_validation.json`: exact counts and
  independent forward verification.
- `uniform_sis_production.hpp`, `production_uniform.cpp`: production sampler
  and metrics. The corresponding pilot sources and all pilot outcomes remain
  archived and are excluded from the production estimate.

## Estimator

Eligible F matchings are sampled uniformly from the exact count
`M_F = 985178375935386285`. Seven H edges are sampled in order; the last H
edge enforces the sum residue. A Hall-preserving completion assigns the
remaining 95 labels while avoiding any additional F or H edge.

Each complete permutation in the anchor event has one F matching and
exactly `7!` ordered H traces. The contribution of a successful trace is

```text
M_F × product(H available-edge counts) × product(completion label counts)
---------------------------------------------------------------------
                            114! × 7!
```

Each target indicator multiplies that contribution. The estimate divides by
**all proposals**, including H deadends as zero. Retrying deadends until a
success, dropping zero batches, or omitting `7!` would change the estimator
and is not done here.

The primary event estimate is approximately `7.7807161113e-18`, corresponding
to an expected recurrence parameter of `1.2852287446e17`. Weighted-moment and
independent-worker confidence intervals are **approximate Monte Carlo
intervals**, not exact binomial intervals. Their reciprocal intervals concern
the expected recurrence parameter, not realized waiting times.

Full score evaluation is performed only when natural G3 holds in either
variant. Consequently the recorded score-tail probabilities apply to

```text
C15W3(127) ∩ (natural G3(127) ∪ natural G3(129)) ∩ full frozen score tail.
```

They are components of the complete score tail; this directory does not
estimate or prove coverage of the score tail outside that intersection.

## Reproduce

Run from the `joint_study` directory. The exact sampler table and retained
outputs are included so that ordinary analysis does not rerun the simulation.

```bash
PYTHONPATH=.deps python rare_event/uniform_anchor/analyze_production.py
```

On another machine, install the study's NumPy/SciPy requirements in a Python
environment and omit `PYTHONPATH=.deps` if they are already importable.

To regenerate the sampler table (several minutes):

```bash
python rare_event/uniform_anchor/f_matchings.py --variant 127 --export-sampler
```

To compile and rerun the fixed production budget:

```bash
g++ -O3 -std=c++17 -march=native -fopenmp rare_event/uniform_anchor/production_uniform.cpp -o rare_event/uniform_anchor/production_uniform
rare_event/uniform_anchor/production_uniform . 50000000 202610263001 8 sis 1 rare_event/uniform_anchor/production_batch0.json 127 fixed_production
rare_event/uniform_anchor/production_uniform . 50000000 202610263002 8 sis 1 rare_event/uniform_anchor/production_batch1.json 127 fixed_production
PYTHONPATH=.deps python rare_event/uniform_anchor/analyze_production.py
```

The two output files are intentionally tied to the frozen seeds and 8-worker
partition. Reproduction should take place in a copied directory if the original
evidence files are to be preserved byte-for-byte. Different architectures may
change final floating-point decimal rounding without changing integer counts.

Independent reviewer files are under `../review/uniform_*`.
