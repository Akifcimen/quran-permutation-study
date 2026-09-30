#!/usr/bin/env python3
# Önceden sabitlenmiş beş özellik (koşu bitmeden yazıldı, 29 Eyl 2026):
# 1 modül 19 mu · 2 tek aileli mi · 3 kitap toplam terazisi var mı · 4 kesin eşitlik sayısı · 5 modülle kısa yazılabilen sabit sayısı
# Kısa yazım kuralı: değer ∈ {c·m, c·m², (c·m)²} ve 1 ≤ c ≤ 4 (sıfır sayılmaz).
import re, sys, json, numpy as np
N = np.arange(1, 115)
ESIT = ("|E|=|O| &", "yarılar eşit", "Σn=Σa", "|U|=m", "|U|=2m", "k=m", "hücreler dengeli", "kF=kG")
def kisa(x, m):
    x = abs(int(x))
    if x == 0: return False
    for c in range(1, 5):
        if x in (c*m, c*m*m, (c*m)**2): return True
    return False
def sabitler(a, al, be, m):
    v = al*N + be*a; w = al*N - be*a; F = v % m == 0; G = w % m == 0; U = F | G
    out = {}
    for ad, fam, q in (("F", F, v), ("G", G, w)):
        if fam.sum() == 0: continue
        qq = q[fam]; nn = N[fam]; aa = a[fam]; ve = qq % 2 == 0; ne = nn % 2 == 0
        out[f"{ad}:Σq"] = qq.sum(); out[f"{ad}:v-yarı"] = qq[ve].sum(); out[f"{ad}:n-yarı"] = qq[ne].sum()
        out[f"{ad}:Σn"] = nn.sum(); out[f"{ad}:Σa"] = aa.sum()
    out["U:boy"] = U.sum(); out["U:Σa"] = a[U].sum()
    return out
def cozumle(dosya):
    bloklar = open(dosya, encoding='utf-8').read().split("--- SIRA ")[1:]
    satirlar = []
    for b in bloklar:
        bas = re.search(r"en iyi mercek \((\d+),(\d+),(\d+)\) \| T2=([\d.]+) \| kF=(\d+) kG=(\d+)", b)
        al, be, m = int(bas[1]), int(bas[2]), int(bas[3]); t2 = float(bas[4])
        olgular = re.findall(r"\] (.+?)\s+p=", b)
        a = np.array([int(x) for x in re.search(r"diziliş: ([\d,]+)", b)[1].split(",")])
        aileF = any(o.startswith("F:") for o in olgular); aileG = any(o.startswith("G:") for o in olgular)
        terazi = any(o.startswith("kitap: |E|=|O| &") for o in olgular)
        esit = sum(any(e in o for e in ESIT) for o in olgular)
        sab = sabitler(a, al, be, m); kisa_say = sum(kisa(x, m) for k, x in sab.items() if not k.endswith("boy")) + (1 if sab["U:boy"] in (m, 2*m) else 0)
        satirlar.append((t2, f"({al},{be},{m})", m == 19, not (aileF and aileG), terazi, esit, len(olgular), kisa_say))
    return satirlar
def yaz(baslik, satirlar):
    print(f"\n{baslik}\n{'T2':>6s} {'mercek':>10s} {'m=19':>5s} {'tek aile':>8s} {'terazi':>6s} {'eşitlik':>8s} {'kısa sabit':>10s}")
    for t2, ln, m19, tek, ter, es, no, ks in satirlar:
        print(f"{t2:6.2f} {ln:>10s} {'evet' if m19 else '-':>5s} {'evet' if tek else 'KARIŞIK':>8s} {'VAR' if ter else '-':>6s} {es:>4d}/{no:<3d} {ks:>10d}")
if __name__ == "__main__":
    K = np.array([s['numberOfAyahs'] for s in json.load(open('../quran_meta.json'))['data']['surahs']['references']])
    A = np.where(N == 9, 127, K); sab = sabitler(A, 1, 1, 19)
    print("KUR'AN (1,1,19) sabitleri:", {k: int(v) for k, v in sab.items()})
    print("  kısa yazılabilen:", [k for k, x in sab.items() if (kisa(x, 19) if not k.endswith('boy') else x in (19, 38))])
    for f, bas in sys.argv[1:]: pass
