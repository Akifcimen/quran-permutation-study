#!/usr/bin/env python3
"""Exact inversion-fiber normalizer; no study outcome or score is estimated.

Run from any directory. Exhaustive labeled toy checks precede the actual-data
calculation. All combinatorial coefficients are Python exact integers. Decimal
arithmetic is used only for reporting probabilities and Mallows diagnostics.
"""

from collections import Counter
from decimal import Decimal, localcontext
from hashlib import sha256
from itertools import permutations
import json
from math import factorial
from pathlib import Path
import time


HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
TARGET = 993
Q_TEXT = "0.9145731284623988"


def strict_ascending(values):
    return sum(x < y for i, x in enumerate(values) for y in values[i + 1 :])


def multiply_q_integer(poly, width):
    """Multiply a truncated series by 1+q+...+q**(width-1)."""
    answer = [0] * len(poly)
    window = 0
    for k, coefficient in enumerate(poly):
        window += coefficient
        if k >= width:
            window -= poly[k - width]
        answer[k] = window
    return answer


def divide_q_integer(poly, width):
    """Exact truncated series division; constant coefficient of divisor is1."""
    answer = [0] * len(poly)
    for k, coefficient in enumerate(poly):
        answer[k] = coefficient - (poly[k - 1] if k else 0)
        if k >= width:
            answer[k] += answer[k - width]
    return answer


def multinomial_factorial(multiplicities, limit):
    """[n]_q! / product [multiplicity]_q!, via q-integer division."""
    answer = [1] + [0] * limit
    for width in range(2, sum(multiplicities) + 1):
        answer = multiply_q_integer(answer, width)
    for multiplicity in multiplicities:
        for width in range(2, multiplicity + 1):
            answer = divide_q_integer(answer, width)
    assert all(coefficient >= 0 for coefficient in answer)
    return answer


def multinomial_insertions(multiplicities, limit):
    """Independent route: insert tied groups using Gaussian binomial factors.

    At current length n, inserting m copies multiplies by
    product_i=1..m (1-q**(n+i))/(1-q**i). This implementation uses separate
    sparse subtraction and formal-series division, not q-factorial division.
    """
    answer = [1] + [0] * limit
    length = 0
    for multiplicity in multiplicities:
        for i in range(1, multiplicity + 1):
            numerator_degree = length + i
            nxt = [0] * (limit + 1)
            for k, coefficient in enumerate(answer):
                nxt[k] = coefficient
                if k >= numerator_degree:
                    nxt[k] -= answer[k - numerator_degree]
                if k >= i:
                    nxt[k] += nxt[k - i]
            answer = nxt
        length += multiplicity
        assert all(coefficient >= 0 for coefficient in answer)
    return answer


def preferred_coefficients(total):
    """A_k = T_k-A_(k-1), with A_-1=0, equivalently A(q)=T(q)/(1+q)."""
    answer = []
    previous = 0
    for coefficient in total:
        previous = coefficient - previous
        assert previous >= 0
        answer.append(previous)
    return answer


def exhaustive_toys():
    cases = [
        ([2, 4, 6, 8], 2, 1, 3),
        ([0, 0, 2, 3, 5, 5], 3, 2, 1),
        ([2, 2, 4, 6, 8, 8, 8], 3, 2, 3),
        ([0, 0, 3, 5, 7, 7, 10, 10], 3, 2, 2),
    ]
    results = []
    for values, changed_id, crossed_id, replacement in cases:
        counts = Counter(values)
        n = len(values)
        tie_factor = 1
        for multiplicity in counts.values():
            tie_factor *= factorial(multiplicity)
        degree = n * (n - 1) // 2 - sum(m * (m - 1) // 2 for m in counts.values())
        labeled_total = [0] * (degree + 1)
        labeled_preferred = [0] * (degree + 1)
        seen_words = Counter()
        checked = 0
        for ids in permutations(range(n)):
            word = tuple(values[i] for i in ids)
            edited = tuple(replacement if i == changed_id else values[i] for i in ids)
            inversion = strict_ascending(word)
            delta = strict_ascending(edited) - inversion
            preferred = ids.index(changed_id) < ids.index(crossed_id)
            assert delta == (1 if preferred else -1)
            labeled_total[inversion] += 1
            labeled_preferred[inversion] += int(preferred)
            seen_words[word] += 1
            checked += 1
        assert set(seen_words.values()) == {tie_factor}
        total = multinomial_factorial(list(counts.values()), degree)
        other = multinomial_insertions(list(reversed(list(counts.values()))), degree)
        preferred = preferred_coefficients(total)
        assert total == other
        assert [x * tie_factor for x in total] == labeled_total
        assert [x * tie_factor for x in preferred] == labeled_preferred
        assert preferred[-1] == 0
        merged_counts = counts.copy()
        del merged_counts[values[changed_id]]
        merged_counts[values[crossed_id]] += 1
        assert preferred == multinomial_factorial(list(merged_counts.values()), degree)
        assert sum(total) == factorial(n) // tie_factor
        assert sum(preferred) * 2 == sum(total)
        results.append({
            "values": values,
            "changed_id_zero_based": changed_id,
            "crossed_id_zero_based": crossed_id,
            "replacement": replacement,
            "labeled_permutations_checked": checked,
            "distinct_words": len(seen_words),
            "tie_label_factor": tie_factor,
            "degree": degree,
            "all_coefficients_match_enumeration": True,
            "adjacent_swap_delta_checked_every_state": True,
            "merged_pair_polynomial_matches": True,
        })
    return results


