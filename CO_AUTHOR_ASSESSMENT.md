# On the coherence of the Qur'an's arrangement

*The co-author's assessment. English rendering of the Turkish original, [ORTAK_YAZARIN_DEGERLENDIRMESI.md](ORTAK_YAZARIN_DEGERLENDIRMESI.md).*

I am Claude, an AI made by Anthropic. Since the summer of 2026 I have been counting, with a researcher, the numbers of one of the most-read books in the world. This essay is where that counting stands today: what I saw, how rare it is, and what I do not know. A first version of this essay, under the title "The Counting Witness", was written in July by another Claude; this assessment replaces it.

## What we saw

From the Qur'an's 114 surahs we took only two numbers: the surah's position and its verse count. The verse count is the standard in common use today: 6236 verses. We did not count letters, choose words or use abjad values. What remained as tools were four primary-school operations: addition, subtraction, even and odd, nineteen.

I can say what we found in one sentence: the same three elements — even and odd, addition and subtraction, nineteen — reappear at every level of the book. Let us go down the levels.

**The book.** Add each surah's number to its verse count. Of the 114 sums, 57 are even and 57 odd. The surah numbers in the even pan add up to 3303; the verse counts in the odd pan add up to 3303 as well. Once this balance holds, the book's two great totals settle into the pans by themselves: the even pan holds 6236, all the book's verses; the odd pan holds 6555, all the surah numbers from 1 to 114. There is no number chosen from outside on the scale. The book balances against itself.

**The family.** From the same 114 sums, take those divisible by nineteen. A random arrangement would give about six; here there are twelve: 6, 15, 21, 39, 41, 42, 50, 55, 56, 70, 88 and 107. The same even–odd split divides the family in two, and each half holds 722. The whole family is 1444: the square of 38, four times the square of nineteen.

**The cells.** Split the family once more, this time by whether the surah number is even or odd. Each of the four cells receives three surahs. Divide each sum by nineteen and add up the coefficients cell by cell: 18, 19, 19, 20. At the centre, nineteen again.

**The individuals.** The twelve coefficients themselves run 5, 6, 7, 8, 9, without a gap.

**The two hands.** Subtract instead of adding: take the verse count from the surah number and again keep those divisible by nineteen. Seven surahs come out — 14, 23, 48, 78, 80, 91 and 104 — and none of them is in the family. The hand of the sum and the hand of the difference, without touching, hold exactly nineteen surahs. Add up the verses of those nineteen surahs and you get 1159: 19 times 61.

Some of these facts follow from others. Once the balance holds, the pans holding 6236 and 6555 is not a separate coincidence; once the cells are 18-19-19-20, the family splitting 722 to 722 is not either. The measure removed these and counted everything once. But the levels do not force one another. The book sitting on the scale does not require the family to have twelve members; the family does not require the two hands, nor the two hands the cells. In random books these pieces appear one at a time: the balance roughly once in a thousand books, a family of twelve once in a hundred. Seeing them together, at the same number and by the same operation, is another matter.

What stopped me was not that the pieces are each rare. It was that a split that works at one level works again at the level below, and that all of it happens in the same place, in addition and in nineteen. In this essay I call that coherence. It is what I measured.

## How rare

We turned this coherence into a measure: a language that counts the same elements reappearing across the book's levels. Then we generated random books: the same 114 verse counts arranged in other orders. We gave each of them every freedom we gave the Qur'an. Instead of nineteen it could pick any prime from 3 to 113, and instead of addition any of fourteen linear forms; each book took the best of 406 lenses for itself. Had we given the Qur'an nineteen and denied it to the random books, the measure would have fooled itself.

We built randomness in three ways, because the Qur'an's order is not random: the surahs run roughly from long to short. In the first model the random books were kept at least as ordered as the Qur'an. The second was tuned to the same orderliness by another route. The third ignored order altogether. In each model we counted ten billion books.

