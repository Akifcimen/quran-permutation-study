// Derin permütasyon motoru v2: t-katmanları + u = n−a "İki El" sayaçları.
//   W : Nu==7 && birleşim==19 && 19 | Σa(birleşim)      W2: W && Σu==133 (katsayı toplamı = üye sayısı)
//   BW/CW/DW/GW/AW/ADW/AGW: W'nin mevcut katmanlarla kesişimleri.
// Kullanım: ./kesif2_motor <seed> <trials>   (trials uint64)
//          ./kesif2_motor selftest           (gerçek veride bayraklar)
// Olaylar (kimlik: KESIF.md tanımlarıyla birebir):
//   A : Katman 1 terazi  (57 çift-t  &&  Σt(çift) == toplam ayet)
//   B : Katman 2         (aile 12 üye && katsayı toplamı 76  [Σt=1444])
//   C : Katman 2b        (B && çift yarı 6 üye && katsayı 38  [722/722])
//   D : Katman 2c        (C && hücreler 3/3/3/3 && tek-kol 19/19 && çift-kol {18,20})
//   E : katsayı kristali (B && çift yarı = {6,6,6,6,6,8} && tek yarı = {5,5,5,7,7,9})
//   F : D && E
//   G : tam kristal      (D && çift hücreler = {{6,6,6},{6,6,8}} && tek hücreler = {{5,5,9},{5,7,7}})
//   AB: A && B    AC: A && C    H: A && D    J: A && G
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const int AYAT127[114] = {
  7,286,200,176,120,165,206,75,127,109,123,111,43,52,99,128,111,110,98,135,
  112,78,118,64,77,227,93,88,69,60,34,30,73,54,45,83,182,88,75,85,
  54,53,89,59,37,35,38,29,18,45,60,49,62,55,78,96,29,22,24,13,
  14,11,11,18,12,12,30,52,52,44,28,28,20,56,40,31,50,40,46,42,
  29,19,36,25,22,17,19,26,30,20,15,21,11,8,8,19,5,8,8,11,
  11,8,3,9,5,4,7,3,6,3,5,4,5,6};

static uint64_t s_state;
static inline uint64_t splitmix64(void){
  uint64_t z = (s_state += 0x9E3779B97F4A7C15ULL);
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
  return z ^ (z >> 31);
}
// Lemire: [0, n) tarafsız (yaklaşık; n<=114 için sapma ihmal edilebilir değil ->
// 64-bit çarpım kullanıyoruz: sapma < 2^-57, tamamen ihmal edilebilir)
static inline uint32_t bounded(uint32_t n){
  return (uint32_t)(((unsigned __int128)splitmix64() * n) >> 64);
}

typedef struct {
  uint64_t T, A, B, C, D, E, F, G, AB, AC, H, J;
  uint64_t W, W2, BW, CW, DW, GW, AW, ADW, AGW;
} Counters;

