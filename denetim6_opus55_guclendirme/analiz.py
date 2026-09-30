#!/usr/bin/env python3
"""Ön-kayıtlı p-değerleri (ONKAYIT.md §3–§4). p = (k+1)/(N+1), k = #{T2 ≥ T2_Kur'an − 1e-6}.
%95 aralık: k için kesin Poisson aralığı (p ≪ 1'de Clopper–Pearson ile aynı), N'ye bölünerek.
Çıktı dizisi sütunları (denetçinin biçimi): 0-2 en-iyi T1,T2,T3 (izinli mercekler) · 3-5 sabit (1,1,19) T1,T2,T3 · 6-8 argmax."""
import glob, math, os, sys
import numpy as np

EPS = 1e-6
FORMS = [(1, 1), (1, 2), (1, 3), (2, 1), (2, 3), (3, 1), (3, 2)]
NL = ["n·a", "n²+a", "n+a²", "n²+a²", "n²−a²", "ebob(n,a)", "n·a+n+a", "a²−n", "n²−a", "n·a−(n+a)"]


def lens_name(li):
    f, m = li // 38, 3 + li % 38
    return f"({FORMS[f][0]},{FORMS[f][1]},{m})" if f < 7 else f"({NL[f - 7]},{m})"


def pois_cdf(k, lam):
    if lam <= 0:
        return 1.0
    s, t = 0.0, math.exp(-lam)
    for i in range(k + 1):
        s += t
        t *= lam / (i + 1)
        if t < 1e-300 and i > lam:
            break
    return min(1.0, s)


def pois_ci(k, a=0.05):
    if k > 300:  # büyük k: exp(-λ) taşmasın diye Wilson–Hilferty yaklaşımı (göreli hata < %1)
        z = 1.959964
        lo = k * (1 - 1 / (9 * k) - z / (3 * math.sqrt(k))) ** 3
        hi = (k + 1) * (1 - 1 / (9 * (k + 1)) + z / (3 * math.sqrt(k + 1))) ** 3
        return lo, hi

    def bis(f, lo, hi):
        for _ in range(200):
            mid = 0.5 * (lo + hi)
            lo, hi = (mid, hi) if f(mid) else (lo, mid)
        return 0.5 * (lo + hi)
    lo = 0.0 if k == 0 else bis(lambda L: 1 - pois_cdf(k - 1, L) < a / 2, 0.0, k + 10 * math.sqrt(k + 1) + 10)
    hi = bis(lambda U: pois_cdf(k, U) > a / 2, 0.0, k + 20 * math.sqrt(k + 1) + 20)
    return lo, hi


class _Chunk:
    """Tam dizi (puan/*.f32) ya da sıkıştırılmış kuyruk (puan_ozet/*.npz). Kuyruk, her sütunda Kur'an eşiğinin 1,5 altına
    kadar olan bütün satırları tam hassasiyetle tutar; bu yüzden k sayımları birebir aynıdır (kontrol/sikistir.py doğrular)."""
    def __init__(self, X, N, Q, quant):
        self.X, self.N, self.Q, self.quant = X, N, Q, quant


def _key(s):
    b = os.path.basename(s).replace("_sifir.f32", "").replace(".npz", "")
    last = b.split("_")[-1]
    return int(last) if last.isdigit() else 0


def load(tag):
    def chunks(pattern_f32, pattern_npz):
        files = sorted(glob.glob(pattern_f32), key=_key)
        if files:
            return [_Chunk(np.fromfile(f, dtype=np.float32).reshape(-1, 9), None,
                           np.loadtxt(f.replace("_sifir.f32", "_kuran.txt")), None) for f in files]
        return [_Chunk(z["tail"], int(z["N"]), z["Q"], z["quantiles"])
                for z in (np.load(f) for f in sorted(glob.glob(pattern_npz), key=_key))]
    C = chunks(f"puan/{tag}_sifir.f32", f"puan_ozet/{tag}.npz") or \
        chunks(f"puan/{tag}_[0-9]*_sifir.f32", f"puan_ozet/{tag}_[0-9]*.npz")
    if not C:
        return None
    for c in C[1:]:
        assert np.allclose(c.Q, C[0].Q)
    return C, C[0].Q


