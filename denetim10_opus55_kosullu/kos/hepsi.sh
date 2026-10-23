#!/bin/bash
# Baştan sona yeniden üretim — bu ölçümde koşulan adımlar, koşuldukları sırayla ve aynı tohumlarla (ONKAYIT §3–§5).
# Uzun sürer (tek makinede ~1 gün); her havuz bölütlüdür ve kaldığı yerden sürer (kos/havuz.py). Adımlar ayrı ayrı da koşulabilir:
#   bash kos/hepsi.sh <adım>      adımlar: derle sinir c1 o0b o0a havuz puan u o2u m20 mallows j10 kesif
# Çıktıların büyük ara dosyaları (tab/) paket dışıdır; küçük sonuçlar sonuc/ altındadır.
cd "$(dirname "$0")/.."
adim=${1:-hepsi}
calis() { [ "$adim" = hepsi ] || [ "$adim" = "$1" ]; }

if calis derle; then
  for p in kosul_u_hazirla kosullu_uret kosullu_puan tam_ortak o_ussu cekirdek zincir_sina tekrar9 cikarim; do cc -O3 -march=native -o src/$p src/$p.c -lm; done
  for p in bit_karsilastir yokla u_capa u_k_sina mallows_hiz hizli_sina lz_esit jz_sina jz_sira_esit hiz_olc kosulsuz_kayit olgu_meta; do cc -O3 -march=native -o kontrol/$p kontrol/$p.c -lm; done
  cc -O3 -o ref/robot4_m ref/robot4_m.c -lm -lpthread       # robotun değiştirilmemiş kopyası (Ö0(a))
  cc -O2 -o ref/robot_yan ref/robot_yan.c -lm -lpthread      # port eşitliği için yan araç (robotun örnekleyicileri)
fi
# Tekdüze kesin örnekleyicinin sınırı B ve kesin P(|F| = 12) (Σk = 12'li bütün vektörler; kodeks başına ~10 dk)
if calis sinir; then ./src/kosul_u_hazirla 129 12 sonuc/u_B_129_K12.txt; ./src/kosul_u_hazirla 127 12 sonuc/u_B_127_K12.txt; ./kontrol/u_k_sina 127 3000000 > sonuc/u_k_sina_127.txt; fi
# Ç1 çapası (dondurmadan önce; 4 × 6,5×10⁶ C-kitap, 127)
if calis c1; then for s in 1 2 3 4; do ./kontrol/u_capa 127 $((7100+s)) 6500000 2000 sonuc/u_capa127_$s; done; fi
# Port eşitliği (robotun örnekleyicileri ve olgu bitleri; 6 × 100.001 kitap) → sonuc/dogrulama_bit_esitligi.txt (kontrol/bit_karsilastir.c başlığı)
# Ö0(b): olgu birlikte-görülme taraması ve aday süzgeçleri (tab/cikarim büyük, paket dışı)
if calis o0b; then
  bash kos/cikarim_kos.sh; bash kos/tarama_sirali.sh
  python3 -I src/cikarim_suz.py 129 U M J L O X > sonuc/cikarim_adaylar_kosulsuz_129.txt; python3 -I src/cikarim_suz.py 127 U O > sonuc/cikarim_adaylar_kosulsuz_127.txt
  python3 -I src/cikarim_suz_O.py 129 O M_O_9101 L_O_9201 > sonuc/cikarim_adaylar_O_129.txt; python3 -I src/cikarim_suz_O.py 127 O > sonuc/cikarim_adaylar_O_127.txt
fi
# Ö0(a): robotun kendi tabloları + aynı kitaplarda özgün / tek düzeltme / T2* sayımı (defter bataryası), keşif: iddia bataryası
if calis o0a; then bash kos/tekrar9.sh; bash kos/tekrar9_iddia.sh; python3 -I src/t9_ozet.py > sonuc/t9/ozet_ana.txt; fi
# Ö1 havuzları (ONKAYIT §4.3 tohumları ve tavanları). İkinci sürücüler sondan başlar, aynı tavanı paylaşır (paralellik sonucu etkilemez).
if calis havuz; then
  for h in u129 u127 m20 m20g127 lg127; do bash kos/uretim_A.sh $h; done
  bash kos/uretim_A.sh mallows & TERS=1 python3 -I kos/havuz.py 129 L O 3291001 100 1000 tab/havuz/L129O 2 16 & wait
  bash kos/uretim_A.sh j10 & TERS=1 python3 -I kos/havuz.py 129 J O 4291001 20 1000 tab/havuz/J129O 3 6 & wait   # ikinci sürücü Mallows bitince başladı (kos/j10_ikinci.sh)
  bash kos/uretim_B.sh c3gercek                              # Ç3 başlangıç denetimi (gerçek dizilişten başlayan M20)
