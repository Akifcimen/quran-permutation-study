#!/usr/bin/env python3
"""Reproducible lexical-numeral inventory of the complete Tanzil Arabic Quran.

The source text is retained verbatim. Normalization is used only for matching.
This deliberately does not infer numbers from verse addresses, grammatical dual
nouns, unnumbered lists, letter values, or arithmetic between separate phrases.
"""
from __future__ import annotations

import csv
import hashlib
import json
import re
from collections import Counter
from fractions import Fraction
from pathlib import Path

BASE = Path(__file__).resolve().parent
P = [n for n in range(3, 114, 2) if all(n % k for k in range(2, int(n**0.5) + 1))]

CARDINAL = {
    "واحد": 1, "واحدا": 1, "واحدة": 1,
    "اثنان": 2, "اثنين": 2, "اثنتان": 2, "اثنتين": 2,
    "اثنا": 2, "اثني": 2, "اثنتا": 2, "اثنتي": 2, "ثنتين": 2,
    "ثلاث": 3, "ثلاثة": 3, "ثلاثا": 3,
    "اربع": 4, "اربعة": 4, "اربعا": 4,
    "خمس": 5, "خمسة": 5, "خمسا": 5,
    "ست": 6, "ستة": 6, "ستا": 6,
    "سبع": 7, "سبعة": 7, "سبعا": 7,
    "ثمان": 8, "ثماني": 8, "ثمانية": 8, "ثمانيا": 8,
    "تسع": 9, "تسعة": 9, "تسعا": 9,
    "عشر": 10, "عشرة": 10, "عشرا": 10,
    "عشرون": 20, "عشرين": 20,
    "ثلاثون": 30, "ثلاثين": 30,
    "اربعون": 40, "اربعين": 40,
    "خمسون": 50, "خمسين": 50,
    "ستون": 60, "ستين": 60,
    "سبعون": 70, "سبعين": 70,
    "ثمانون": 80, "ثمانين": 80,
    "تسعون": 90, "تسعين": 90,
    "مائة": 100, "مئة": 100,
    "مائتان": 200, "مائتين": 200, "مئتان": 200, "مئتين": 200,
    "ثلاثمائة": 300, "ثلاثمئة": 300,
    "الف": 1000, "الفا": 1000, "الاف": 1000,
    "الفان": 2000, "الفين": 2000,
}
DISTRIBUTIVE = {"مثني": 2, "رباع": 4}
FRACTION = {
    "نصف": "1/2", "نصفه": "1/2", "نصفها": "1/2",
    "ثلث": "1/3", "ثلثه": "1/3", "ثلثها": "1/3",
    "ثلثا": "2/3", "ثلثي": "2/3", "ثلثان": "2/3",
    "ربع": "1/4", "ربعه": "1/4",
    "خمسه": "1/5", "سدس": "1/6", "سدسه": "1/6",
    "معشار": "1/10", "معشاره": "1/10",
}
ORDINAL = {
    "اول": 1, "اولا": 1, "اولي": 1,
    "ثاني": 2, "ثالث": 3, "ثالثة": 3,
    "رابع": 4, "رابعهم": 4,
    "خامس": 5, "خامسة": 5,
    "سادس": 6, "سادسهم": 6,
    "سابع": 7, "ثامن": 8, "ثامنهم": 8,
}
VAGUE = {"بضع", "بضعة", "الوف"}

def norm(s: str) -> str:
    s = re.sub(r"[\u064b-\u065f\u0670\u06d6-\u06ed\u0640]", "", s)
    return s.translate(str.maketrans({"أ": "ا", "إ": "ا", "آ": "ا", "ٱ": "ا", "ى": "ي"}))

