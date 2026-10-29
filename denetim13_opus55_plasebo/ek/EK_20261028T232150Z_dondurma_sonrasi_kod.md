# EK — dondurmadan sonra eklenen kod (2026-10-28T23:21:50Z)

**Konu.** Ön-kayıt 2026-10-28T22:33:04Z'de donduruldu (SHA-256 3837c9d8…71a5a). Dondurma anında özeti alınan 30 kaynak dosyanın
(`sonuc/onkayit_kod_sha256.txt`) hepsi bu ekin yazıldığı anda **değişmemiştir** (`shasum -a 256 -c`: 30/30 OK).

**Dondurmadan sonra eklenenler** (hiçbiri bir tanımı, ağırlığı, eşiği, bedeli ya da kuralı değiştirmez; ön-kaydın tarif ettiği yordamı
uygular ya da yalnız raporlama içindir):
- `kontrol/o0_kuran.c` — Ö0'ın Kur'an kısmı (ONKAYIT §7.1).
- `kontrol/o0_bilgi.c`, `kontrol/o0b_b12.c`, `kontrol/o0b_yeni.c` — Ö0 bilgi puanı çapası (§7.2); 12. den.'in tablosu kendi kodlarıyla okunur.
- `src/hedef_ayrinti.c` — hedef başına betimsel ayrıntı (eşik, en iyi mercek, aramaların sayımları, sınıf, L* profili, el dili puanı).
- `src/kuran_profil.c` — Kur'an'ın verilen mercekteki L* profili (§9 b).
- `src/analiz.py` — §5.3–§10'un uygulaması (derinlik kuralı, p_x, Clopper–Pearson, p_kal, model-tabanlı fark, ikinciller, hükümler).
- `src/rakipler.py`, `src/dil_ozet.py`, `src/hesap_saati.py` — raporlama.
- `kos/referans.sh`, `kos/dil.sh`, `kos/kontrol.py`, `kos/akis.sh` — yürütücüler (§6.3 sırası, §5.3 düzeyleri, §4.4 dondurma).

**Sonuca etkisi.** Yok.