// tek deneme değerlendirmesi: a[] = 114 ayet sayısı (sûre i+1 -> a[i])
static inline void evaluate(const int *a, Counters *K){
  int evenCnt = 0; long sumTEven = 0;
  int famCnt = 0, coeffSum = 0;
  int cellCnt[4] = {0,0,0,0}, cellSum[4] = {0,0,0,0};
  uint8_t histE[24], histO[24];
  memset(histE, 0, sizeof histE); memset(histO, 0, sizeof histO);
  // hücre indeksi: (tek_t?2:0) + (tek_no?1:0)
  uint16_t cellHist[4]; cellHist[0]=cellHist[1]=cellHist[2]=cellHist[3]=0; // kullanılmıyor; sum+karşılaştırma yerine multiset aşağıda
  int cellVals[4][4]; int cellN[4] = {0,0,0,0}; // hücre başına ham katsayılar (en çok 3 beklenir, taşmaya karşı 4)
  (void)cellHist;
  int nu = 0, usum = 0, uniCnt = 0; long uniSa = 0;
  for(int i = 0; i < 114; i++){
    int n = i + 1, t = n + a[i];
    int u = n - a[i];
    int fu = (u % 19 == 0);
    int ft = (t % 19 == 0);
    if(fu){ nu++; usum += u; }
    if(ft || fu){ uniCnt++; uniSa += a[i]; }
    int te = ((t & 1) == 0);
    if(te){ evenCnt++; sumTEven += t; }
    if(t % 19 == 0){
      int c = t / 19;
      famCnt++; coeffSum += c;
      int idx = (te ? 0 : 2) + ((n & 1) ? 1 : 0);
      cellCnt[idx]++; cellSum[idx] += c;
      if(cellN[idx] < 4) cellVals[idx][cellN[idx]++] = c;
      if(c < 24){ if(te) histE[c]++; else histO[c]++; }
    }
  }
  K->T++;
  int A_ = (evenCnt == 57 && sumTEven == 6234);
  int B_ = (famCnt == 12 && coeffSum == 76);
  int evenHalfCnt = cellCnt[0] + cellCnt[1];
  int evenHalfSum = cellSum[0] + cellSum[1];
  int C_ = (B_ && evenHalfCnt == 6 && evenHalfSum == 38);
  int D_ = 0, E_ = 0, G_ = 0;
  if(C_){
    int cnts_ok = (cellCnt[0]==3 && cellCnt[1]==3 && cellCnt[2]==3 && cellCnt[3]==3);
    int odd_ok  = (cellSum[2]==19 && cellSum[3]==19);
    int even_ok = ((cellSum[0]==18 && cellSum[1]==20) || (cellSum[0]==20 && cellSum[1]==18));
    D_ = cnts_ok && odd_ok && even_ok;
    E_ = (histE[6]==5 && histE[8]==1 && histO[5]==3 && histO[7]==2 && histO[9]==1);
    if(D_ && E_){
      // hücre multiset kimliği: sum*100 + kareler toplamı (3'lü, değerler <=21)
      int id[4];
      for(int k = 0; k < 4; k++){
        int s = 0, q = 0;
        for(int m = 0; m < cellN[k]; m++){ s += cellVals[k][m]; q += cellVals[k][m]*cellVals[k][m]; }
        id[k] = s * 1000 + q;
      }
      // hedefler: {6,6,6}=18108? 18*1000+108=18108; {6,6,8}=20136; {5,5,9}=19131; {5,7,7}=19123
      int evenPair = ((id[0]==18108 && id[1]==20136) || (id[0]==20136 && id[1]==18108));
      int oddPair  = ((id[2]==19131 && id[3]==19123) || (id[2]==19123 && id[3]==19131));
      G_ = evenPair && oddPair;
    }
  }
  if(A_) K->A++;
  if(B_) K->B++;
  if(C_) K->C++;
  if(D_) K->D++;
  if(E_) K->E++;
  if(D_ && E_) K->F++;
  if(G_) K->G++;
  if(A_ && B_) K->AB++;
  if(A_ && C_) K->AC++;
  if(A_ && D_) K->H++;
  if(A_ && G_) K->J++;
  int W_ = (nu == 7 && uniCnt == 19 && (uniSa % 19) == 0);
  if(W_){
    K->W++;
    if(usum == 133) K->W2++;
    if(B_) K->BW++;
    if(C_) K->CW++;
    if(D_) K->DW++;
    if(G_) K->GW++;
    if(A_) K->AW++;
    if(A_ && D_) K->ADW++;
    if(A_ && G_) K->AGW++;
  }
}

int main(int argc, char **argv){
  if(argc >= 2 && strcmp(argv[1], "selftest") == 0){
    Counters K; memset(&K, 0, sizeof K);
    evaluate(AYAT127, &K);
    printf("selftest  A=%llu B=%llu C=%llu D=%llu E=%llu F=%llu G=%llu H=%llu J=%llu W=%llu W2=%llu GW=%llu AGW=%llu\n",
      (unsigned long long)K.A,(unsigned long long)K.B,(unsigned long long)K.C,(unsigned long long)K.D,
      (unsigned long long)K.E,(unsigned long long)K.F,(unsigned long long)K.G,
      (unsigned long long)K.H,(unsigned long long)K.J,
      (unsigned long long)K.W,(unsigned long long)K.W2,(unsigned long long)K.GW,(unsigned long long)K.AGW);
    return 0;
  }
  if(argc < 3){ fprintf(stderr, "kullanım: %s <seed> <trials>\n", argv[0]); return 1; }
  s_state = strtoull(argv[1], NULL, 10);
  uint64_t trials = strtoull(argv[2], NULL, 10);
  int a[114]; memcpy(a, AYAT127, sizeof a);
  Counters K; memset(&K, 0, sizeof K);
  for(uint64_t it = 0; it < trials; it++){
    // Fisher–Yates (süregelen karma: karmanın karması da tekdüze)
    for(int i = 113; i > 0; i--){
      int j = (int)bounded((uint32_t)(i + 1));
      int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    }
    evaluate(a, &K);
  }
  printf("T=%llu A=%llu B=%llu C=%llu D=%llu E=%llu F=%llu G=%llu AB=%llu AC=%llu H=%llu J=%llu W=%llu W2=%llu BW=%llu CW=%llu DW=%llu GW=%llu AW=%llu ADW=%llu AGW=%llu\n",
    (unsigned long long)K.T,(unsigned long long)K.A,(unsigned long long)K.B,(unsigned long long)K.C,
    (unsigned long long)K.D,(unsigned long long)K.E,(unsigned long long)K.F,(unsigned long long)K.G,
    (unsigned long long)K.AB,(unsigned long long)K.AC,(unsigned long long)K.H,(unsigned long long)K.J,
    (unsigned long long)K.W,(unsigned long long)K.W2,(unsigned long long)K.BW,(unsigned long long)K.CW,
    (unsigned long long)K.DW,(unsigned long long)K.GW,(unsigned long long)K.AW,
    (unsigned long long)K.ADW,(unsigned long long)K.AGW);
  return 0;
}
