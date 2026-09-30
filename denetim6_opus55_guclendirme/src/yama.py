# GÜÇLENDİRME yaması: beşinci denetimin robotlarına YALNIZ mercek-uzayı kısıtı ekler (ROBOT_MODS ortam değişkeni).
# Başka hiçbir satır değişmez; her değiştirme tam bir kez uygulanmak zorunda (assert).
import sys
MASK = r'''
static unsigned char LMASK[NLENS];   // GÜÇLENDİRME yaması: izinli mercekler (ROBOT_MODS="19" gibi; yoksa hepsi)
static void set_mods(void){ for(int li=0;li<NLENS;li++) LMASK[li]=1; const char *e=getenv("ROBOT_MODS"); if(!e) return;
  for(int li=0;li<NLENS;li++) LMASK[li]=0; char buf[512]; strncpy(buf,e,511); buf[511]=0;
  for(char *t=strtok(buf,","); t; t=strtok(NULL,",")){ int m=atoi(t); if(m<MMIN||m>MMAX) continue; for(int f=0;f<NFORM;f++) LMASK[f*NMOD+(m-MMIN)]=1; }
  int c=0; for(int li=0;li<NLENS;li++) c+=LMASK[li]; fprintf(stderr,"izinli mercek sayısı: %d (ROBOT_MODS=%s)\n",c,e); }
'''
def patch(src, dst):
    s = open(src, encoding='utf-8').read()
    def rep(old, new):
        nonlocal s
        assert s.count(old) == 1, (src, old, s.count(old))
        s = s.replace(old, new)
    rep('#define NCTX 41\n', '#define NCTX 41\n' + MASK)
    rep('for(int li=0; li<NLENS; li++){ fres_t R; facts(Z.a,li,&R);', 'for(int li=0; li<NLENS; li++){ if(!LMASK[li]) continue; fres_t R; facts(Z.a,li,&R);')
    rep('for(int li=0;li<NLENS;li++){ sc_t S=score_lens(a,li,NULL);', 'for(int li=0;li<NLENS;li++){ if(!LMASK[li]) continue; sc_t S=score_lens(a,li,NULL);')
    rep('set_disabled(); if(getenv("ROBOT_MINTR")) MINTR=atol(getenv("ROBOT_MINTR"));', 'set_disabled(); if(getenv("ROBOT_MINTR")) MINTR=atol(getenv("ROBOT_MINTR")); set_mods();')
    open(dst, 'w', encoding='utf-8').write(s)
for a, b in (('src/robot.c','src/robot_m.c'), ('src/robot2.c','src/robot2_m.c'), ('src/robot4.c','src/robot4_m.c')):
    patch(a, b); print('yamalandı:', b)