def forms(token: str):
    """Enumerate conservative proclitic/article removal, shortest first."""
    seen = set()
    frontier = [(norm(token), "")]
    for _ in range(4):
        nxt = []
        for word, prefix in frontier:
            if (word, prefix) in seen:
                continue
            seen.add((word, prefix))
            yield word, prefix
            if len(word) > 2 and word[0] in "وفبكل":
                nxt.append((word[1:], prefix + word[0]))
            if word.startswith("ال") and len(word) > 3:
                nxt.append((word[2:], prefix + "ال"))
            # li + definite article is written لل rather than لال.
            if word.startswith("لل") and len(word) > 3:
                nxt.append((word[2:], prefix + "لل"))
        frontier = nxt

def component(token: str):
    for word, prefix in forms(token):
        if word in CARDINAL:
            return {"word": word, "prefix": prefix, "value": CARDINAL[word], "kind": "cardinal"}
        if word in {"احد", "احدي"}:
            return {"word": word, "prefix": prefix, "value": 1, "kind": "potential_one"}
    return None

def load_source(name: str):
    data = {}
    for line in (BASE / name).read_text(encoding="utf-8-sig").splitlines():
        if re.match(r"^\d+\|\d+\|", line):
            s, a, text = line.split("|", 2)
            data[(int(s), int(a))] = text
    assert len(data) == 6236, f"Unexpected corpus size: {len(data)}"
    assert len({s for s, _ in data}) == 114
    return data

def classify_status(ref: str, kind: str, value):
    if kind in {"indefinite_or_referential_one", "ambiguous_ordinal", "vague_quantity", "non_numeric_homograph"}:
        return "not_a_primary_exact_cardinal"
    if ref == "18:22":
        return "reported_competing_claim"
    if ref in {"4:171", "5:73"} and str(value) == "3":
        return "rejected_theological_claim"
    if ref in {"12:4", "12:43", "12:46"}:
        return "reported_dream_or_dream_restatement"
    if ref in {"2:259", "18:19", "20:103", "20:104", "23:113"}:
        return "reported_duration_or_correction_review_context"
    return "in_text_statement_no_fine_speech_act_claim"