def row(tag, col=1, qcol=None):
    r = load(tag)
    if r is None:
        return None
    C, q = r
    qcol = col if qcol is None else qcol
    ks = [int(np.sum(c.X[:, col] >= q[qcol] - EPS)) for c in C]
    k = sum(ks)
    N = sum((c.N if c.N is not None else len(c.X)) for c in C)
    lo, hi = pois_ci(k)
    if C[0].N is None:
        allx = np.concatenate([c.X[:, col] for c in C])
        med, q9999 = float(np.median(allx)), float(np.quantile(allx, 0.9999))
    else:  # sıkıştırılmış kayıt: parça niceliklerinin ortalaması (yaklaşık; yalnız bilgi sütunu)
        med = float(np.mean([c.quant[0, col] for c in C]))
        q9999 = float(np.mean([c.quant[4, col] for c in C]))
    args = np.concatenate([c.X[:, 7][c.X[:, col] >= q[qcol] - EPS] for c in C])
    return dict(tag=tag, k=k, N=N, p=(k + 1) / (N + 1), lo=lo / N, hi=hi / N, Q=q[qcol], ks=ks,
                med=med, q9999=q9999, args=args)


def fmt(r, x7=True):
    s = f"{r['p']:.2e} [{r['lo']:.1e}–{r['hi']:.1e}] ({r['k']}/{r['N']:,})"
    return s + (f" | ×7 okul: {min(1, 7 * r['p']):.2e}" if x7 else "")


