# Reversible mutation kernels for rare-event simulation

## Status and scope

The implementation in rare_event/src/kernels.hpp passed independent exact
transition checks, production inversion-update checks, and empirical
independent-start stationarity checks. It does not change any frozen feature,
score, textual weight, count convention, or null model.

The kernels provide transitions invariant for the same three uniform
conditional permutation distributions. They do **not** establish useful mixing
at a deep rare-event level. A rare-event estimator still requires independent
replicates, numerical-movement diagnostics, and checks for mode/genealogy collapse.

## API

~~~cpp
rare::MutationKernel kernel(base129, model, seed, refresh_per_mille = 10);
rare::Proposal p = kernel.propose(current_ids);
if (p.null_ok) {
    // Apply the splitting-level test or a soft-target Metropolis ratio here.
    // Rejecting means retaining current_ids for this actual Markov step.
}
~~~

Models are 0=uniform, 1=block6, 2=exact joint inversion. The input current state
must already belong to the selected null support. Construct a kernel once per
worker, rather than once per particle.

The proposal contains:

- draw.ids: candidate labeled permutation.
- null_ok: membership in the original null support, including both exact
  inversion counts where required.
- changed: whether any labeled identity changed position.
- numeric_changed: whether at least one 129 or 127 numerical value changed
  position. A changed tie label can have no effect on the study statistic.
- kind: the proposal family.
- delta129/delta127: accumulated inversion changes for local fiber moves.
- refresh_proposals: Mallows attempts consumed by an independent refresh.
- repair_alternatives: restoring second transpositions other than undoing the
  first.
- filter_prime: independently selected prime for a residue-preserving move.

propose_kind is provided for fixed pilot comparisons and validation. Selecting a
move family based on the current state generally invalidates the symmetry
argument unless a separate Hastings correction is supplied.

When numeric_changed is false, a caller may retain the cached study features
and score; labeled positions can still be updated. Distinguish numerical motion
from nominal acceptance in convergence reports.

## Symmetric local moves

A uniformly selected transposition is an involution. A fixed number of iid
transpositions has exactly the same proposal probability as the reverse
sequence. A three-cycle is selected with both orientations equally likely. A
selected six-position set is shuffled uniformly. All position choices and
mixture weights are independent of the state.

The block model confines every selected position set to a fixed block.
Rejecting final proposals outside an inversion fiber therefore retains the
uniform conditional distribution, without a Hastings ratio.

A single swap of two unequal values changes the strict inversion count by a
nonzero amount. Consequently a single-transposition chain on a fixed inversion
fiber can move only tied labels. The inversion mixture uses paired swaps,
three-cycles, four swaps, and paired repair.

### Fast swap deltas

For i<j, x=a[i], y=a[j], the change in strict ascending-pair count equals

~~~text
sign(x-y) × [1 + number of intermediate values inclusively between x,y
              + number of intermediate values strictly between x,y]
~~~

It is zero for x=y. The direct O(distance) update is independently checked against
an O(N²) definition. Rank-prefix tables permit every possible second-swap delta
to be queried in O(1), after O(N²) table construction.

### Parity and residue filters

Parity moves select two positions of the same positional parity and swap their
values only when the values have equal parity. The membership in the parity
split and its aggregate table P are preserved. The filter condition is unchanged
under the reverse swap, so a failed filter is a symmetric self-transition.

Residue moves additionally require both count conventions' values to agree
modulo 2p, where p is drawn from the fixed primary semantic mixture independently
of the state. For every one of the 14 forms, the corresponding R matrix modulo p
is preserved as well. Exact raw P preservation and modular R preservation were
checked against direct aggregates.

Two filtered moves use the same selected prime and two iid filtered swap steps.
Their composition is reversible. A failed filter is never retried until success.

## Exact paired repair

From a state x in the joint inversion fiber:

1. Choose a transposition u uniformly among all unordered position pairs.
2. Apply it, obtaining an intermediate state z, potentially outside the fiber.
3. Enumerate A(z), all transpositions v that return z to the fixed joint fiber.
4. If |A(z)|>1, choose uniformly from A(z) excluding u. If |A(z)|=1, undo u.
5. Return y=v(z).

