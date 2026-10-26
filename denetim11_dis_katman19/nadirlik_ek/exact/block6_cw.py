#!/usr/bin/env python3
"""Count natural C=15 and W=3 exactly; export an exact conditional sampler.

Integer DP state = (C coefficient state, remaining H count, H_N-F_N mod19).
Overlap F/H edges are excluded locally. Both variants have identical F/H
events under the frozen block6 model. Counted space consists of labeled orders.
"""
from __future__ import annotations
import argparse
from collections import defaultdict
from fractions import Fraction
from functools import lru_cache
import json
from pathlib import Path
import struct
import time
from block6_core import ConditionalCore, ROOT, TARGETS


class ConditionalCW:
    def __init__(self, core):
        self.core = core
        export = core.export()
        raw_nodes = export["states"]
        self.node_index = {(x["block"], tuple(x["remaining"])): i
                           for i, x in enumerate(raw_nodes)}
        self.terminal = len(raw_nodes)
        self.roots = [self.node_index[(0, t)] for t in TARGETS]
        self.nodes = []
        for x in raw_nodes:
            bi = x["block"]
            edges = []
            for choice in x["choices"]:
                child = self.terminal if bi == 18 else self.node_index[(bi + 1, tuple(choice["remaining"]))]
                groups = defaultdict(list)
                for index in choice["indexes"]:
                    profile = core.local_profiles[bi][index]
                    if not profile["overlap"]:
                        groups[(profile["hcount"], profile["h_minus_f_mod19"])].append(index)
                for (h, d), indexes in sorted(groups.items()):
                    edges.append((child, h, d, tuple(indexes)))
            self.nodes.append((bi, edges))

    @lru_cache(None)
    def count(self, node, hleft, residue):
        if hleft < 0:
            return 0
        if node == self.terminal:
            return int(hleft == 0 and residue == 0)
        total = 0
        for child, h, d, indexes in self.nodes[node][1]:
            if h <= hleft:
                total += len(indexes) * self.count(child, hleft - h, (residue - d) % 19)
        return total

    @property
    def total(self):
        return sum(self.count(root, 7, 0) for root in self.roots)

    def export_binary(self, path):
        # All integers little-endian. Counts are unsigned 256-bit. This is
        # sufficient because 720^19 has fewer than 256 bits.
        def u32(fh, *values):
            fh.write(struct.pack("<" + "I" * len(values), *values))
        with path.open("wb") as fh:
            fh.write(b"BLK6CW01")
            u32(fh, len(self.nodes), self.terminal, len(self.roots), *self.roots)
            fh.write(self.total.to_bytes(32, "little"))
            for bi, profiles in enumerate(self.core.local_profiles):
                u32(fh, len(profiles))
                for p in profiles:
                    u32(fh, p["ec"], p["es_half_129"], p["en_mod19"],
                        p["fn_mod19"], p["hcount"], p["h_minus_f_mod19"],
                        p["overlap"], p["edit_in_E"], *p["ids"])
            for bi, edges in self.nodes:
                u32(fh, bi, len(edges))
                for child, h, d, indexes in edges:
                    u32(fh, child, h, d, len(indexes), *indexes)
            for node in range(self.terminal + 1):
                for hleft in range(8):
                    for residue in range(19):
                        fh.write(self.count(node, hleft, residue).to_bytes(32, "little"))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", type=Path, default=Path(__file__).resolve().parent)
    args = ap.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    start = time.perf_counter()
    base = list(map(int, (ROOT / "data" / "counts.txt").read_text().split()))
    core = ConditionalCore(base)
    cw = ConditionalCW(core)
    count = cw.total
    prob = Fraction(count, 720 ** 19)
    cond = Fraction(count, core.count)
    result = {"event": "Natural C=15 AND W=3, alpha=beta=1, prime19, block6",
              "is_full_score_tail_probability": False,
              "count": str(count), "space": str(720 ** 19),
              "reduced_numerator": str(prob.numerator),
              "reduced_denominator": str(prob.denominator),
              "probability": float(prob),
              "W_given_C_probability": float(cond),
              "W_given_C_numerator": str(cond.numerator),
              "W_given_C_denominator": str(cond.denominator),
              "target_counts": [str(cw.count(r, 7, 0)) for r in cw.roots],
              "cache_before_export": cw.count.cache_info()._asdict(),
              "positive_C_nodes": len(cw.nodes),
              "local_group_edges": sum(len(x[1]) for x in cw.nodes),
              "both_variants_identical_C_and_W": True}
    cw.export_binary(args.out / "block6_CW_sampler.bin")
    result["cache_after_export"] = cw.count.cache_info()._asdict()
    result["seconds"] = time.perf_counter() - start
    (args.out / "block6_CW_exact.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
