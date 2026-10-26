# Block7 component result

Across **100,000,000 fixed production proposals**, the exact size-weighted mixture and1/K thinning produced **2,480 retained primary exceedances**. The estimated probability of the frozen primary tail intersecting the six-anchor W7 union is **1.986761971e-12**, with exact scaled-binomial95% interval **[1.909331367e-12, 2.066526847e-12]**.

The importance-weighted cross-check is 1.992369767e-12 (standard error 3.4573e-14). The two fixed batches differ by -1.084 binomial standard deviations. The observed thinning acceptance differs from its independently known exact value by 0.317 standard deviations. The pilot was excluded.

## Rigorous bounds on the full frozen block6 tail

The component is a subset of the full event. Combining its one-sided97.5% lower endpoint with the original0/1m one-sided97.5% upper endpoint gives the conservative **95% full-tail interval [1.909331367e-12, 3.68887265e-06]**. This uses the union bound for the two endpoint errors; no independence assumption is needed.

The corresponding mean waiting-time interval is **[271,085.531, 523,743,556,016.738] independent block6 draws**. It is a confidence interval for the mean waiting time of a geometric distribution,1/p, not a promise about when an individual hit will occur. The full-tail probability still has no precise point estimate from this calculation.

For the W7 subset event alone, the plug-in mean waiting estimate is 503,331,558,788.686, with interval [483,903,705,976.179, 523,743,556,016.738]. This must not be reported as the full-tail mean.

## Comparison with the natural19 pattern

The separately estimated natural127 G3,C15,W3,M15(view0) event has probability 6.042101501e-15 and95% interval [5.10179225e-15, 7.105503217e-15]. The W7 component point estimate is 328.82 times as frequent. A conservative **95% one-sided lower bound on the full-tail/natural-pattern probability ratio is 268.712**, using the component lower and natural-pattern upper97.5% endpoints.

The primary result retains the complete frozen scorer, both variants, and all grammar alternatives. Only the auxiliary proposal is conditioned on the W7 union. The complement is not assumed empty. Secondary policy counters additionally intersect any G balance and are labeled accordingly in the JSON.

Detailed exact masses, per-policy counters, individual batch values, confidence formulas, source hashes, and validation results are in `final_summary.json`.