The inverse u always belongs to A(z), so the construction is defined.

For a nontrivial path x --u--> z --v--> y, the reverse path uses v first, reaches
the same z, and chooses u second. Both paths have probability

~~~text
1 / binomial(N,2) / (|A(z)| - 1).
~~~

The target fiber is fixed, so A(z) does not depend on which endpoint initiated
the path. Thus the proposal is symmetric on the fiber. Excluding the undo move
does not disturb detailed balance. Multiple paths add symmetric contributions.

The implementation uses two rank-prefix tables, one for each count convention,
and checks both inversion deltas. Reservoir sampling selects the second
transposition uniformly without storing a large vector.

## Full support and its practical limit

The default mixture contains 1% independent draws from the validated original
null sampler. Positive independent-refresh weight gives a positive probability
to every null state; after a level-set rejection it gives full support within
every nonempty level set. This establishes irreducibility mathematically.

At a very rare level, however, almost every independent refresh will fail the
level condition. It does not supply a useful finite-time mixing guarantee.
The exact toy audit explicitly found disconnected components for adjacent-only
or filtered-only kernels inside an additional level set. Broad paired/global
moves and convergence checks are required in practice.

## Checks actually run

### Exact arithmetic and transition structure

- 103,680 comparisons of direct and rank-prefix swap deltas against an
  independent O(M²) reference over all 720 labeled orders of a six-item tied
  multiset, with an edited tied identity.
- Exact symmetry and row masses for global, adjacent, parity-filtered,
  residue-filtered, and inverse-paired three-cycle proposals.
- Exact constrained transition matrices for a 35-state joint inversion fiber
  and an additional 18-state event.
- Exact rational paired-repair transition probabilities, including exclusion of
  the undo move, row sums, detailed balance, and additional event rejection.
- Positive independent refresh verified to connect every pair of fiber states.

### Actual 114-item implementation

For each null model, 100,000 chained proposals were generated. Candidate
inversion deltas and membership were reconciled with direct counts in 2,098
checks per model. Between 18,200 and 19,908 direct aggregate-preservation checks
were performed per model.

For each model, 6,000 independent null starts were each evolved for 20 mutation
steps. Six numerical/position moments were compared with a separate group of
6,000 independent null draws. Maximum absolute standardized differences were:

| Model | Maximum absolute z |
|---|---:|
| Uniform | 1.607 |
| Block6 | 1.503 |
| Exact inversion | 3.568 |

The threshold of 6 was fixed in the validation code. These are empirical
reasonableness checks, not a proof of independence or rare-tail mixing.

The combined runtime was approximately 18.5 seconds on the current host.
The inversion portion includes independent-null draws and production mutation
checks, so it is not a pure per-proposal speed benchmark.

## Actual inversion-fiber movement

The following proportions are from the 100,000-proposal unconstrained-fiber
validation chain, **before any additional rare-event score restriction**:

| Move | Null-valid proposals | Numeric changes |
|---|---:|---:|
| Adjacent paired swaps | 47.77% | 46.82% |
| Adjacent three-cycle | 31.59% | 31.53% |
| Global paired swaps | 0.71% | 0.69% |
| Four global swaps | 0.08% | 0.08% |
| Parity-filtered paired swaps | 26.37% | 0.15% |
| Residue-filtered paired swaps | 89.12% | 0.00% in 9,880 proposals |
| Exact paired repair | 100% | 61.15% |
| Independent refresh | 100% | 100% |

This illustrates why a high nominal acceptance rate can be misleading:
filtered proposals often return the same numerical arrangement.

## Reproduction

~~~bash
g++ -O3 -std=c++17 -Wall -Wextra -Wpedantic \
  rare_event/tests/test_kernels.cpp -o rare_event/tests/test_kernels
./rare_event/tests/test_kernels data/counts.txt \
  > rare_event/tests/kernel_runtime_validation.json
python rare_event/tests/test_kernels_exact.py
~~~

Machine-readable results are kernel_runtime_validation.json and
kernel_exact_validation.json. The production header SHA-256 at validation is
3a1778c180b36995c31c540ea057b8856bda037b2f074f977dd720ee58212807.

