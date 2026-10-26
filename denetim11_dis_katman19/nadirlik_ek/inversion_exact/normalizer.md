# Exact size of the joint inversion null support

## Result and interpretation

For the study's 114 labeled items, the exact uniform-permutation probability
of the two conditioning requirements is

\[
\Pr(I_{129}=993,\ I_{127}=994)
=1.67871875613784507477\ldots\times10^{-35}.
\]

This is one in approximately **5.9569239716 × 10³⁴**, or **115.5201227722 bits**.
It measures the size of the inversion null support within unrestricted uniform
permutations. It is **not the probability of the observed full-grammar score**
under that conditional null. It must not be multiplied into the conditional
score probability without defining a different unconditional event in advance.

The original Mallows rejection sampler with
`q = 0.9145731284623988` has a theoretical acceptance probability of
**0.002110636533192102119**, or **473.7907187116 proposals per accepted draw**.
The previous training and test proposal totals agree with that prediction.

## Data facts and the adjacent-value reduction

The original data have exactly one 129, at zero-based identity 8, and exactly
one 128, at identity 15. There is no 127. The 127-count convention changes only
identity 8 from 129 to 127. The original and edited observed ascending-pair
counts were independently recounted as 993 and 994.

The convention is

\[
I(a)=\#\{(i,j):i<j,\ a_i<a_j\}.
\]

Changing 129 to 127 leaves every comparison unchanged except its comparison
with 128. If 129 appears before 128, that pair changes from uncounted to counted,
so the change is +1. If 128 appears first, the change is −1. Consequently the joint
fiber is exactly

\[
\{I_{129}=993,\;129\text{ occurs before }128\}.
\]

Let `T_k` count all distinct numerical words with inversion count k, and `A_k`
count those with 129 before 128. Swapping the two unique adjacent ranks is a
bijection from the preferred class at k−1 to the other class at k. All pair
comparisons with third items are unchanged. Therefore

\[
T_k=A_k+A_{k-1},\qquad A_{-1}=0,
\]

or, as generating polynomials,

\[
A(z)=\frac{T(z)}{1+z}.
\]

The same polynomial is obtained by merging 129 and 128 into a single tied group
of size two. Its numerical inversion count equals that of the representative
with 129 before 128. This supplies another exact coefficient check.

## Exact enumeration by generating polynomials

If the distinct numerical values have multiplicities `m_1,...,m_r`, their
inversion polynomial is the Gaussian multinomial

\[
T(z)=\frac{[114]_z!}{\prod_j[m_j]_z!},\qquad
[n]_z!=\prod_{i=1}^{n}(1+z+\cdots+z^{i-1}).
\]

The ascending-pair convention has the same coefficient polynomial as the
usual descending-pair convention by reversing a word. All coefficients were
computed as exact integers. The main calculation truncates after k = 993; formal
series division is valid because each divisor has constant coefficient one.

There are 77 distinct values, the polynomial degree is 6384, and every numerical
word has exactly

\[
\prod_j m_j! = 176120502681600
\]

labeled realizations. Thus

\[
\#\text{joint labeled fiber}=A_{993}\prod_jm_j!,\qquad
\Pr_{\rm uniform}(\text{joint fiber})
=\frac{A_{993}}{114!/\prod_jm_j!}.
\]

The exact coefficient is

```text
A_993 = 242443177649573838345119558907835969331713589035282802573127524541132859808741517025939468218607905677706859067115832077558275717980947877
```

Multiplying by the tie-label factor gives the exact labeled fiber size:

```text
42699214319367394381358834180900134003938998517611685187594869566045359645447337212488224169154611626929530737668687177793417597768935695908888326963200
```

All denominator integers, `T_993`, `A_992`, and the exact coefficient arrays
through 993 are stored in the accompanying JSON files. The recurrence was
checked as an exact integer identity.

Some related mathematical probabilities help distinguish the quantities:

| Quantity | Probability |
|---|---:|
| Uniform joint fiber | 1.6787187561378451 × 10⁻³⁵ |
| Uniform I129 exactly 993, without the second constraint | 3.2153879403718140 × 10⁻³⁵ |
| Uniform I129 at most 993 | 3.7534346087639142 × 10⁻³⁴ |
| Preferred order conditional on I129 = 993 | 0.5220890254205920 |
| Mallows I129 exactly 993 | 0.0040426755408079781 |
| Mallows joint fiber | 0.0021106365331921021 |

## Mallows normalizer and proposal comparison

The projected numerical Mallows law gives a word with ascending-pair count k
probability `q^k / T(q)`. Therefore

\[
p_{\rm accept}=\frac{A_{993}q^{993}}{T(q)}.
\]

The partition function is approximately
`3.54898744351012263075 × 10^101`. The mean strict inversion count at the
specified q is `992.9999999999994223`, consistent with the fitted target 993.

The internal tie-label randomization in the production sampler does not
change the acceptance requirement here because the two relevant identities
have unique numerical values. After acceptance it supplies uniform labeled
states within the joint fiber. Early rejection when the accumulated count
exceeds 993 also leaves the acceptance probability unchanged, since all
remaining additions are nonnegative.

For n accepted draws, the total proposal count has negative-binomial mean
`n/p` and standard deviation `sqrt(n*(1-p))/p` under the ideal sampler. The
previous recorded proposal counts compare as follows:

| Run | Accepted draws | Actual proposals | Predicted mean proposals | Standardized deviation |
|---|---:|---:|---:|---:|
| Training | 200,000 | 94,677,856 | 94,758,143.74 | −0.3793 |
| Test | 1,000,000 | 474,324,719 | 473,790,718.71 | +1.1283 |
| Combined | 1,200,000 | 569,002,575 | 568,548,862.45 | +0.8751 |

These comparisons validate a necessary feature of the sampler; they do not
establish the complete distribution on the fiber. The separate sampler and
kernel validations address that distribution. The high-precision decimal
probabilities describe the mathematical Mallows law; they do not imply that
the floating-point alias implementation realizes its probabilities to 90
decimal places. The effect of representing the specified q in binary64 is
also recorded and is negligible here.

## Validation and reproduction

Before the actual-data calculation, the script enumerates all 46,104 labeled
permutations of four small examples, including repeated values. For every
state it checks the edit delta, preferred order, inversion count, and
tie-label multiplicity. All polynomial coefficients and preferred-class
coefficients agree with exhaustive enumeration.

The actual-data calculation then receives independent checks:

- A second arithmetic route inserts tied groups using Gaussian binomial
  factors and reproduces all coefficients through 993.
- That second route computes the entire polynomial. Its mass is exactly the
  ordinary multinomial count, its degree is 6384, and its coefficients are
  palindromic with endpoint coefficients one.
- The full preferred-class polynomial has degree 6383, is palindromic, and has
  exactly half the total word count.
- Merging the adjacent unique pair into a tie reproduces every preferred
  coefficient through 993.
- Both observed inversion targets are recounted directly from the data.

Run:

```bash
python rare_event/inversion_exact/normalizer.py
```

Files:

- `normalizer.py`: exact calculation and validation.
- `normalizer.json`: exact counts, probability calculations, and recorded
  proposal comparisons.
- `coefficients_0_to_993.json`: complete exact integer coefficient archive.

Input counts SHA-256:
`0777e72c82177411059ac4bd665a44e0c27ba40ccc91d54469b08c021e00b96d`.

Coefficient archive SHA-256:
`768c5bcdcb4350ecfb105ae557ef47b35cb235bd2a602451f2988fac7bf8f685`.
