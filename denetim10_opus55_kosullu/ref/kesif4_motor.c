// Derin permütasyon motoru v4: t-katmanları + İki El + MATRİS İLKELLİK sayaçları.
// Yeni: isabetlerde grup matrislerinin (P: parite, R: 19-aile) mod-19 mertebesi
// hesaplanır; ord==360 (ilkellik) her katman seviyesinde doğrudan sayılır.
//   X1: A ∧ ord(P)=360        X2: B ∧ ord(R)=360
//   X3: C ∧ her ikisi 360     X4: D ∧ her ikisi     X5: D∧W ∧ her ikisi   X6: G ∧ her ikisi
//   W3: W ∧ pozitif-u toplamı 266 (=14×19) ["7 merdiveni" süsü — yalnız sayılır]
// Kullanım: ./kesif4_motor <seed> <trials> | selftest
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
static inline uint32_t bounded(uint32_t n){
  return (uint32_t)(((unsigned __int128)splitmix64() * n) >> 64);
}

// 2x2 mod-19 mertebe: tam 360 mı? (ilk I dönüşü 360. adımda ise ilkel)
static int ord_is_360(int m00,int m01,int m10,int m11){
  int x00=1,x01=0,x10=0,x11=1;
  for(int k=1;k<=360;k++){
    int y00=(x00*m00+x01*m10)%19, y01=(x00*m01+x01*m11)%19;
    int y10=(x10*m00+x11*m10)%19, y11=(x10*m01+x11*m11)%19;
    x00=y00;x01=y01;x10=y10;x11=y11;
    if(x00==1&&x01==0&&x10==0&&x11==1) return k==360;
  }
  return 0;
}

typedef struct {
  uint64_t T,A,B,C,D,E,F,G,AB,AC,H,J;
  uint64_t W,W2,W3,BW,CW,DW,GW,AW,ADW,AGW;
  uint64_t X1,X2,X3,X4,X5,X6;
} Counters;

