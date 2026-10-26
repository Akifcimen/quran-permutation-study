"""Analyze the frozen Monte Carlo study without changing its score or sample size.

Run after the three training and test commands in run_study.sh.
Only NumPy, SciPy and Matplotlib are required. Scores are ranking utilities;
2**(-score) is deliberately never interpreted as a probability.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path

import numpy as np
from scipy.stats import beta

ROOT = Path(__file__).resolve().parent
MODELS = ["uniform", "block6", "inversion_exact"]
POLICIES = ["uniform", "mixture", "literal", "discussion", "anchored19", "frequency"]
POLICY_JSON_NAMES = ["uniform_29", "primary_mixture", "literal_integer_values", "number_discussion_values", "conditional_19", "literal_frequency"]
MODEL_LABELS = {"uniform": "Serbest sıralama", "block6": "Altılı bloklar", "inversion_exact": "Aynı terslik sayıları"}
POLICY_LABELS = ["Metin ağırlığı yok", "Birincil karışım", "Açık sayı değerleri", "Sayı tartışması", "19'a koşullu", "Geçiş sıklığı"]
DIAG_NAMES = [
    "E1_all_modules_somewhere_same_variant", "E2_shared_form_prime", "E3_centre_equals_prime", "E4_discussion_prime",
    "any_C7", "any_C15", "any_C7_and_M15", "any_C15_and_M15",
    "natural129_C7", "natural129_C15", "natural129_M15", "natural129_C7_M15", "natural129_G3", "natural129_W3",
    "natural127_C7", "natural127_C15", "natural127_M15", "natural127_C7_M15", "natural127_G3", "natural127_W3",
]


def binomial_summary(hits: int, n: int, *, rank: bool = False) -> dict:
    if n == 0:
        return {"hits": hits, "n": n, "rate": None, "cp95": None, "upper95_one_sided": None, "status": "not_estimable"}
    if not 0 <= hits <= n:
        raise ValueError((hits, n))
    lo = 0.0 if hits == 0 else float(beta.ppf(0.025, hits, n - hits + 1))
    hi = 1.0 if hits == n else float(beta.ppf(0.975, hits + 1, n - hits))
    one = 1.0 if hits == n else float(beta.ppf(0.95, hits + 1, n - hits))
    result = {"hits": hits, "n": n, "rate": hits / n, "cp95": [lo, hi], "upper95_one_sided": one}
    if rank:
        result["p_plus_one"] = (hits + 1) / (n + 1)
    return result


def load_scores(model: str) -> np.ndarray:
    prefix = ROOT / "results" / ("test_" + model)
    log = json.loads(prefix.with_name(prefix.name + "_run.json").read_text())
    paths = [prefix.with_name(prefix.name + f"_scores_{i}.bin") for i in range(log["threads"])]
    if all(p.exists() for p in paths):
        arrays = [np.fromfile(p, dtype="<f8").reshape(-1, len(POLICIES)) for p in paths]
        values = np.concatenate(arrays, axis=0)
    else:
        with np.load(prefix.with_name(prefix.name + "_scores.npz")) as packed:
            values = packed["scores"]
    assert values.shape == (log["n"], len(POLICIES))
    assert log["processed"] == log["n"]
    assert np.all(np.isfinite(values)) and np.all(values >= 0)
    return values


def analyze(*, compress: bool = True, plots: bool = True) -> dict:
    protocol = json.loads((ROOT / "protocol.json").read_text())
    summary = {
        "protocol_sha256": hashlib.sha256((ROOT / "protocol.json").read_bytes()).hexdigest(),
        "training_per_model": protocol["training_per_model"],
        "testing_per_model": protocol["testing_per_model"],
        "primary_policy": "mixture",
        "score_interpretation": "Calibrated surrogate ranking utility; NOT a joint event surprisal or analytic p bound.",
        "scope": protocol["selection_scope"],
        "models": {},
    }
    score_arrays = {}
    for model in MODELS:
        result_dir = ROOT / "results"
        run = json.loads((result_dir / f"test_{model}_run.json").read_text())
        train = json.loads((result_dir / f"train_{model}_run.json").read_text())
        assert train["n"] == protocol["training_per_model"]
        assert run["n"] == protocol["testing_per_model"]
        assert set(train["worker_seeds"]).isdisjoint(run["worker_seeds"])
        scores = load_scores(model)
        score_arrays[model] = scores
        obs = json.loads((result_dir / f"observed_{model}_scores.json").read_text())
        policies = {}
        for k, name in enumerate(POLICIES):
            assert obs["policies"][k]["name"] == POLICY_JSON_NAMES[k]
            target = obs["policies"][k]["value"]
            samples = scores[:, k]
            hits = int(np.count_nonzero(samples >= target))
            policies[name] = {
                "observed_score": target,
                **binomial_summary(hits, len(samples), rank=True),
                "control_mean": float(samples.mean()),
                "control_sd": float(samples.std()),
                "control_max": float(samples.max()),
                "control_quantiles": {str(q): float(np.quantile(samples, q)) for q in [0, .5, .9, .95, .99, .999, .9999]},
                "winning_program": obs["policies"][k],
            }
        diag = json.loads((result_dir / f"test_{model}_diagnostics.json").read_text())
        counts = diag["counts"]
        assert diag["n"] == len(scores) and len(counts) == 20
        assert counts[3] <= counts[2] <= counts[1] <= counts[0]
        assert counts[7] <= counts[6] <= counts[4]
        diagnostics = {name: binomial_summary(int(c), len(scores)) for name, c in zip(DIAG_NAMES, counts)}
        extensions = {f"E{i+2}_given_E{i+1}": binomial_summary(counts[i+1], counts[i]) for i in range(3)}
        extensions["C15_given_C7"] = binomial_summary(counts[5], counts[4])
        extensions["M15_given_C7"] = binomial_summary(counts[6], counts[4])
        extensions["M15_given_C15"] = binomial_summary(counts[7], counts[5])
        for v, start in [(129, 8), (127, 14)]:
            extensions[f"natural{v}_M15_given_C7"] = binomial_summary(counts[start+3], counts[start])
            extensions[f"natural{v}_C7_given_M15"] = binomial_summary(counts[start+3], counts[start+2])
        summary["models"][model] = {"training_run": train, "test_run": run, "policies": policies, "diagnostics": diagnostics, "conditional_extensions": extensions}
        if compress:
            np.savez_compressed(result_dir / f"test_{model}_scores.npz", scores=scores, policy_names=np.array(POLICIES))
    summary["robust_max_p"] = {p: max(summary["models"][m]["policies"][p]["p_plus_one"] for m in MODELS) for p in POLICIES}
    q19 = (1/29 + 1/5 + 1/4) / 3
    summary["text_weight_19"] = {"uniform": 1/29, "primary_mixture": q19, "coding_advantage_bits_per_binding": float(np.log2(q19 * 29)), "independent_probability_factor": False}
    summary["text_score_increments"] = {
        m: {p: summary["models"][m]["policies"][p]["observed_score"] - summary["models"][m]["policies"]["uniform"]["observed_score"] for p in POLICIES}
        for m in MODELS
    }
    (ROOT / "results/summary.json").write_text(json.dumps(summary, ensure_ascii=False, indent=2) + "\n")
    if plots:
        make_plots(summary, score_arrays)
    return summary


def make_plots(summary: dict, score_arrays: dict) -> None:
    os.environ.setdefault("MPLCONFIGDIR", str(ROOT / ".cache/matplotlib"))
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    plt.rcParams.update({"font.family": "DejaVu Sans", "font.size": 11, "axes.spines.top": False, "axes.spines.right": False})
    fig, axes = plt.subplots(1, 3, figsize=(15, 5), sharey=True, layout="constrained")
    for ax, model in zip(axes, MODELS):
        d = summary["models"][model]["policies"]["mixture"]
        samples = score_arrays[model][:, 1]
        bins = np.linspace(0, max(d["control_max"], d["observed_score"]) + 2, 65)
        ax.hist(samples, bins=bins, color="#417b91", alpha=.9, label="1.000.000 kontrol")
        ax.set_yscale("log")
        ax.axvline(d["observed_score"], color="#bf4b35", linewidth=2.5, label="Gözlenen düzen")
        ax.set_title(MODEL_LABELS[model])
        ax.set_xlabel("Tarif maliyeti çıkarılmış sıralama puanı")
        ax.text(.97, .96, f"Gözlenen: {d['observed_score']:.2f}\nKontrol en yüksek: {d['control_max']:.2f}\nEşit/aşan: {d['hits']:,}", transform=ax.transAxes, ha="right", va="top", fontsize=10, bbox={"facecolor":"white", "alpha":.9, "edgecolor":"none"})
    axes[0].set_ylabel("Kontrol sayısı · logaritmik ölçek")
    axes[-1].legend(loc="lower right", fontsize=9)
    fig.suptitle("Aynı tam arama uygulanmış kontrol dağılımları — birincil metin karışımı", fontsize=15)
    fig.savefig(ROOT / "figures/primary_distributions.png", dpi=180)
    fig.savefig(ROOT / "figures/primary_distributions.svg")
    plt.close(fig)
    fig, ax = plt.subplots(figsize=(10, 5), layout="constrained")
    x = np.arange(6)
    colors = ["#417b91", "#bd8342", "#6e5b91"]
    for model, color, offset in zip(MODELS, colors, [-.22, 0, .22]):
        vals = [summary["models"][model]["policies"][p]["observed_score"] for p in POLICIES]
        ax.bar(x + offset, vals, width=.22, label=MODEL_LABELS[model], color=color)
    ax.set_xticks(x, [s.replace(" ", "\n", 1) for s in POLICY_LABELS])
    ax.set_ylabel("Gözlenen sıralama puanı")
    ax.set_title("Metinsel sayı kuralına duyarlılık")
    ax.legend(loc="upper left", fontsize=9)
    fig.text(.5, -.015, "Puan farkı bağımsız olasılık çarpanı değildir; her sütun kendi kontrol dağılımıyla sınanır.", ha="center", fontsize=10)
    fig.savefig(ROOT / "figures/text_policy_sensitivity.png", dpi=180, bbox_inches="tight")
    plt.close(fig)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--no-compress", action="store_true")
    parser.add_argument("--no-plots", action="store_true")
    args = parser.parse_args()
    result = analyze(compress=not args.no_compress, plots=not args.no_plots)
    for model in MODELS:
        d = result["models"][model]["policies"]["mixture"]
        print(f"{model}: target={d['observed_score']:.9f} max={d['control_max']:.9f} hits={d['hits']}/{d['n']} p_plus_one={d['p_plus_one']:.9g}")
