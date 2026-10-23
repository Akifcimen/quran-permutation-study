// kurallar.h — Ö0(b)'nin KESİN gerektirme listesi ve Ö0(c)'nin tekrarsız puanı için kural kümeleri (sabit mercek (1,1,19)).
// Her kural (öncül kümesi P → sonuç c): P'nin bütün olguları doğru olan her kitapta c de doğrudur (c'nin kendi tanımındaki
// boş-olmama koşulu — kF ≥ 1, kG ≥ 1 — c doğruysa zaten sağlanır). Kanıtlar ONKAYIT.md §Ö0(b)'de.
// Yalnız PUANLAMAYI ETKİLEYEN kurallar burada: öncüller ve sonuç farklı düğümlerde (aynı düğümdeki gerektirmeyi robotun
// "düğümde en nadir olgu" kuralı zaten tekrarsız sayar).
// Kapalı olgular: (i) bataryada devre dışı (MX×3, G: Σw=m·kG, U: Σv(U) kare/asal-kuvvet), (ii) terazi olguları ("kitap:"×12;
// terazi O4 = önceki literatür ve merceğin kaynağı), (iii) koşulun gerektirdiği olgular (O altında 5 olgu).
#ifndef KURALLAR_H
#define KURALLAR_H
#include "puan.h"
static void kurallar_kur(kurallar_t *K, char kosul, int kitap_kapali){
  memset(K,0,sizeof *K);
  // --- L0: koşulsuz, kesin, düğüm-arası ---
  kural_ekle(K, BIT(X_NA[1]),                 S_M2[1], "R1  G: Σn=Σa → G: m²|Σq");
  kural_ekle(K, BIT(H_M2V[0]),                S_M2[0], "R2  F: v-yarılar m²| → F: m²|Σq");
  kural_ekle(K, BIT(H_M2N[0]),                S_M2[0], "R3  F: n-yarılar m²| → F: m²|Σq");
  kural_ekle(K, BIT(H_M2V[1]),                S_M2[1], "R4  G: v-yarılar m²| → G: m²|Σq");
  kural_ekle(K, BIT(H_M2N[1]),                S_M2[1], "R5  G: n-yarılar m²| → G: m²|Σq");
  kural_ekle(K, BIT(X_N0[0]),                 U_W,     "R6  F: m|Σn → U: m|Σw(U)");
  kural_ekle(K, BIT(X_NA[0]),                 U_W,     "R7  F: Σn=Σa → U: m|Σw(U)");
  kural_ekle(K, BIT(X_N0[1]),                 U_V,     "R8  G: m|Σn → U: m|Σv(U)");
  kural_ekle(K, BIT(X_N0[0])|BIT(X_N0[1]),    U_N,     "R9  F: m|Σn ∧ G: m|Σn → U: m|Σn(U)");
  kural_ekle(K, BIT(X_N0[0])|BIT(X_N0[1]),    U_A,     "R10 F: m|Σn ∧ G: m|Σn → U: m|Σa(U)");
  kural_ekle(K, BIT(X_NA[0])|BIT(X_N0[1]),    U_N,     "R11 F: Σn=Σa ∧ G: m|Σn → U: m|Σn(U)");
  kural_ekle(K, BIT(X_NA[0])|BIT(X_N0[1]),    U_A,     "R12 F: Σn=Σa ∧ G: m|Σn → U: m|Σa(U)");
  kural_ekle(K, BIT(R_EQ)|BIT(K1[0]),         K1[1],   "R13 R: kF=kG ∧ F: k≥2E → G: k≥2E");
  kural_ekle(K, BIT(R_EQ)|BIT(K1[1]),         K1[0],   "R14 R: kF=kG ∧ G: k≥2E → F: k≥2E");
  kural_ekle(K, BIT(R_EQ)|BIT(K2[0]),         K2[1],   "R15 R: kF=kG ∧ F: k=m → G: k=m");
  kural_ekle(K, BIT(R_EQ)|BIT(K2[1]),         K2[0],   "R16 R: kF=kG ∧ G: k=m → F: k=m");
  kural_ekle(K, BIT(K2[0])|BIT(K2[1]),        R_EQ,    "R17 F: k=m ∧ G: k=m → R: kF=kG");
  kural_ekle(K, BIT(H_EQV[0])|BIT(RP_F),      K1[0],   "R18 F: v-yarılar eşit ∧ RP: F'de aynı v ≥5 → F: k≥2E");
  kural_ekle(K, BIT(DCELL[0])|BIT(RP_F),      K1[0],   "R19 F: hücreler dengeli ∧ RP: F'de aynı v ≥5 → F: k≥2E");
  kural_ekle(K, BIT(K2[0])|BIT(U_2M),         K1[1],   "R20 F: k=m ∧ U: |U|=2m → G: k≥2E");
  kural_ekle(K, BIT(K2[1])|BIT(U_2M),         K1[0],   "R21 G: k=m ∧ U: |U|=2m → F: k≥2E");
  kural_ekle(K, BIT(K2[1])|BIT(U_M),          U_W,     "R22 G: k=m ∧ U: |U|=m → U: m|Σw(U)");
  kural_ekle(K, BIT(K2[1])|BIT(U_M),          X_N0[0], "R23 G: k=m ∧ U: |U|=m → F: m|Σn");
  kural_ekle(K, BIT(K2[0])|BIT(U_M),          U_V,     "R24 F: k=m ∧ U: |U|=m → U: m|Σv(U)");
  kural_ekle(K, BIT(K2[0])|BIT(U_M),          X_N0[1], "R25 F: k=m ∧ U: |U|=m → G: m|Σn");
  kural_ekle(K, BIT(H_NEARV[0])|BIT(BAND[0]), K1[0],   "R26 F: v-yarılar Δ∈{m²,2m²} ∧ F: katsayı bandı → F: k≥2E");
  kural_ekle(K, BIT(H_NEARV[1])|BIT(BAND[1]), K1[1],   "R27 G: v-yarılar Δ∈{m²,2m²} ∧ G: katsayı bandı → G: k≥2E");
  // --- kapalılar ---
  K->kapali = BIT(MX_P)|BIT(MX_R)|BIT(MX_PR)|BIT(G_AVG)|BIT(U_VPP);
  if(kitap_kapali){ K->kapali |= BIT(B_CNT); for(int j=0;j<11;j++) K->kapali |= BIT(B_EQ[j]); }
  // --- L_O: O (= O1∧O2∧O3; kF=12, Σt(F)=1444, 6/6, 722/722, tek-t hücreleri 361/361) altında ek kesin kurallar ---
  if(kosul=='O'||kosul=='P'){
    kural_ekle(K, BIT(R_EQ),  K1[1], "RO1 (O) R: kF=kG → G: k≥2E   [kF=12 ⇒ kG=12]");
    kural_ekle(K, BIT(U_2M),  K1[1], "RO2 (O) U: |U|=2m → G: k≥2E   [kG = 26 + |F∩G| ≥ 26]");
    K->kapali_kosul = BIT(K1[0])|BIT(S_M2[0])|BIT(S_SQ[0])|BIT(H_EQV[0])|BIT(H_M2V[0]);
    K->kapali |= K->kapali_kosul; }
  // O' (= O ∧ O4, kosul 'P'): terazi olguları zaten kapalı; ek kural yok.
}
#endif
