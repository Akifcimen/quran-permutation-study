# Whole-corpus Arabic numeral inventory and semantic anchors

This directory contains a reproducible lexical scan of all 6,236 verses in the
114-sura Tanzil Arabic Quran, together with an independent model-assisted review
of every eligible prime numeral context. It does not contain numerical-structure
simulation results.

## Frozen results

The eligible candidate domain is the 29 odd primes from 3 through 113. The literal
prime values actually present as explicit cardinal or distributive numerals are
**3, 5, 7, 11 and 19**. Uniform weights across distinct values are **1/5 each**.

The generic explicit-number-discussion rule returns **3, 5, 7 and 19**, with
uniform weights **1/4 each**. A broader rule admitting signs, dreams and symbolic
or epistemic contexts also admits 11, returning the full five-value set. The
broader branch is therefore a sensitivity analysis rather than an additional
independent source of evidence.

| Value | Literal mentions | Notes |
| ---: | ---: | --- |
| 3 | 19 | Includes distributive threes at 4:3 and 35:1. |
| 5 | 2 | 18:22 and 58:7. |
| 7 | 24 | Repeated occurrences within a verse are counted separately. |
| 11 | 1 | The composed numeral at 12:4. |
| 19 | 1 | The composed numeral at 74:30. |
| Total | 47 | 38 different verse contexts. |

Excluding distributive forms changes the count of 3 to 17, without changing any
distinct-value candidate set. Frequency weights are provided only as a declared
sensitivity analysis; the default does not turn repeated mentions into separate
independent anchors. Under frequency weighting 19 has weight 1/47, not a special
privilege derived from a subjective importance score.

Across the broader lexical inventory there are **207 explicit integer mentions**
(200 ordinary cardinals and 7 distributives), representing **30 distinct integer
values**. These figures are conditional on the lexical scope below, especially
the treatment of one, compound numerals and grammatical duals.

## Source and provenance

Both original text files were downloaded directly from the primary source,
Tanzil Project, using `https://tanzil.net/pub/download/index.php` with a POST form.
`source_metadata.json` records the exact parameters, retrieval time, sizes and
SHA-256 hashes. The source identifies itself as **Version 1.1**, copyright
2007–2026 Tanzil Project. The Simple Clean and Uthmani files are retained verbatim,
including the source copyright and terms block. See `SOURCE_NOTICE.txt`.

The source is a 6,236-verse Hafs numbering representation with the basmala included
in the first-verse text of most suras. No number in the basmala affects this
inventory. Sura/verse addresses are provenance keys only; they are never counted
as numeral occurrences or used to generate the semantic candidate values.

Selected substantive passages were also checked through Quran.com and Diyanet.
Their URLs, reviewed Arabic contexts and rationales are in `salience_audit.json`.

## What is and is not counted

1. Match explicit lexical Arabic cardinal numerals, including distributive forms
   such as twos, threes and fours. Arabic matching normalizes orthographic variants
   internally, but displayed Arabic phrases and context are copied verbatim.
2. Parse a grammatical number phrase as its whole amount: 19, 99, 300, 3,000,
   5,000, 50,000 and 100,000 are not additional independent occurrences of their
   component numbers. For example, 18:25 has 300 and 9, not 3 and not a literal309;
   29:14 has 1,000 and 50, not a literal950.
3. Preserve rejected and reported numerical claims as literal mentions and label
   their status. Do not remove competing values from 18:22 merely because they
   are presented as claims whose knowledge is discussed.
4. Keep ordinal and fractional forms separately. A fifth does not create a
   cardinal5 occurrence, and one third does not create a cardinal3 occurrence.
5. Treat `واحد` forms and the explicit oneness statement at112:1 as cardinal1.
   The indefinite/referential `أحد` family is retained in the audit separately;
   it does not automatically mean an explicitly asserted quantity1.
6. Exclude implicit grammatical dual nouns (two days, two months, two hands),
   numbers inferred from an unnumbered list, verse addresses, letter values and
   arithmetic totals not stated as numeral expressions. Vague quantities such as
   several or thousands have no exact integer value.

The parser explicitly disambiguates the wild-beast homograph at5:3 from seven;
the unite-hearts verbs at3:103 and8:63 from a thousand; negative-copula/crushed
verbs from six; and the turning-aside expression at22:9 from second. A full
unmatched-root audit was inspected to catch omitted numeral inflections. These
are lexical and grammatical choices, not decisions informed by simulation scores.