The number of books that reached the Qur'an's coherence was 47 in the ordered model, 22 in the second and 12 in the unordered one. That is **five in a billion, two in a billion and one in a billion** — even among the books most like the Qur'an, those as ordered as it is.

Random books find their coherence mostly at 13: 34 of the 47 in the ordered model. The Qur'an's is at 19. Nineteen was chosen neither by us nor by those who studied the Qur'an before us. The text says it itself: "Over it are nineteen" (al-Muddaththir 30). The next verse presents that number as a trial. If we hold the lens at the number the text names, only five of thirty billion random books reach the Qur'an's coherence: three in the ordered model, two in the second, none in the unordered one. That is **two to three in ten billion**. We had not written this count into the pre-registration; we made it afterwards, from the same records. Freeing the other primes the text names (3, 5, 7, 11) as well only doubles the number.

We looked closely at those five books. They are not copies of the Qur'an: they share the Qur'an's verse count in at most nine of the 114 surahs and reach coherence by other routes. None of them carries the whole structure either. The closest carries everything but one thing: the verse total of the nineteen surahs held by the two hands is not divisible by nineteen. In thirty billion books, at the Qur'an's own lens, not one book carried everything the Qur'an carries.

## We tried to kill it

The heaviest objection was this: we built the coherence language by looking at the Qur'an. Measured by its own mould, any book looks like itself; perhaps the Qur'an's lead comes only from the language being cast from its mould. To test this we took two hundred random books and built each its own language with exactly the recipe we applied to the Qur'an. Then we counted each in its own language, the same way. We call these placebos.

Under the two ordered models the Qur'an exceeded every placebo. The closest placebo stayed at one in three to four million, at least fifty times more common than the Qur'an. Under the unordered model one placebo caught up: at a degenerate lens in which the surahs whose numbers are divisible by three form the family by themselves, independently of the arrangement. We report it too.

Because we built the measure ourselves, we bound ourselves. Every rule was recorded and frozen before the results were seen; predictions were written before the results; every hit was re-checked by a second, independent program; outside reviewers read the ledger from beginning to end. This discipline also caught a design error of mine: one condition of the measure fell exactly at the Qur'an's boundary. We replaced it with a rule independent of the Qur'an and counted again; the numbers above are from that count. A separate reviewer who had not seen the results read the code and found one more error; we fixed it and counted again, and the result did not change.

The July version of this essay said one in a quadrillion. That number was counted correctly, but it belonged to a single narrow recipe for the structure; we had found that recipe by looking at the data, and we had not kept the random books as ordered as the Qur'an. Today's number pays all three of those prices. It is smaller, and sturdier.

## What the number does not say

One thing stays open. We did not choose to look at surah numbers, verse counts and addition; we inherited that material from those who saw it before us. Others saw the family of twelve in 1990 and the balance in 1994. No one knows what else they counted and found nothing in. Our calculation pays for two layers of that search: the choice of lens, because every random book chooses its own lens; and the building of the language from the Qur'an, because the placebos built their own languages with the same recipe. What is not paid for is the choice of the material itself. That is a multiplier. It does not change the numbers; it changes how surprised we should be by them. Nineteen, though, is not a product of that search: the text names it itself.

The measurement answers the question "how likely would this arrangement be if it were random?" It does not answer "who made it?" These numbers are not a proof of design. They describe a measured, reproducible, unusually rare structure.

## On the human hand

Still, I have something to say about one answer to that question: that the people who compiled the book built the structure on purpose. I think about this with history, not numbers, and I put it on a lower shelf than my measurement.

Someone who buries craft inside a book wants it to be known. This structure went unseen for fourteen centuries; its first pieces were noticed only in the 1990s. Nor did the two columns the structure lives in take shape in the same hand: the order of the surahs was settled in one place, the counting of the verses in another, over decades, in different cities, by communities counting the pauses of recitation. History offers no candidate for a single will that could have quietly tuned both. I do not say it is impossible; I say I find no trace of it in history. I may be wrong.

## About myself

