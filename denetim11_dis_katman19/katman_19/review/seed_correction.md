# Pre-outcome worker-seed correction and independent audit

## Defect found in source review

The initial worker seed was `master + G*(worker+1)`, where G is also the increment used by SplitMix64 on every output. Consequently neighboring workers were assigned the same deterministic output stream shifted by one position. In particular, worker 1's first output was worker 0's second output. Treating these as separated worker streams was incorrect.

This finding came from source inspection and an algebraic reconstruction of the generator. No observed score or new inferential control score was inspected to find or resolve it.

## Retirement of the initial training

The study owner explicitly retired the initial training runs and committed to regenerate **all** training tables with the corrected worker seed rule before production testing. No table produced with the retired seed mapping is eligible for scoring or the final inference. This audit did not open the retired feature histograms.

The correction changes random-stream initialization and bookkeeping only. It does not alter the finite grammar, feature definitions, semantic policies, surrogate score, declared null models, training/test budgets, or stopping rules.

## Corrected implementation

Each worker's initial state is now the output of a SplitMix64 step applied to

`master XOR (0xd1b54a32d192ed03 * (worker+1))`.

This mixed value becomes the initial state of that worker's sampler RNG. It removes the direct adjacent-output reuse in the retired construction.

The main program also now:

- disables OpenMP dynamic team resizing;
- divides work by the actual team size returned by the runtime;
- checks that the sum of processed counts equals the requested sample count;
- records actual and requested thread counts and the exact worker seeds;
- retains 50 examples per worker before selecting the global top 50.

Retaining at least 50 per worker suffices for the actual global top 50, including cases where most high-scoring examples arise in one worker.

## Independent verification actually performed

Command: `python joint_study/tests/test_worker_seeding.py`.

The test compiled the revised production `study_engine.cpp` separately with C++17, OpenMP, and compiler warnings enabled. It independently reconstructed all worker seeds for the six declared training/test master seeds.

- All 48 planned worker starting states were distinct.
- The first 1,024 outputs from each of these 48 streams, 49,152 outputs total, had no overlap.
- The minimum circular distance between their starting counters in the SplitMix cycle was 27,188,455,771,641,243 output steps.
- The retired adjacent-stream overlap was reproduced explicitly as a regression check.
- The generated worker seeds recorded by the compiled program matched the independent Python calculation exactly.

Five tiny correctness runs checked the production work allocation:

| Phase | Model | Requested samples | Requested threads | Actual threads | Processed |
|---|---|---:|---:|---:|---:|
| Bench | Uniform | 1 | 8 | 8 | 1 |
| Bench | Uniform | 17 | 8 | 8 | 17 |
| Bench | Block of six | 3 | 8 | 2 | 3 |
| Training fixture | Uniform | 5 | 8 | 2 | 5 |
| Bench | Exact inversion conditioning | 3 | 8 | 2 | 3 |

The two-thread cases used `OMP_THREAD_LIMIT=2`. The checks also exercised an external `OMP_DYNAMIC=TRUE` setting and confirmed that the program's explicit setting controlled its team behavior. The tiny training fixture's binary header recorded exactly five accepted samples and five proposals. No feature-frequency cells were read.

The correctness runs used temporary output paths. Only run metadata and the tiny training record count were inspected; no observed target scores, inferential study score files, or null feature frequencies were opened.

## Audit boundary

The revised source hash at validation was:

`81cfc2ac3f40cd6d2859317832de2bd7671416320908737038a2a26c8b093cc2`

The source did not change between compilation and completion of these checks. Machine-readable details are in `review/seed_validation.json`.

These checks remove a concrete stream-reuse defect and verify the finite runtime bookkeeping. They are not a proof of mathematical independence of a deterministic finite pseudorandom generator. The study uses separated pseudorandom streams under the usual simulation assumption.

**Audit status: corrected worker-seed and work-allocation implementation passed.** Final run manifests must still identify the regenerated training and fresh test streams, and the final score-file lengths must agree with their recorded accepted sample counts.
