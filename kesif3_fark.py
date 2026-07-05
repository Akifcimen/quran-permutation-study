#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""İkinci değişken u = sûre no − ayet sayısı incelemesi.
1) u'nun 19-ailesi (üyeler, toplamlar, her iki kodeks)
2) t-ailesi ile etkileşim: kesişim, birleşim
3) Aday olayların permütasyon ölçümü (aday listesi ölçümden ÖNCE sabitlenmiştir)
"""
import json
import numpy as np

d = json.load(open('/Users/dejuremacminim1/Desktop/hrf2/quran_meta.json'))
A0 = np.array([s['numberOfAyahs'] for s in d['data']['surahs']['references']])
N = np.arange(1, 115)

def betim(tag, A):
    t = N + A; u = N - A
    print(f"\n===== {tag} =====")
    ft = (t % 19 == 0); fu = (u % 19 == 0)
    print(f"t-ailesi ({int(ft.sum())} üye): {list(N[ft])}")
    print(f"u-ailesi ({int(fu.sum())} üye): {list(N[fu])}")
    print(f"  u değerleri: {list(u[fu])}  (katsayılar: {list(u[fu]//19)})")
    print(f"  Σu = {int(u[fu].sum())} = {int(u[fu].sum()//19)}×19 ; Σ|u| = {int(abs(u[fu]).sum())}")
    kes = ft & fu
    print(f"KESİŞİM: {int(kes.sum())} üye {list(N[kes])}")
    bir = ft | fu
    print(f"BİRLEŞİM: {int(bir.sum())} üye")
    sa, sn, st = int(A[bir].sum()), int(N[bir].sum()), int(t[bir].sum())
    print(f"  Σa(birleşim) = {sa}" + (f" = 19×{sa//19} ✓" if sa % 19 == 0 else f" (mod19={sa%19})"))
    print(f"  Σn(birleşim) = {sn} (mod19={sn%19}) ; Σt(birleşim) = {st} (mod19={st%19}, ={st})")
    print(f"  Σa(t-ailesi) = {int(A[ft].sum())} ; Σa(u-ailesi) = {int(A[fu].sum())}")
    # u-ailesinin kendi iç yapısı
    te = (t % 2 == 0)
    print(f"  u-ailesi parite: çift-t {int((fu&te).sum())} / tek-t {int((fu&~te).sum())}")
    # tüm bölenler taraması: hangi d için u-katları 12 üye veriyor? (kullanıcının 12 bulgusu için)
    on2 = [dd for dd in range(2, 60) if int((u % dd == 0).sum()) == 12]
    print(f"  u'nun d-katları tam 12 üye veren d'ler: {on2}")

A127 = np.where(N == 9, 127, A0)
betim("RK-127", A127)
betim("STD-129", A0)

# ---------------- PERMÜTASYON ÖLÇÜMLERİ (kodeks 127 çokluğu) ----------------
# Aday olaylar (ölçümden önce sabit):
#  M1: |birleşim| = 19
#  M2: M1 ve 19 | Σa(birleşim)
#  M3: (Nt, Nu) = (12, 7)
#  M4: B (=Nt 12 ve Σt=1444) ve Nu = 7
#  M5: M4 ve 19 | Σa(birleşim)
#  M6: M5 ve M1 (kesişim boş demek)  — tam gözlenen tablo
print("\n===== PERMÜTASYON ÖLÇÜMÜ (20M, kodeks 127) =====")
rng = np.random.default_rng(7)
B_TRIALS = 20_000_000
CHUNK = 100_000
cnt = dict(M1=0, M2=0, M3=0, M4=0, M5=0, M6=0, B=0, B_and_Nu7=0, Nt12=0, Nu7=0)
counts = A127.copy()
for _ in range(B_TRIALS // CHUNK):
    P = rng.permuted(np.tile(counts, (CHUNK, 1)), axis=1)
    T = P + N; U = N - P
    FT = (T % 19 == 0); FU = (U % 19 == 0)
    nt = FT.sum(1); nu = FU.sum(1)
    uni = (FT | FU).sum(1)
    saU = (P * (FT | FU)).sum(1)
    m1 = (uni == 19)
    m2 = m1 & (saU % 19 == 0)
    m3 = (nt == 12) & (nu == 7)
    b = (nt == 12) & ((T * FT).sum(1) == 1444)
    m4 = b & (nu == 7)
    m5 = m4 & (saU % 19 == 0)
    m6 = m5 & m1
    cnt['M1'] += int(m1.sum()); cnt['M2'] += int(m2.sum()); cnt['M3'] += int(m3.sum())
    cnt['M4'] += int(m4.sum()); cnt['M5'] += int(m5.sum()); cnt['M6'] += int(m6.sum())
    cnt['B'] += int(b.sum()); cnt['Nt12'] += int((nt == 12).sum()); cnt['Nu7'] += int((nu == 7).sum())
T = B_TRIALS
print(f"denemeler: {T:,}")
for k in ['Nt12', 'Nu7', 'M1', 'M2', 'M3', 'B', 'M4', 'M5', 'M6']:
    n = cnt[k]
    print(f"  {k}: {n:8d}  p={n/T:.3e}" + (f"  (1/{T/n:,.0f})" if n else ""))
print("\nBağımsızlık/koşullular:")
print(f"  P(Nu=7) = {cnt['Nu7']/T:.4f} ; P(Nu=7 | Nt=12) = {cnt['M3']/max(cnt['Nt12'],1):.4f}  (çarpımsallık testi)")
print(f"  P(Nu=7 | B) = {cnt['M4']/max(cnt['B'],1):.4f}")
print(f"  P(M5 | B)  = {cnt['M5']/max(cnt['B'],1):.5f}   ← mevcut yapının üstüne koşullu çarpan")
print(f"  P(M2)      = {cnt['M2']/T:.3e}  (bağımsız katman olarak)")
