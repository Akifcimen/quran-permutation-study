#!/usr/bin/env python3
"""Small deterministic validation, with exhaustive trace/determinant coverage.

Builds a temporary C++ driver and compares it with independent Python algebra.
Reads no Monte Carlo study outcomes and does not generate study null samples.
"""

import itertools
import pathlib
import random
import subprocess
import tempfile

from matrix_reference import (PRIMES, canonical_view, companion_maxorder_polynomial,
                              eval8, flags, matrix_order)


def main():
    src = pathlib.Path(__file__).resolve().parent
    with tempfile.TemporaryDirectory(prefix="joint-matrix-check-") as temp:
        driver = pathlib.Path(temp) / "matrix_driver"
        subprocess.run(["g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic",
                        str(src / "matrix_driver.cpp"), "-o", str(driver)], check=True)

        table_text = subprocess.check_output([str(driver), "--tables"], text=True)
        table_entries = 0
        for line in table_text.splitlines():
            p_text, bits = line.split()
            p = int(p_text)
            assert len(bits) == p * p
            for t in range(p):
                for d in range(p):
                    expected = companion_maxorder_polynomial(t, d, p)
                    assert (bits[t * p + d] == "1") == expected, (p, t, d)
                    table_entries += 1
        assert len(table_text.splitlines()) == len(PRIMES)

        observed_p = (3303, 2931, 3252, 3303)
        observed_r = (590, 854, 5965, 5380)
        assert matrix_order(observed_p, 19) == 360
        assert matrix_order(observed_r, 19) == 360
        assert flags(observed_p, observed_r, 19) == 15

        rng = random.Random(2026102501)
        cases = [(19, observed_p, observed_r),
                 (19, (0, 0, 0, 0), (1, 0, 0, 1)),
                 (19, (1, 1, 0, 1), (1, 0, 1, 1)),
                 (3, (1, 0, 0, 1), (1, 0, 0, 1))]
        for p in PRIMES:
            for _ in range(2):
                a = tuple(rng.randint(-10000, 10000) for _ in range(4))
                b = tuple(rng.randint(-10000, 10000) for _ in range(4))
                cases.append((p, a, b))
        inputs = "".join(" ".join(map(str, (p, *a, *b))) + "\n" for p, a, b in cases)
        outputs = subprocess.check_output([str(driver)], input=inputs, text=True).splitlines()
        assert len(outputs) == len(cases)
        view_checks = 0
        for (p, a, b), output in zip(cases, outputs):
            packed = int(output)
            expected = eval8(a, b, p)
            assert packed == expected, (p, a, b, packed, expected)
            for pr, pc, rr, rc in itertools.product(range(2), repeat=4):
                index = canonical_view(pr, pc, rr, rc)
                actual = (packed >> (4 * index)) & 15
                expected_flags = flags(a, b, p, pr, pc, rr, rc)
                assert actual == expected_flags, (p, pr, pc, rr, rc, actual, expected_flags)
                view_checks += 1

    print(f"PASS: {table_entries} trace/determinant entries across {len(PRIMES)} primes")
    print(f"PASS: {len(cases)} exact matrix-pair comparisons")
    print(f"PASS: {view_checks} direct 16-view-to-8-view comparisons")
    print("PASS: observed P, R and PR+RP have order 360; commutator square is identity")


if __name__ == "__main__":
    main()
