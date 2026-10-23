// robot4_m.c'nin kendi örnekleyicileri ve facts() ile sabit mercek (1,1,19) bitlerini dosyaya yazar (doğrulama).
// Kullanım: ./robot_yan <null U|J10|M20> <seed> <N> <cikti.bin>   (ROBOT_BASE ile 129)
#define main robot_main_kullanilmaz
#include "robot4_m.c"
#undef main
int main(int argc, char **argv){
  init_facts();
  for(int p=2;p<=7;p++){ if(p==4||p==6) continue; long v=(long)p*p*p; while(v<1000000){ PP[nPP++]=v; v*=p; } }
  for(int m=2;m<64;m++){ int pr=1; for(int d=2;d*d<=m;d++) if(m%d==0) pr=0; ISPRIME[m]=pr; if(pr){ long o=(long)m*m-1; NPF[m]=0; for(long q=2;q<=o;q++){ if(o%q==0){ int isp=1; for(long d=2;d*d<=q;d++) if(q%d==0){isp=0;break;} if(isp) PF[m][NPF[m]++]=(int)q; } } } }
  for(int i=0;i<28;i++) ISMD[MEDINAN[i]]=1; for(int i=0;i<29;i++) ISMQ[MUQ[i]]=1;
  memcpy(BASE,AYAT127,sizeof BASE);
  if(getenv("ROBOT_BASE")){ const char *s=getenv("ROBOT_BASE"); char *e; for(int i=0;i<114;i++){ BASE[i]=(int)strtol(s,&e,10); s=e; if(*s==',') s++; } }
  S_OBS=0; for(int i=0;i<114;i++) S_OBS+=(long)(i+1)*BASE[i];
  LI_Q=0*NMOD+(19-MMIN);
  char null=argv[1][0]; double par=strlen(argv[1])>1?atof(argv[1]+1):0; uint64_t seed=strtoull(argv[2],NULL,10), N=strtoull(argv[3],NULL,10);
  FILE *fo=fopen(argv[4],"wb"); sampler_t Z; sampler_init(&Z,null,par,seed);
  // önce gerçek kitap
  { fres_t R; facts(BASE,LI_Q,&R); uint64_t lo=(uint64_t)R.bits, hi=(uint64_t)(R.bits>>64); int16_t b[114]; for(int i=0;i<114;i++) b[i]=BASE[i];
    fwrite(b,2,114,fo); fwrite(&lo,8,1,fo); fwrite(&hi,8,1,fo); int k[3]={R.kF,R.kG,R.same}; fwrite(k,4,3,fo); }
  for(uint64_t it=0; it<N; it++){ sampler_next(&Z); fres_t R; facts(Z.a,LI_Q,&R); uint64_t lo=(uint64_t)R.bits, hi=(uint64_t)(R.bits>>64);
    int16_t b[114]; for(int i=0;i<114;i++) b[i]=Z.a[i]; fwrite(b,2,114,fo); fwrite(&lo,8,1,fo); fwrite(&hi,8,1,fo); int k[3]={R.kF,R.kG,R.same}; fwrite(k,4,3,fo); }
  fclose(fo); printf("yazıldı: %llu kitap (+gerçek), olgu=%d\n",(unsigned long long)N,NFACT); return 0; }
