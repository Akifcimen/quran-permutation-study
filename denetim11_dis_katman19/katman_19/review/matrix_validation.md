# Independent matrix kernel validation

Validated before inspecting any new study simulation outcomes.

## Scope and API

`src/matrix.hpp` defines `MatrixEngine`. Its constructor calls an idempotent `init()`. The engine supports the 29 declared odd primes from 3 through 113. `maxorder(matrix, p)` tests exact order `p*p-1`. `eval(pidx, P, R)` returns eight packed four-bit flags, one nibble per canonical matrix-view pair. Both matrices may contain unreduced or negative row-major integer entries.

Nibble index is `P_column_flip*4 + R_row_flip*2 + R_column_flip`, with the P row flip fixed to zero. Bits are, in order: maximal order of P; maximal order of R; maximal order of PR+RP; and (PR-RP)^2=I.

For all 16 original view pairs, the corresponding canonical index is `(P_column_flip XOR P_row_flip)*4 + (R_row_flip XOR P_row_flip)*2 + (R_column_flip XOR P_row_flip)`. The discarded binary choice is simultaneous conjugation of both matrices by the swap matrix. All four predicates are invariant under that conjugation. Thus the eight canonical views cover all sixteen original views exactly at the predicate level.

## Exact method

The engine precomputes an order flag for every trace/determinant pair. Order p^2-1 requires an irreducible quadratic characteristic polynomial, and all matrices with such a polynomial are conjugate to its companion matrix. Each companion is tested by exact modular matrix exponentiation at p^2-1 and at (p^2-1)/r for every distinct prime factor r.

For runtime evaluation, put u=tr(PR), tP=tr(P), tR=tr(R), dP=det(P), and dR=det(R). The exact identities used are:

- tr(PR+RP)=2u;
- det(PR+RP)=u^2-tP*tR*u+tP^2*dR+tR^2*dP;
- det(PR+RP)+det(PR-RP)=4*dP*dR;
- tr(PR-RP)=0.

Consequently (PR-RP)^2=I if and only if det(PR+RP)=4*dP*dR+1 in the field. This avoids explicit runtime matrix products without approximation.

## Independent checks actually run

Command: `python joint_study/src/test_matrix_kernel.py`.

- All 122,701 trace/determinant entries across all 29 primes matched a Python quotient-polynomial exponentiation reference. That reference does not use the C++ discriminant filter.
- 62 matrix-pair cases, including all 29 primes, unreduced negative entries, zero matrices, identity matrices, a repeated-root case, and the stated observed P/R matrices, matched direct Python matrix multiplication and exact GL(2,p)-order reduction.
- All 992 direct individual view comparisons confirmed the 16-to-8 mapping and every nibble flag.
- The stated observed natural matrices gave orders 360 for P, R, and PR+RP, and commutator square I.
- Compilation used C++17, `-O2 -Wall -Wextra -pedantic`; no compiler warning was emitted.

No Monte Carlo study outcomes were opened by this validation.

## File hashes at validation

| File | SHA-256 |
|---|---|
| src/matrix.hpp | 03d9fba5028b7af889a12d657a13241286521ff2021ee685cf19cdcac7818f18 |
| src/matrix_reference.py | 3e6f1d016614aaa2a93cdf198a72ba393f45db562bf8e4de1de22333328ec87f |
| src/matrix_driver.cpp | e6ed1d11dd7abd0a83aba34bb4ff21cebd846e3f18509f8692e49d3756f9e59a |
| src/test_matrix_kernel.py | b820453dafe8b54ef0a8ed7b297bad3c0e84b80dd16a5f97d19631627be5ef1c |

This note validates the matrix kernel only. The feature extraction, scoring model, permutation sampler, and final statistical interpretation require separate review.
