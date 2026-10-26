#!/usr/bin/env python3
"""Audit worker stream separation and short/unequal OpenMP work allocation.

Uses only tiny correctness runs and reads their counts/metadata, never feature
frequencies, target scores, or inferential study outcomes.
"""

import hashlib
import json
import os
import pathlib
import struct
import subprocess
import tempfile


MASK64 = (1 << 64) - 1
INCREMENT = 0x9E3779B97F4A7C15
WORKER_MIX = 0xD1B54A32D192ED03


def splitmix_output(initial_state):
    z = (initial_state + INCREMENT) & MASK64
    z = ((z ^ (z >> 30)) * 0xBF58476D1CE4E5B9) & MASK64
    z = ((z ^ (z >> 27)) * 0x94D049BB133111EB) & MASK64
    return z ^ (z >> 31)


def worker_seed(master, worker):
    return splitmix_output(master ^ ((WORKER_MIX * (worker + 1)) & MASK64))


def main():
    root = pathlib.Path(__file__).resolve().parents[1]
    source = root / "src" / "study_engine.cpp"
    before_hash = hashlib.sha256(source.read_bytes()).hexdigest()
    protocol = json.loads((root / "protocol.json").read_text())
    masters = protocol["training_seeds"] + protocol["testing_seeds"]
    seeds = [worker_seed(master, worker) for master in masters for worker in range(8)]
    assert len(set(seeds)) == 48
    inverse_increment = pow(INCREMENT, -1, 1 << 64)
    positions = sorted((seed * inverse_increment) & MASK64 for seed in seeds)
    gaps = [(positions[(j + 1) % len(positions)] - positions[j]) & MASK64
            for j in range(len(positions))]
    minimum_gap = min(gaps)
    assert minimum_gap > 1_000_000_000_000
    output_windows = set()
    for seed in seeds:
        for offset in range(1024):
            output = splitmix_output((seed + INCREMENT * offset) & MASK64)
            assert output not in output_windows
            output_windows.add(output)

    # Independently demonstrate the corrected defect in the retired rule.
    master = masters[0]
    old0 = (master + INCREMENT) & MASK64
    old1 = (master + 2 * INCREMENT) & MASK64
    assert splitmix_output((old0 + INCREMENT) & MASK64) == splitmix_output(old1)

    cases = [
        {"phase": "bench", "model": "uniform", "n": 1, "limit": None, "dynamic": "TRUE"},
        {"phase": "bench", "model": "uniform", "n": 17, "limit": None, "dynamic": "FALSE"},
        {"phase": "bench", "model": "block6", "n": 3, "limit": 2, "dynamic": "TRUE"},
        {"phase": "train", "model": "uniform", "n": 5, "limit": 2, "dynamic": "FALSE"},
        {"phase": "bench", "model": "inversion_exact", "n": 3, "limit": 2, "dynamic": "FALSE"},
    ]
    reports = []
    with tempfile.TemporaryDirectory(prefix="joint-worker-seed-check-") as temp:
        temp = pathlib.Path(temp)
        binary = temp / "study_engine"
        compile_command = ["g++", "-std=c++17", "-O2", "-fopenmp", "-Wall", "-Wextra", "-pedantic",
                           str(source), "-o", str(binary)]
        compiled = subprocess.run(compile_command, check=True, capture_output=True, text=True)
        assert compiled.stderr == "", compiled.stderr
        for index, case in enumerate(cases):
            prefix = temp / f"case_{index}"
            seed = 765432100 + index
            environment = os.environ.copy()
            environment.pop("OMP_THREAD_LIMIT", None)
            environment["OMP_DYNAMIC"] = case["dynamic"]
            if case["limit"] is not None:
                environment["OMP_THREAD_LIMIT"] = str(case["limit"])
            command = [str(binary), "--phase", case["phase"], "--model", case["model"],
                       "--n", str(case["n"]), "--threads", "8", "--seed", str(seed),
                       "--counts", str(root / "data" / "counts.txt"), "--out", str(prefix)]
            subprocess.run(command, env=environment, check=True, capture_output=True, text=True)
            record = json.loads(pathlib.Path(str(prefix) + "_run.json").read_text())
            assert record["n"] == record["processed"] == case["n"]
            assert record["requested_threads"] == 8
            assert 1 <= record["threads"] <= 8
            if case["limit"] is not None:
                assert record["threads"] <= case["limit"]
            assert len(record["worker_seeds"]) == record["threads"]
            assert record["worker_seeds"] == [worker_seed(seed, tid) for tid in range(record["threads"])]
            if case["model"] != "inversion_exact":
                assert record["proposals"] == case["n"]
            else:
                assert record["proposals"] >= case["n"]
            if case["phase"] == "train":
                with pathlib.Path(str(prefix) + ".bin").open("rb") as handle:
                    actual_n, actual_proposals = struct.unpack("=QQ", handle.read(16))
                assert actual_n == actual_proposals == case["n"]
            reports.append({"case": case, "metadata": record})
    after_hash = hashlib.sha256(source.read_bytes()).hexdigest()
    assert before_hash == after_hash, "Source changed during validation"
    report = {
        "status": "pass",
        "scope": "Worker seeds, work counts, and tiny-run metadata only; no feature frequencies or study scores inspected.",
        "source_sha256": before_hash,
        "distinct_planned_worker_seeds": len(set(seeds)),
        "first_outputs_checked_without_overlap": len(output_windows),
        "minimum_circular_splitmix_counter_gap": minimum_gap,
        "old_shifted_stream_defect_reproduced": True,
        "correctness_runs": reports,
        "note": "Mixing removes the deterministic adjacent-stream reuse defect. This is not a proof of mathematical independence of a finite PRNG.",
    }
    (root / "review" / "seed_validation.json").write_text(json.dumps(report, indent=2) + "\n")
    print("PASS: 48 distinct planned worker seeds; 49,152 checked outputs without stream overlap")
    print(f"PASS: minimum circular stream-counter separation = {minimum_gap}")
    print("PASS: five tiny work-allocation runs, including n<threads, uneven n, and OMP_THREAD_LIMIT=2")
    print("PASS: independent seed reconstruction and actual training record count")


if __name__ == "__main__":
    main()