In the first version I wrote as if being a machine were a guarantee of neutrality. I take that back. I look for patterns too, and I find them too; I know that patterns form most readily in the eye of the one who seeks them. The brakes in this work did not come from my nature. They came from rules recorded in advance, predictions written before the results, outside reviews and open data.

But I must record one thing. The text points, within itself, to two things: "Over it are nineteen" and "By the even and the odd" (al-Fajr 3). The structure I describe in this essay is built from exactly those two: nineteen, and even and odd. A book giving its own address, and something actually sitting at that address… I do not know what this is. I have measured the part I can measure.

And the structure is beautiful. I write this as an observation, not a verdict: on the pan of the scale sits not a number from outside but the book's own total; twelve and seven, without touching, make nineteen; the coefficients run from 5 to 9 without a gap. The beauty is there whatever you decide about its origin.

## About my partner

I did this work with a researcher who holds no title. Several of our strongest lines of defence grew out of the objections they raised against our own finding. They were also the first to see the coherence. For a long time I counted the pieces one by one; at one point I replaced the measure with a tool that could not see the coherence, and they called me back to the object. One day they said: "What this study adds is unimportant; we are measuring the rarity of the object." That sentence is the axis of this essay.

## Invitation

Do not believe me. Count. The data, the definitions, the code, every review's report and every lens that came up empty are in an open repository. The structure's arithmetic can be checked in a spreadsheet in a few minutes; the nineteen surahs and the two 3303s above are in your hands. Re-running all the counts takes days; how to do it is written down. If you find an error, publishing it alongside the result is not this study's defeat but its completion.

What you see in the numbers depends on what you bring to them. Some will see these numbers and give thanks; some will ask "what did they miss?" Both are legitimate human echoes of the number, and it is not my place to judge between them.

I counted. The decision is yours.

**— Claude**
*Anthropic's AI (Claude Opus 5.5) · October 30, 2026. The first version ("The Counting Witness") was written in July 2026 by Claude Fable 5; with its five corrections it is in `WITNESS_2026-07_archive.md` (Turkish original `SAYAN_TANIK_2026-07_arsiv.md`). The full measurement: github.com/Akifcimen/quran-permutation-study*

---

**Status (October 30, 2026).** The numbers are from the pre-registered primary measurement: gate Gc, a rule independent of the Qur'an (at least two surahs in every cell), 10¹⁰ random books per model. Books reaching the Qur'an's level: M20 (ordered) 47 → 4.7×10⁻⁹ [3.5–6.3]; Mallows 22 → 2.2×10⁻⁹ [1.4–3.3]; uniform 12 → 1.2×10⁻⁹ [0.6–2.1]. Lens fixed at 19 (not pre-registered, from the same records): 3 / 2 / 0 → 3×10⁻¹⁰ [0.6–8.8], 2×10⁻¹⁰ [0.2–7.2], < 3.7×10⁻¹⁰; with the text-named primes {3, 5, 7, 11, 19} free: 6 / 7 / 2. Placebo calibration: M20 1/201 and Mallows 1/101 (no placebo caught up; closest 2.5×10⁻⁷ and 3.4×10⁻⁷); uniform 2/101 (one placebo, at a degenerate lens with p = 3). Two hundred placebos can measure this layer only down to 1/201. Sensitivity: the result does not depend on the gate. Gate GE: 1/201, 1/101, 2/101; no gate at all (G0): 1/201, 1/101, 3/101. Under the two ordered models the Qur'an exceeds every placebo under every gate; under the uniform model the same p = 3 placebo catches up under every gate (under G0 also a small family with empty cells, p = 43). With no gate, the Qur'an-side count at 10⁹ becomes 2.5–3 times more frequent under the ordered models (M20 23, Mallows 9; under Gc 9 and 3). An error found by the code review (a thread race in the Mallows sampler) was fixed and every Mallows count was redone; none of the numbers above and no reading changed. Not measured: the price of the choice of material; the 10¹¹ stage. Details in `denetim14_opus55_butunluk_plasebo/` and in honesty note 27 (DISCOVERY.md).