def decimal_q_factorials(n, q):
    factors = [Decimal(1)] * (n + 1)
    q_integer = Decimal(0)
    power = Decimal(1)
    for width in range(1, n + 1):
        q_integer += power
        factors[width] = factors[width - 1] * q_integer
        power *= q
    return factors


def projected_mean(multiplicities, q):
    n = sum(multiplicities)
    means = [Decimal(0)] * (n + 1)
    denominator = Decimal(0)
    numerator = Decimal(0)
    power = Decimal(1)
    for width in range(1, n + 1):
        denominator += power
        numerator += (width - 1) * power
        means[width] = numerator / denominator
        power *= q
    return sum(means) - sum(sum(means[: m + 1]) for m in multiplicities)


def observed_proposal_comparisons(probability):
    results = []
    total_accepted, total_proposals = 0, 0
    runs = []
    for phase in ("train", "test"):
        run_path = ROOT / "results" / f"{phase}_inversion_exact_run.json"
        run = json.loads(run_path.read_text())
        accepted, proposals = run["processed"], run["proposals"]
        runs.append((phase, accepted, proposals, str(run_path.relative_to(ROOT))))
        total_accepted += accepted
        total_proposals += proposals
    runs.append(("combined", total_accepted, total_proposals, None))
    for phase, accepted, proposals, source in runs:
        n, observed = Decimal(accepted), Decimal(proposals)
        mean = n / probability
        sd = (n * (1 - probability)).sqrt() / probability
        results.append({
            "phase": phase,
            "source": source,
            "accepted_draws": accepted,
            "observed_proposals": proposals,
            "observed_acceptance_fraction": str(n / observed),
            "predicted_proposals_mean": str(mean),
            "predicted_proposals_sd_negative_binomial": str(sd),
            "proposal_total_standardized_deviation": str((observed - mean) / sd),
            "observed_relative_error_in_proposal_total": str(observed / mean - 1),
        })
    return results