def parse():
    simple = load_source("quran_simple-clean.txt")
    uthmani = load_source("quran_uthmani.txt")
    assert simple.keys() == uthmani.keys()
    rows = []
    for (sura, verse), text in simple.items():
        ref = f"{sura}:{verse}"
        tokens = list(re.finditer(r"\S+", text))
        i = 0
        while i < len(tokens):
            raw = tokens[i].group()
            comp = component(raw)
            selected = None
            used = 1
            rule = ""
            notes = []
            parts = []
            # Longest grammatical number phrase first; adjacent components of
            # 11,12,19,99,300,3000,5000,50000,100000 are not separate mentions.
            if comp and i + 1 < len(tokens):
                nxt = component(tokens[i + 1].group())
                if nxt:
                    a, b = comp["value"], nxt["value"]
                    if 1 <= a <= 9 and b == 10 and not nxt["prefix"]:
                        selected = (a + b, "cardinal")
                        used, rule, parts = 2, "compound_teen", [a, b]
                    elif 1 <= a <= 9 and b in range(20, 100, 10) and nxt["prefix"] == "و":
                        selected = (a + b, "cardinal")
                        used, rule, parts = 2, "compound_unit_and_tens", [a, b]
                    elif a >= 2 and b in {100, 1000} and not nxt["prefix"]:
                        selected = (a * b, "cardinal")
                        used, rule, parts = 2, "compound_times_scale", [a, b]
            if selected is None and comp:
                value, kind = comp["value"], comp["kind"]
                if kind == "potential_one":
                    if ref == "112:1" and comp["word"] == "احد":
                        selected = (1, "cardinal")
                        rule = "explicit_oneness_112_1"
                    else:
                        selected = (1, "indefinite_or_referential_one")
                        rule = "one_anyone_pronoun_not_primary_cardinal"
                elif ref in {"3:103", "8:63"} and comp["word"] == "الف":
                    selected = (None, "non_numeric_homograph")
                    rule = "united_hearts_verb_not_thousand"
                elif norm(raw) in {"لست", "الست", "وبست"} and comp["word"] == "ست":
                    selected = (None, "non_numeric_homograph")
                    rule = "negative_copula_or_crushed_verb_not_six"
                elif ref == "5:3" and comp["word"] == "سبع":
                    selected = (None, "non_numeric_homograph")
                    rule = "wild_beast_not_seven"
                else:
                    selected = (value, kind)
                    rule = "lexical_cardinal"
                    if ref in {"4:3", "35:1"} and value == 3:
                        selected = (3, "distributive")
                        rule = "explicit_distributive_threes"
            if selected is None:
                for word, prefix in forms(raw):
                    if word in DISTRIBUTIVE:
                        selected = (DISTRIBUTIVE[word], "distributive")
                        rule = "explicit_distributive_numeral"
                        break
                    if word in FRACTION:
                        selected = (FRACTION[word], "fraction")
                        rule = "lexical_fraction"
                        break
                    if word == "ثمن" and ref == "4:12":
                        selected = ("1/8", "fraction")
                        rule = "inheritance_eighth_not_price"
                        break
                    if word in ORDINAL:
                        if word == "ثاني" and ref == "22:9":
                            selected = (None, "non_numeric_homograph")
                            rule = "turning_aside_not_second"
                        elif word == "اولي":
                            # أولى may be first, worthier, or possessors after
                            # orthographic normalization. It is never used as
                            # a primary exact cardinal in this inventory.
                            selected = (1, "ambiguous_ordinal")
                            rule = "awla_ula_ambiguous_kept_for_audit"
                        else:
                            selected = (ORDINAL[word], "ordinal")
                            rule = "lexical_ordinal"
                        break
                    if word in VAGUE:
                        selected = (None, "vague_quantity")
                        rule = "no_single_exact_integer"
                        break
                    if re.fullmatch(r"احد(?:ا|ي)?(?:هم|هما|هن|كم|كن|نا)?", word):
                        selected = (1, "indefinite_or_referential_one")
                        rule = "one_anyone_pronoun_not_primary_cardinal"
                        break
            if selected is not None:
                value, kind = selected
                start, end = tokens[i].start(), tokens[i + used - 1].end()
                primary = kind in {"cardinal", "distributive"}
                rows.append({
                    "mention_id": f"{sura:03d}:{verse:03d}:{start:04d}",
                    "surah": sura, "verse": verse, "verse_key": ref,
                    "token_start": i, "token_end_exclusive": i + used,
                    "char_start": start, "char_end_exclusive": end,
                    "arabic_phrase_simple": text[start:end],
                    "value": value, "kind": kind,
                    "primary_literal_integer": primary,
                    "eligible_odd_prime_under_114": primary and value in P,
                    "parse_rule": rule, "compound_components_not_separate_mentions": parts,
                    "status": classify_status(ref, kind, value),
                    "status_scope": "Reported/rejected/dream flags explicitly annotated for relevant passages; remaining label deliberately does not assert fine-grained speech act.",
                    "arabic_context_simple": text,
                    "arabic_context_uthmani": uthmani[(sura, verse)],
                    "quran_com_url": f"https://quran.com/{sura}/{verse}",
                    "tanzil_url": f"https://tanzil.net/#{sura}:{verse}",
                    "source_attribution": "Arabic contexts reproduced verbatim from Tanzil Quran Text, Version 1.1, Copyright (C) 2007-2026 Tanzil Project, CC BY 3.0. Source https://tanzil.net/ ; original text may not be changed. Full notice accompanies source files and SOURCE_NOTICE.txt.",
                    "notes": notes,
                })
            i += used
    return rows, simple

def dump_csv(path: Path, rows):
    if not rows:
        return
    with path.open("w", encoding="utf-8-sig", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=list(rows[0]))
        writer.writeheader()
        for row in rows:
            writer.writerow({k: json.dumps(v, ensure_ascii=False) if isinstance(v, (list, dict)) else v for k, v in row.items()})

