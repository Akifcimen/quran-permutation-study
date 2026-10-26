# Rare-event extension of the frozen joint-grammar study

This directory extends the earlier study without changing its data, trained
score, primary threshold, candidate grammar, or original results. The dated
plans separate pilot work from fixed-budget production. The main report and
executed notebook explain which calculations estimate a fixed conjunction,
which estimate a component of a scanned-score tail, and which attempt its
complete tail.

## Read the results

- `nadirlik_ek_raporu.md`: Turkish report with numerical results and limitations.
- `nadirlik_ek_analizi.html`: self-contained rendered, executed notebook.
- `nadirlik_ek_analizi.ipynb`: executable companion using saved outputs.
- `summary.json`: recomputed probability and uncertainty summaries.
- `package_manifest.json`: SHA-256 and size of every extension file included in
  the distribution, excluding the manifest itself.

The full distribution also includes the original `joint_study` archive entries.
Their contents are copied unchanged. The archive-level manifest records both
the original entries and this extension. No external compiled executable or
private runtime directory is required to inspect the results.

## Recompute reported tables and figures

Work from the `joint_study` directory, which contains `protocol.json`. Use
Python 3.12 and install the packages listed in `rare_event/requirements.txt` in
your own environment. Ordinary numerical analysis requires only numpy, scipy
and matplotlib; notebook execution adds the remaining packages.

```bash
python rare_event/analyze_extension.py --plots
python rare_event/build_deliverables.py
```

These commands analyze the saved fixed-budget outputs. They do **not** silently
launch or extend any Monte Carlo production run. The second command executes
every notebook cell and exports the HTML version. A notebook can also be opened
in Jupyter, with the working directory set to the package root.

## Components and independent checks

| Directory | Purpose |
|---|---|
| `exact/` | Exact block6 counts, exact conditional samplers, two independent 100m-proposal experiments, natural matrix residue reductions |
| `block7/` | Exact six-anchor counts and intersections; 100m fixed proposals for the primary score-tail component; binomial inference after thinning |
| `uniform_anchor/` | Exact F-matching counts and integer alias sampler; 100m weighted independent proposals for the fixed natural 127 conjunction under unrestricted permutations |
| `production/` | Three original null models, eight independent SMC replicates each; all final states, levels, weights, and fixed plan |
| `pilot/` | All hard-score and guided SMC pilots, excluded from production means |
| `src/` | SMC, reversible null kernels, and exact-endpoint soft guide |
| `review/` | Independent probability, state, score, source, support, seed and interpretation audits |
| `tests/` | Finite-state tests and exact conditional sampler checks |
| `inversion_exact/` | Exact inversion-fiber normalization and rejection-proposal validation |

The original scorer and feature extractor remain in `src/` at the package
root. In particular, the production audit re-evaluates all six frozen policy
scores on all 12,288 saved SMC states. Precision failures remain in the output;
they are not removed or replaced by successful runs.

```bash
python rare_event/review/production_review.py
python rare_event/exact/validate_exact.py
python rare_event/exact/validate_exact_c10.py
python rare_event/block7/independent_counts.py
python rare_event/block7/independent_intersections.py
```

Individual component READMEs explain the exact samplers and their proofs.
The exact count validators may take longer than summary regeneration.

## Re-run the expensive experiments

The recorded JSON plans are authoritative for counts, seeds, model settings,
source hashes, scoring scope and stopping rules. Compile with a C++17 compiler,
Boost headers and OpenMP support. For example:

```bash
g++ -O3 -std=c++17 -march=native -fopenmp rare_event/src/smc_guided_stratified.cpp -o rare_event/smc_guided_stratified
g++ -O3 -std=c++17 -march=native -fopenmp rare_event/exact/sample_cw.cpp -o rare_event/exact/sample_cw
g++ -O3 -std=c++17 -march=native -fopenmp rare_event/exact/sample_cw10.cpp -o rare_event/exact/sample_cw10
g++ -O3 -std=c++17 -march=native -fopenmp rare_event/block7/sample.cpp -o rare_event/block7/sample
g++ -O3 -std=c++17 -march=native -fopenmp rare_event/uniform_anchor/production_uniform.cpp -o rare_event/uniform_anchor/production_uniform
```

Keep reruns in a new directory so the published evidence stays available:

```bash
mkdir -p rare_event/rerun
rare_event/exact/sample_cw . 100000000 202610250701 8 rare_event/rerun/block6_CW.json
rare_event/exact/sample_cw10 . 100000000 202610250801 8 rare_event/rerun/block6_C10W.json
rare_event/block7/sample . 50000000 202610251702 8 rare_event/rerun/block7_batch0.json
rare_event/block7/sample . 50000000 202610251703 8 rare_event/rerun/block7_batch1.json
rare_event/uniform_anchor/production_uniform . 50000000 202610263001 8 sis 1 rare_event/rerun/uniform_batch0.json 127 production
rare_event/uniform_anchor/production_uniform . 50000000 202610263002 8 sis 1 rare_event/rerun/uniform_batch1.json 127 production
```

Example SMC command for the uniform model:

```bash
rare_event/smc_guided_stratified --model uniform --out rare_event/rerun/uniform --schedule rare_event/pilot/schedule_soft.txt --train results/train_uniform.bin --counts data/counts.txt --population 512 --reps 8 --moves 128 --threads 8 --refresh 10 --seed 202610251201 --initialization iid
```

The other two SMC models use `block6` / seed `202610251202` and
`inversion_exact` / seed `202610251203`, with the matching training file and
distinct output prefix. The full SMC runs are computationally substantial.
Timing and hashes are retained in the published logs. Bitwise reproducibility
of floating-point details may depend on compiler and machine; the original
source, schedules, seeds, integer counting tables and all recorded outputs are
included for comparison.

## Interpretation rules

1. A fixed natural 19 conjunction is a different event from the maximum over
   the finite grammar of alternatives. Its reciprocal is not an adjusted
   global rarity.
2. Exact anchor probability times an iid conditional binomial estimate permits
   a scaled Clopper–Pearson interval. Unequal importance weights and dependent
   SMC particles do not permit that same interval.
3. A component gives a lower bound on the complete event's probability, not an
   upper bound. Reciprocal inequalities reverse.
4. Every reported interval for `1/p` describes uncertainty in an **expected**
   waiting count; it is not a prediction interval for an individual waiting
   time.
5. Text-derived prime weights were frozen before these simulations but after
   the 19 hypothesis was known. They encode salience within the fixed grammar;
   they do not independently estimate the chance of a particular verse's
   location or meaning.
6. Results are conditional on the stated null model. Distinct null models are
   not pooled. No multiplication by the probability of the exact inversion
   support is part of the conditional test.