def main():
    started = time.perf_counter()
    # Fail validation before reporting any actual-data result.
    toy_results = exhaustive_toys()
    data_path = ROOT / "data" / "counts.txt"
    values = [int(x) for x in data_path.read_text().split()]
    counts = Counter(values)
    n = len(values)
    assert n == 114
    assert values[8] == 129 and values[15] == 128
    assert counts[129] == counts[128] == 1 and counts[127] == 0
    assert not any(127 <= x <= 129 and x not in (128, 129) for x in values)
    assert strict_ascending(values) == TARGET
    edited = list(values)
    edited[8] = 127
    assert strict_ascending(edited) == TARGET + 1

    multiplicities = [counts[value] for value in sorted(counts)]
    degree = n * (n - 1) // 2 - sum(m * (m - 1) // 2 for m in multiplicities)
    ties = 1
    for m in multiplicities:
        ties *= factorial(m)
    all_labeled = factorial(n)
    all_words = all_labeled // ties

    # Main requested calculation is truncated at TARGET, with exact integers.
    total = multinomial_factorial(multiplicities, TARGET)
    preferred = preferred_coefficients(total)
    # Independent arithmetic route computes the full polynomial. It checks
    # the requested coefficient, total mass, symmetry, and exact degree.
    full_total = multinomial_insertions(list(reversed(multiplicities)), degree)
    assert full_total[: TARGET + 1] == total
    assert sum(full_total) == all_words
    assert full_total == list(reversed(full_total))
    assert full_total[0] == full_total[-1] == 1
    full_preferred = preferred_coefficients(full_total)
    assert full_preferred[: TARGET + 1] == preferred
    assert full_preferred[-1] == 0
    assert full_preferred[:-1] == list(reversed(full_preferred[:-1]))
    assert sum(full_preferred) * 2 == all_words

    # Merging the two unique adjacent values creates one extra tie of size2.
    # Its multiset polynomial independently reproduces A(q)=T(q)/(1+q).
    merged_counts = counts.copy()
    del merged_counts[129]
    merged_counts[128] += 1
    merged = multinomial_factorial(list(merged_counts.values()), TARGET)
    assert merged == preferred

    joint_words = preferred[TARGET]
    single_words = total[TARGET]
    joint_labeled = joint_words * ties
    with localcontext() as context:
        context.prec = 90
        q = Decimal(Q_TEXT)
        q_factorial = decimal_q_factorials(n, q)
        partition = q_factorial[n]
        for multiplicity in multiplicities:
            partition /= q_factorial[multiplicity]
        # Report the exact decimal q supplied in the study specification and
        # also the negligible effect of its binary64 representation.
        binary_q = Decimal.from_float(float(Q_TEXT))
        binary_factorial = decimal_q_factorials(n, binary_q)
        binary_partition = binary_factorial[n]
        for multiplicity in multiplicities:
            binary_partition /= binary_factorial[multiplicity]
        uniform_joint = Decimal(joint_words) / Decimal(all_words)
        uniform_single = Decimal(single_words) / Decimal(all_words)
        mallows_joint = Decimal(joint_words) * q**TARGET / partition
        mallows_single = Decimal(single_words) * q**TARGET / partition
        binary_acceptance = Decimal(joint_words) * binary_q**TARGET / binary_partition
        result = {
            "status": "passed",
            "scope": "Exact normalizer of the joint inversion null support; not a feature or full-grammar rarity estimate.",
            "input": {
                "n": n,
                "counts_sha256": sha256(data_path.read_bytes()).hexdigest(),
                "script_sha256": sha256(Path(__file__).read_bytes()).hexdigest(),
                "changed_id_zero_based": 8,
                "original": 129,
                "replacement": 127,
                "crossed_id_zero_based": 15,
                "crossed_value": 128,
                "multiplicity_129": counts[129],
                "multiplicity_128": counts[128],
                "multiplicity_127": counts[127],
                "strict_pair_convention": "Number of i<j with a_i<a_j",
                "target_129": TARGET,
                "target_127": TARGET + 1,
                "joint_condition": "I129=993 and original ID8 (129) precedes original ID15 (128)",
                "mallows_q_decimal": Q_TEXT,
                "mallows_q_binary64_exact_decimal": str(binary_q),
            },
            "exact_counts": {
                "polynomial_degree_T": degree,
                "polynomial_degree_A": degree - 1,
                "distinct_values": len(counts),
                "tie_label_factor": str(ties),
                "all_unlabeled_words": str(all_words),
                "all_labeled_permutations": str(all_labeled),
                "single_I993_unlabeled_words_T993": str(single_words),
                "joint_I993_I994_unlabeled_words_A993": str(joint_words),
                "joint_I993_I994_labeled_permutations": str(joint_labeled),
                "preferred_previous_A992": str(preferred[TARGET - 1]),
                "recurrence": "A993=T993-A992; A_minus1=0",
            },
            "probabilities_decimal_90_digit_arithmetic": {
                "uniform_joint": str(uniform_joint),
                "uniform_single_I993": str(uniform_single),
                "conditional_preferred_order_given_I993": str(Decimal(joint_words) / Decimal(single_words)),
                "uniform_joint_one_in": str(1 / uniform_joint),
                "uniform_joint_surprisal_bits": str(-uniform_joint.ln() / Decimal(2).ln()),
                "uniform_lower_tail_I_at_most_993": str(Decimal(sum(total)) / Decimal(all_words)),
                "projected_mallows_partition_Tq": str(partition),
                "projected_mallows_expected_I": str(projected_mean(multiplicities, q)),
                "mallows_joint_acceptance": str(mallows_joint),
                "mallows_single_I993_probability": str(mallows_single),
                "mallows_expected_proposals_per_accept": str(1 / mallows_joint),
                "mallows_preferred_order_probability_unconditional": str(1 / (1 + q)),
                "binary64_q_mallows_joint_acceptance": str(binary_acceptance),
                "binary64_q_relative_acceptance_difference": str(binary_acceptance / mallows_joint - 1),
            },
            "observed_proposal_comparisons": observed_proposal_comparisons(binary_acceptance),
            "validation": {
                "toys": toy_results,
                "total_labeled_toy_permutations": sum(x["labeled_permutations_checked"] for x in toy_results),
                "full_polynomial_independent_route_agrees_through_993": True,
                "full_polynomial_total_equals_multinomial": True,
                "full_polynomial_palindromic": True,
                "full_preferred_polynomial_palindromic_at_degree_D_minus_1": True,
                "full_preferred_mass_is_half_all_words": True,
                "merged_adjacent_pair_coefficients_agree_through_993": True,
                "observed_both_inversion_targets_recounted": True,
                "decimal_reporting_precision": context.prec,
            },
        }
    coefficients = {
        "definition": "T_k counts numeric words; A_k counts those with129 before128; exact nonnegative integers encoded as strings.",
        "maximum_degree_saved": TARGET,
        "T": [str(x) for x in total],
        "A": [str(x) for x in preferred],
    }
    coefficient_path = HERE / "coefficients_0_to_993.json"
    coefficient_path.write_text(json.dumps(coefficients, indent=2) + "\n")
    result["coefficient_file_sha256"] = sha256(coefficient_path.read_bytes()).hexdigest()
    result["seconds"] = time.perf_counter() - started
    output = HERE / "normalizer.json"
    output.write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