def main():
    rows, verses = parse()
    cardinal = [r for r in rows if r["primary_literal_integer"]]
    prime = [r for r in rows if r["eligible_odd_prime_under_114"]]
    cardinal_counts = Counter(r["value"] for r in cardinal)
    prime_counts = Counter(r["value"] for r in prime)
    expected = {3, 5, 7, 11, 19}
    assert set(prime_counts) == expected, prime_counts
    # Known lexical ambiguity and composition checks, independent of any
    # subsequent numerical-structure simulation.
    assert not any(r["verse_key"] == "5:3" and r["value"] == 7 for r in cardinal)
    assert [r["value"] for r in cardinal if r["verse_key"] == "74:30"] == [19]
    assert [r["value"] for r in cardinal if r["verse_key"] == "12:4"] == [11]
    assert [r["value"] for r in cardinal if r["verse_key"] == "18:25"] == [300, 9]
    assert [r["value"] for r in cardinal if r["verse_key"] == "38:23"] == [99, 1]
    assert not any(r["verse_key"] in {"3:103", "8:63"} and r["value"] == 1000 for r in cardinal)
    assert sum(r["value"] == 6 for r in cardinal) == 7
    assert sum(r["value"] == 1000 for r in cardinal) == 8
    for name, data in [("inventory", rows), ("literal_integer_inventory", cardinal), ("eligible_prime_inventory", prime)]:
        (BASE / f"{name}.json").write_text(json.dumps(data, ensure_ascii=False, indent=2), encoding="utf-8")
        dump_csv(BASE / f"{name}.csv", data)
    summary = {
        "corpus_verses": len(verses), "corpus_surahs": 114,
        "source_verse_total": len(verses),
        "all_lexical_audit_rows": len(rows),
        "row_kinds": dict(Counter(r["kind"] for r in rows)),
        "primary_literal_integer_mentions": len(cardinal),
        "primary_literal_integer_distinct_values": sorted(cardinal_counts),
        "primary_literal_integer_frequency": dict(sorted(cardinal_counts.items())),
        "eligible_prime_universe": P,
        "literal_eligible_prime_values": sorted(prime_counts),
        "literal_eligible_prime_mention_count": sum(prime_counts.values()),
        "literal_eligible_prime_frequency": dict(sorted(prime_counts.items())),
        "literal_uniform_distinct_value_weights": {str(p): (1 / len(prime_counts) if p in prime_counts else 0) for p in P},
        "literal_frequency_weights_sensitivity_only": {str(p): prime_counts[p] / sum(prime_counts.values()) for p in P},
        "prime_values_by_verse": {str(p): sorted({r["verse_key"] for r in prime if r["value"] == p}, key=lambda x: tuple(map(int, x.split(":")))) for p in sorted(prime_counts)},
        "source_hashes": {n: hashlib.sha256((BASE / n).read_bytes()).hexdigest() for n in ["quran_simple-clean.txt", "quran_uthmani.txt"]},
        "coverage_claim": "Whole-corpus scan of explicit lexical cardinal numerals and distributive numerals, with grammatical compounds; ordinal, fraction, indefinite-one, vague and specified homograph hits retained separately. Does not count implicit dual nouns, unnumbered lists, inferred totals or every semantic expression of quantity. Fine speech-act annotation is intentionally partial.",
    }
    (BASE / "numeral_summary.json").write_text(json.dumps(summary, ensure_ascii=False, indent=2), encoding="utf-8")
    print(json.dumps({k: summary[k] for k in ["corpus_verses", "all_lexical_audit_rows", "row_kinds", "primary_literal_integer_mentions", "primary_literal_integer_distinct_values", "literal_eligible_prime_values", "literal_eligible_prime_frequency"]}, ensure_ascii=False, indent=2))

if __name__ == "__main__":
    main()