This is a whole-corpus inventory of the stated lexical class, **not a claim to
have exhausted every possible semantic expression of quantity in Arabic**.
Ordinal/ambiguous-ordinal rows are an audit convenience rather than a fully
disambiguated grammatical corpus. They are excluded from the prime-anchor rules.

## Semantic annotation protocol

The core number-discussion tier covers explicit addition/subtraction/completion,
debate or correction of a number, knowledge of a number, a count relation with a
generalization, enumeration giving a count rationale, or an explicit explanation
of the function of a stated count. It was deliberately not limited to the exact
wording about test, certainty and belief at74:31.

Clear core examples include:

- 2:196: three and seven explicitly total ten.
- 4:171 and5:73: a numerical theological claim involving three is rejected and
  contrasted with one; the claim status is retained.
- 18:22: three, five and seven occur in competing claims followed by explicit
  discussion of knowledge of their number.
- 24:58: three times are enumerated and repeated as three privacy occasions.
- 58:7: three/fourth and five/sixth are followed by a smaller/larger generalization.
- 74:30–31: nineteen is followed by an explanation referring to their number.

All47 eligible-prime mentions were reviewed independently from the parser.
Broader or borderline cases are not silently forced into the core: the seven
ears with100 grains each, symbolic mappings in the seven-valued dream, and seven
additional seas receive separately recorded tiers. Their inclusion or exclusion
does not change the core distinct-value set. Eleven at12:4 belongs to the broader
dream/symbolic tier; no local metacomment explains the count11 itself.

The semantic file also records38 additional groups of nonprime, ordinal,
fractional or dual contexts, including30+10=40,300+9,1,000−50, alternatives8/10,
12months with4sacred, and fractions of the night. This nonprime review is an audit
list, not an exhaustive catalogue of every conceivable salience interpretation.

The parser and independent review initially differed because a simple reviewer
scan missed definite-article forms at9:118,17:44 and23:86. Direct comparison found
and corrected the omissions. The final47 occurrences agree exactly. The values
and semantic weight sets were unaffected.

## Selection history and statistical limitations

The observed19 hypothesis was already known. The semantic team did not inspect
Monte Carlo outcomes while choosing or freezing these weights. This is
model-assisted annotation, **not a blinded human panel or a retroactive
preregistration of the original discovery**. Timestamps and selection history
are recorded in `semantic_weights.json` and the audit.

These weights can be held fixed under a null that permutes whole suras while
keeping their content together. They support a conditional test of alignment
between textual anchors and ordering-dependent numerical structure. They are
not an independent probability that the text mentions19; multiplying an existing
numerical p-value by1/5 or1/4 would not be justified by this inventory.

The127-verse sensitivity variant removes9:128–129 from the numeric input. Those
verses contain no primary literal numeral; the semantic candidate sets and
weights are therefore identical. The actual source files here are preserved
unchanged and use the conventional129-verse ninth sura.

## Files and reproduction

- `quran_simple-clean.txt`, `quran_uthmani.txt`: unchanged original source files.
- `source_metadata.json`, `SOURCE_NOTICE.txt`: provenance and source terms.
- `parser.py`: number lexicon, composition, ambiguity rules and meaningful checks.
- `build_semantic_outputs.py`: regenerates tables, verifies independent review
  agreement and joins annotations; refuses a silent change to frozen weights.
- `inventory.csv/json`: all lexical audit rows, including excluded kinds.
- `literal_integer_inventory.csv/json`:207 primary integer mentions.
- `eligible_prime_inventory.csv/json`:47 fully reviewed eligible prime mentions.
- `numeral_summary.json`: counts, occurrence locations and lexical scope.
- `salience_audit.json`: all prime contexts plus additional nonprime review,
  rationales, tiers, source spotchecks and correction history.
- `semantic_weights.csv/json`: normalized weights for all29 candidate primes.
- `lexical_candidates.tsv`: broad lexical retrieval used during manual auditing;
  it includes false positives and must not be treated as the final inventory.

Reproduce from the retained sources with Python3 and only the standard library:

```bash
python semantic/build_semantic_outputs.py
```

Running `parser.py` alone regenerates the lexical tables without the semantic
annotation join. Run the builder afterward for the complete deliverable.

Source attribution: Tanzil Quran Text, Version1.1, Copyright(C)2007–2026 Tanzil
Project, Creative Commons Attribution3.0. https://tanzil.net/ . Original Arabic
text is reproduced verbatim; no source-text edits were made. Full source terms
are retained in the original files and `SOURCE_NOTICE.txt`.