def main():
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    out = []
    P = out.append
    P("# Ön-kayıtlı sonuçlar (otomatik üretildi: analiz.py)\n")
    P("Kur'an'ın puanı her satırda sabittir (en iyi merceği (1,1,19)); p = (k+1)/(N+1); köşeli parantez: k/N için %95 kesin Poisson aralığı.\n")
    P("## A1 — birincil: modül 19 sabit, biçim serbest (T2, en iyi 8 düğüm)\n")
    P("| Analiz | Sıfır | Kur'an T2 | p (izinli mercekler, en iyi) | parça başına k (10×2M) | sıfır medyan / %99,99 |")
    P("|---|---|---|---|---|---|")
    res = {}
    for a, desc in (("A1a", "iddia, 7 biçim"), ("A1b", "defter, matris kapalı, 7 biçim"),
                    ("A1c", "defter, matris kapalı, 17 biçim")):
        for n in ("U", "J10", "M20"):
            r = row(f"{a}_{n}")
            if r is None:
                continue
            res[(a, n)] = r
            P(f"| {a} ({desc}) | {n} | {r['Q']:.2f} | {fmt(r)} | {r['ks']} | {r['med']:.2f} / {r['q9999']:.2f} |")
    P("")
    P("### Sabit mercek (1,1,19) sağlaması — denetçinin 'mercek verili' basamağıyla karşılaştırma (aynı kitaplar, sütun 4)\n")
    P("| Analiz | Sıfır | p sabit (1,1,19) | denetçi |")
    P("|---|---|---|---|")
    ref = {("A1a", "U"): "2,3×10⁻⁷ (9/4×10⁷)", ("A1a", "J10"): "6,1×10⁻⁶ (244/4×10⁷)", ("A1a", "M20"): "5,2×10⁻⁶ (206/4×10⁷)",
           ("A1b", "J10"): "3,75×10⁻⁵ (74/2×10⁶)", ("A1b", "U"): "1,0×10⁻⁶ (1/2×10⁶)",
           ("A1c", "J10"): "4,4×10⁻⁵ (J10, 646 mercekli tablo)", ("A1c", "U"): "5,0×10⁻⁷ (0/2×10⁶)"}
    for (a, n), _ in res.items():
        r = row(f"{a}_{n}", col=4)
        P(f"| {a} | {n} | {fmt(r, x7=False)} | {ref.get((a, n), '—')} |")
    P("")
    P("### Kur'an'ı eşleyen/geçen sıfır kitaplarının en iyi mercekleri (A1c)\n")
    for n in ("U", "J10", "M20"):
        r = res.get(("A1c", n))
        if r is None:
            continue
        vals, cnt = np.unique(r["args"].astype(int), return_counts=True)
        top = sorted(zip(cnt, vals), reverse=True)[:8]
        P(f"- {n}: " + ", ".join(f"{lens_name(v)}×{c}" for c, v in top))
    P("")
    P("## B3 — T1 ve T3 (birincil çıktılardan; yalnız rapor)\n")
    P("| Analiz | Sıfır | T1 p | T3 p |")
    P("|---|---|---|---|")
    for (a, n) in res:
        r1, r3 = row(f"{a}_{n}", col=0), row(f"{a}_{n}", col=2)
        P(f"| {a} | {n} | {fmt(r1, x7=False)} | {fmt(r3, x7=False)} |")
    P("")
    P("## B1 — metnin adlandırdığı 14 modül {3…12, 19, 20, 30, 40}\n")
    P("| Analiz | Sıfır | p |")
    P("|---|---|---|")
    for a in ("B1a", "B1c"):
        for n in ("U", "J10", "M20"):
            r = row(f"{a}_{n}")
            if r:
                P(f"| {a} ({'iddia, 7 biçim' if a == 'B1a' else 'defter, 17 biçim'}) | {n} | {fmt(r)} |")
    P("")
    P("## B5 — kör batarya (hücreler kapalı), m = 19, 7 biçim\n")
    for n in ("U", "J10"):
        r = row(f"B5_{n}")
        if r:
            P(f"- {n}: Kur'an T2 = {r['Q']:.2f} → p = {fmt(r)}")
    P("")
    P("## B2 — tablo hassasiyeti (A1c, m = 19, 2×10⁷ kitaplık tablolar, 2×10⁷ yeni kitap)\n")
    for n in ("U", "J10", "M20"):
        r = row(f"B2_{n}")
        if r:
            base = res.get(("A1c", n))
            ratio = f" | A1c'ye oran ×{r['p'] / base['p']:.2f}" if base else ""
            P(f"- {n}: Kur'an T2 = {r['Q']:.2f} → p = {fmt(r)}{ratio}")
    P("")
    P("## B4 — denetçinin atladığı M20'de tam belgeli arama (646 mercek, orijinal robot4)\n")
    r = row("B4_M20_tam")
    if r:
        P(f"- M20: Kur'an T2 = {r['Q']:.2f} → serbest p = {fmt(r)}  (denetçi J10: 5,5×10⁻³ → ×7 3,9×10⁻²; U: 1,44×10⁻⁴ → ×7 1,0×10⁻³)")
        rf = row("B4_M20_tam", col=4)
        P(f"- M20 sabit (1,1,19): {fmt(rf, x7=False)}")
    P("")
    P("## KEŞİF AMAÇLI (ön-kayıt dışı; ONKAYIT_EK.md ve sonradan eklenen sağlamlık kontrolleri)\n")
    P("### E — çift-sayma kontrolü: terazi kanıt sayılmazsa (merceği 'satın aldıysa')\n")
    P("| Batarya | Sıfır | Kur'an T2 (terazisiz) | E1: sabit (1,1,19) | E2: m=19, biçim serbest |")
    P("|---|---|---|---|---|")
    for a, desc in (("E1", "iddia, 7 biçim"), ("E3", "defter, 17 biçim")):
        for n in ("U", "J10", "M20"):
            rf, rb = row(f"{a}_{n}", col=4), row(f"{a}_{n}", col=1)
            if rf and rb:
                P(f"| {desc} | {n} | {rb['Q']:.2f} | {fmt(rf, x7=False)} | {fmt(rb)} |")
    P("")
    P("### X — modül 19 ve 38 = 2×19 (bir 19-analistinin 19'un katına da bakabileceği itirazına karşı)\n")
    for n in ("U", "J10", "M20"):
        r = row(f"X38_{n}")
        if r:
            P(f"- A1c bataryası, m ∈ {{19, 38}}, {n}: p = {fmt(r)}")
    txt = "\n".join(out) + "\n"
    open("sonuc/ONKAYITLI_SONUCLAR.md", "w").write(txt)
    print(txt)


if __name__ == "__main__":
    main()
