# KESIF-16 ÖN KAYIT: Boy-Sırası İtirazı (İrvin Cemil Schick, 11 Temmuz 2026)

**Statü:** Analizden ÖNCE yazıldı. Çalışmayı inceleyen İrvin Cemil Schick'in (adıyla anılması
13 Temmuz'da kendi izniyle) itirazına cevaben tasarlanmış kontrol. Yazışmanın tam kaydı:
KESIF.md/DISCOVERY.md dürüstlük notu 10.

## İtiraz (aslına sadık özet)
Sûreler kabaca boy (uzunluk) sırasına dizilidir; bu yüzden dizilişin rastgele olmaması zaten
beklenir (tam boy sırası 1/114!) ve bu zorunluluk diğer ölçümleri "bastırır": çift-tek dengesi
vb. yapılar boy-sıralılığın yan ürünü olabilir.

## Ayrıştırma
İki ayrı iddia var: (İ1) "diziliş rastgele değildir" — kabul; bunu zaten iddia etmiyoruz.
(İ2) "çekirdek katmanlar boy-sıralılığa KOŞULLU olarak da sıradandır" — ölçülebilir; bu testin konusu.

## Tanımlar
- Sıralılık istatistiği: S = Σ n·a(n) (küçük S = uzun sûreler önde). Gerçek veri S_obs.
- Katmanlar: kesif11 bataryası BİREBİR (terazi A1, aile B, iç terazi C, İki El W).
- Sıfır-1 (koşulsuz): ayet sayıları konumlara tekdüze rastgele (mevcut çalışmanın sıfırı).
- Sıfır-2 (koşullu): {S(π) ≤ S_obs} kümesi üzerinde tekdüze dağılım — yani "en az gerçek kitap
  kadar boy-sıralı" dizilişler. Örnekleme: transpozisyon-önerili Metropolis (öneri simetrik,
  kısıt göstergesi → durağan dağılım tekdüze), çok-zincir, yakma + seyreltme.

## Analizler
- **A1:** "Saf boy sırası" kitapları (tam azalan / tam artan dizilmiş ayet sayıları) bataryadan
  kaç katman geçirir? (İtirazın limit hâli: sıralılık yapıyı üretiyorsa, EN sıralı kitap en çok üretmeli.)
- **A2:** Koşulsuz MC (10⁷): katman oranları S-dilimlerine (ondalıklara) göre değişiyor mu?
  Ayrıca S_obs'un koşulsuz dağılımdaki z-skoru (İ1'in büyüklüğü — dürüstçe raporlanır).
- **A3 (birincil):** Koşullu MCMC: P(katman | S ≤ S_obs) ÷ P(katman | koşulsuz) oranları.
  Yorum kuralı (önceden): oran ~1 → İ2 reddedilir (sıralılık yapıyı beslemiyor); oran >> 1 →
  itiraz haklı, çekirdek p'si koşullu sıfıra göre yeniden fiyatlanır ve bu deftere yazılır.
- Bileşik J koşullu MCMC ile erişilemez (10⁻¹⁵); katman-bazında sonuç + bağımsızlık argümanı raporlanır.
  Gerekçe: sıralılık BÜYÜKLÜK düzeninde yaşar, katmanlar KALINTI aritmetiğinde (mod 2, mod 19);
  komşu büyüklüklerin kalıntıları pratikte bağımsızdır — bu sav A3 ile sınanır, varsayılmaz.

## Dürüstlük
Sonuç ne çıkarsa deftere girer ve itiraz sahibine bildirilir; itiraz haklıysa çekirdek rapor düzeltilir.