static inline void evaluate(const int *a, Counters *K){
  int evenCnt = 0; long sumTEven = 0;
  long snE = 0, saE = 0;            // çift-t tarafının Σn, Σa (P matrisi için)
  long fn = 0, fa = 0;              // aile Σn, Σa (R matrisi için)
  int famCnt = 0, coeffSum = 0;
  int cellCnt[4] = {0,0,0,0}, cellSum[4] = {0,0,0,0};
  uint8_t histE[24], histO[24];
  memset(histE, 0, sizeof histE); memset(histO, 0, sizeof histO);
  int cellVals[4][4]; int cellN[4] = {0,0,0,0};
  int nu = 0, usum = 0, uposs = 0, uniCnt = 0; long uniSa = 0;
  for(int i = 0; i < 114; i++){
    int n = i + 1, t = n + a[i];
    int u = n - a[i];
    int fu = (u % 19 == 0);
    int ft = (t % 19 == 0);
    if(fu){ nu++; usum += u; if(u > 0) uposs += u; }
    if(ft || fu){ uniCnt++; uniSa += a[i]; }
    int te = ((t & 1) == 0);
    if(te){ evenCnt++; sumTEven += t; snE += n; saE += a[i]; }
    if(ft){
      int c = t / 19;
      famCnt++; coeffSum += c; fn += n; fa += a[i];
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
      int id[4];
      for(int k = 0; k < 4; k++){
        int s = 0, q = 0;
        for(int m = 0; m < cellN[k]; m++){ s += cellVals[k][m]; q += cellVals[k][m]*cellVals[k][m]; }
        id[k] = s * 1000 + q;
      }
      int evenPair = ((id[0]==18108 && id[1]==20136) || (id[0]==20136 && id[1]==18108));
      int oddPair  = ((id[2]==19131 && id[3]==19123) || (id[2]==19123 && id[3]==19131));
      G_ = evenPair && oddPair;
    }
  }
  int W_ = (nu == 7 && uniCnt == 19 && (uniSa % 19) == 0);
  // matris ilkellikleri (yalnız gereken isabetlerde hesapla)
  int pOK = 0, rOK = 0;
  if(A_ || C_){
    // P = [[Σn(çift), Σa(çift)], [Σn(tek), Σa(tek)]] mod 19 ; toplamlar sabit: Σn=6555, Σa=6234
    int p00 = (int)(snE % 19), p01 = (int)(saE % 19);
    int p10 = (int)((6555 - snE) % 19), p11 = (int)((6234 - saE) % 19);
    pOK = ord_is_360(p00, p01, p10, p11);
  }
  if(B_){
    int r00 = (int)(fn % 19), r01 = (int)(fa % 19);
    int r10 = (int)((6555 - fn) % 19), r11 = (int)((6234 - fa) % 19);
    rOK = ord_is_360(r00, r01, r10, r11);
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
  if(W_){
    K->W++;
    if(usum == 133) K->W2++;
    if(uposs == 266) K->W3++;
    if(B_) K->BW++;
    if(C_) K->CW++;
    if(D_) K->DW++;
    if(G_) K->GW++;
    if(A_) K->AW++;
    if(A_ && D_) K->ADW++;
    if(A_ && G_) K->AGW++;
  }
  if(A_ && pOK) K->X1++;
  if(B_ && rOK) K->X2++;
  if(C_ && pOK && rOK) K->X3++;
  if(D_ && pOK && rOK) K->X4++;
  if(D_ && W_ && pOK && rOK) K->X5++;
  if(G_ && pOK && rOK) K->X6++;
}

static void print_counters(const char *pfx, const Counters *K){
  printf("%sT=%llu A=%llu B=%llu C=%llu D=%llu E=%llu F=%llu G=%llu AB=%llu AC=%llu H=%llu J=%llu "
         "W=%llu W2=%llu W3=%llu BW=%llu CW=%llu DW=%llu GW=%llu AW=%llu ADW=%llu AGW=%llu "
         "X1=%llu X2=%llu X3=%llu X4=%llu X5=%llu X6=%llu\n", pfx,
    (unsigned long long)K->T,(unsigned long long)K->A,(unsigned long long)K->B,(unsigned long long)K->C,
    (unsigned long long)K->D,(unsigned long long)K->E,(unsigned long long)K->F,(unsigned long long)K->G,
    (unsigned long long)K->AB,(unsigned long long)K->AC,(unsigned long long)K->H,(unsigned long long)K->J,
    (unsigned long long)K->W,(unsigned long long)K->W2,(unsigned long long)K->W3,
    (unsigned long long)K->BW,(unsigned long long)K->CW,(unsigned long long)K->DW,(unsigned long long)K->GW,
    (unsigned long long)K->AW,(unsigned long long)K->ADW,(unsigned long long)K->AGW,
    (unsigned long long)K->X1,(unsigned long long)K->X2,(unsigned long long)K->X3,
    (unsigned long long)K->X4,(unsigned long long)K->X5,(unsigned long long)K->X6);
}

int main(int argc, char **argv){
  if(argc >= 2 && strcmp(argv[1], "selftest") == 0){
    Counters K; memset(&K, 0, sizeof K);
    evaluate(AYAT127, &K);
    print_counters("selftest ", &K);
    return 0;
  }
  if(argc < 3){ fprintf(stderr, "kullanım: %s <seed> <trials>\n", argv[0]); return 1; }
  s_state = strtoull(argv[1], NULL, 10);
  uint64_t trials = strtoull(argv[2], NULL, 10);
  int a[114]; memcpy(a, AYAT127, sizeof a);
  Counters K; memset(&K, 0, sizeof K);
  for(uint64_t it = 0; it < trials; it++){
    for(int i = 113; i > 0; i--){
      int j = (int)bounded((uint32_t)(i + 1));
      int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    }
    evaluate(a, &K);
  }
  print_counters("", &K);
  return 0;
}