fi
# Tekdüze: puanlama (terazi kapalı + terazi açık), Ç4(a), tam birlikte-olasılık (yakın + bant; bant kuralı tetiklendi → bütün bant),
# Kur'an'ın çözünürlük eki (R = 5.000), birleşik tam sıra, O' (iskelet başına 3.000 kesin tamamlama), O' düzey-1 tam, Ç5 (Ö2, 127)
if calis u; then
  bash kos/puanla.sh 129 O tab/havuz/U129O U129O; bash kos/puanla.sh 127 O tab/havuz/U127O U127O
  bash kos/uretim_B.sh c4u
  bash kos/tam.sh 129 O U 6291001 tab/havuz/U129O U129O 2; python3 -I src/tam_cozunurluk.py sonuc/o1/U129O.tam.txt > sonuc/o1/U129O_tam_cozunurluk.txt
  bash kos/tam_bant.sh 129 O U 6291001 tab/havuz/U129O U129O 2
  SADECE_KURAN=1 ./src/tam_ortak 129 O U 1 6299001 5000 20 2.0 500 2000 sonuc/o1/U129O_kuran_R5000 sonuc/o1/U129O.puan.bin $(ls tab/havuz/U129O/*.bin | sort)
  python3 -I src/tam_analiz.py sonuc/o1/U129O.tam.txt kuran=sonuc/o1/U129O_kuran_R5000.tam.txt ek=sonuc/o1/U129O_bant.tam.txt > sonuc/o1/U129O_tam_analiz_birlesik.txt
  bash kos/o_ussu.sh U 2; bash kos/puanla.sh 129 P tab/havuz/U129P U129P
  DUZEY1=1 ./src/tam_ortak 129 P U 1 6295001 50 20 2.0 500 2000 sonuc/o1/U129P sonuc/o1/U129P.puan.bin $(ls tab/havuz/U129P/*.bin | sort) > sonuc/o1/U129P_tam.log 2>&1
  python3 -I src/tam_cozunurluk.py sonuc/o1/U129P.tam.txt > sonuc/o1/U129P_tam_cozunurluk.txt; python3 -I src/tam_analiz.py sonuc/o1/U129P.tam.txt > sonuc/o1/U129P_tam_analiz.txt
  ./src/cekirdek 127 U 7271001 20000000 1 sonuc/o2/U127_cekirdek.txt $(ls tab/havuz/U127O/*.bin | sort)
  python3 -I src/cekirdek_analiz.py sonuc/o2/U127_cekirdek.txt - --pG 7.142e-10 6.07e-10 8.30e-10
fi
# M20: puanlama, Ç4(b), tam, O', Ö2 (129 ve 127) — kos/m20_sonrasi.sh; O' düzey-1 tam ayrıca
if calis m20; then
  bash kos/m20_sonrasi.sh
  python3 -I src/tam_cozunurluk.py sonuc/o1/M129O.tam.txt > sonuc/o1/M129O_tam_cozunurluk.txt
  DUZEY1=1 ./src/tam_ortak 129 P M 1 6295002 50 20 2.0 500 2000 sonuc/o1/M129P sonuc/o1/M129P.puan.bin $(ls tab/havuz/M129P/*.bin | sort)
  python3 -I src/tam_cozunurluk.py sonuc/o1/M129P.tam.txt > sonuc/o1/M129P_tam_cozunurluk.txt; python3 -I src/tam_analiz.py sonuc/o1/M129P.tam.txt > sonuc/o1/M129P_tam_analiz.txt
  python3 -I src/cekirdek_analiz.py sonuc/o2/M129_cekirdek.txt tab/havuz/M129O; python3 -I src/cekirdek_analiz.py sonuc/o2/M127_cekirdek.txt tab/havuz/M127G
  python3 -I src/kume_m20.py tab/havuz/M129O
fi
# Mallows ve J10: kos/mallows_sonrasi.sh, kos/j10_sonrasi.sh (havuzlar bitince kendiliğinden)
if calis mallows; then bash kos/mallows_sonrasi.sh; python3 -I src/tam_cozunurluk.py sonuc/o1/L129O.tam.txt > sonuc/o1/L129O_tam_cozunurluk.txt
  python3 -I src/cekirdek_analiz.py sonuc/o2/L129_cekirdek.txt tab/havuz/L129O; python3 -I src/cekirdek_analiz.py sonuc/o2/L127_cekirdek.txt tab/havuz/L127G; fi
if calis j10; then bash kos/j10_sonrasi.sh; fi
# Keşif (ön-kayıt dışı): olay düzeyinde frekanslar ve tekdüze 129 için P(A∧W|G)
if calis kesif; then
  python3 -I src/olay_frekans.py tab/havuz/U129O tab/havuz/U127O; python3 -I src/olay_frekans.py tab/havuz/M129O; python3 -I src/olay_frekans.py tab/havuz/L129O
  ./src/cekirdek 129 U 7291001 20000000 1 sonuc/o2/U129_cekirdek_kesif.txt $(ls tab/havuz/U129O/*.bin | sort)
  python3 -I src/olay_gw.py sonuc/o2/U129_cekirdek_kesif.txt 117 200000 > sonuc/o2/U129_olay_gw_kesif.txt
  python3 -I src/olay_gw.py sonuc/o2/M129_cekirdek.txt 1490 100000 > sonuc/o2/M129_olay_gw_kesif.txt
  python3 -I src/olay_gw.py sonuc/o2/M127_cekirdek.txt 200 136181 C > sonuc/o2/M127_olay_gw_C_kesif.txt
  python3 -I src/olay_gw.py sonuc/o2/M129_cekirdek.txt 1490 1142926 C > sonuc/o2/M129_olay_gw_C_kesif.txt
fi
