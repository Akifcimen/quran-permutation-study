## 2. Ne ölçüldü, nasıl (özet; ayrıntı `ONKAYIT.md`)

### 2.1 Bütünlük dili L
Bir kitap = 114 konuma atanmış ayet sayıları a_n. **Mercek** λ = (α, β, p): t = αn + βa, u = αn − βa; 14 doğrusal biçim (α, |β| ∈ {1,2,3},
aralarında asal, β'nin iki işareti; dış çalışmanın 14 biçimi) × 29 asal (3…113) = 406 mercek. **Bölme:** birincil (D1, D2) = (t-paritesi,
n-paritesi). **Düzeyler:** kitap (E⁰/E¹), aile F = {p | t} (yarılar F⁰/F¹), hücreler (yarıların n-paritesiyle bölünmesi), katsayılar q = t/p,
karşı aile H = {p | u} ve birleşim U = F ∪ H. Dilde Kur'an'a özgü sabit yoktur.

20 yüklem, dört motif türünde:

| Tür | Yüklemler |
|---|---|
| (a) bölme dengesi | B1 kitap 57/57; B2 kitap toplamı (eşit toplam ya da n-/a-bileşenine çapraz denge: α·Σ_{E⁰}n = \|β\|·Σ_{E¹}a); F1/F2 aile yarıları eşit sayı/eşit Σq; R0c/R0e, R1c/R1e her satırda hücrelerin eşit sayısı/eşit toplamı |
| (b) modülün geri dönüşü | Qc hücre merkezi = p (Σq = 4p); QS p \| Σq; Qh p \| iki yarı toplamı; QkF k_F = p; QU \|U\| = p; QkH k_H = p; Qa p \| Σa(U); Qn p \| Σn(U); QH p \| Σ(u/p)(H) |
| (c) iki el | W1 F ∩ H = ∅ |
| (d) ardışıklık | K1 dört hücre toplamının farklı değerleri kesintisiz; K2 farklı katsayılar kesintisiz bant |

Permütasyonla değişmeyen (sabit) yüklemler analitik olarak dışlanır. **Kanıtlı gerektirmeler** (sayım puanında ayıklanır): Qc→QS,
Qh→QS, F2∧QS→Qh, QkF∧QkH∧QU→Qn,Qa, **K1∧Qc→F2,R1e** (t-çift satırda q çift olduğundan ardışık hücre toplamları ve toplam 4p yalnız
(p−1, p+1 / p, p) desenine izin verir), p ≥ 5'te K1∧Qh→F2, pozitif biçimlerde Qc∧Qh→F2 ve p = 3'e özgü sonlu-uzay kuralları; kanıtlanamamış
aday kalmadı (yöntem: 3,9×10⁶ kitapta birlikte-görülme taraması, soyut yapılandırma uzaylarının kapsamlı sayımı, kesin bölüntü sayımı,
karşı-örnek araması; çürütülen adaylar ön-kayıtta listelidir).

### 2.2 İki puan
- **(i) Sayım puanı:** bir mercekte kapıdan geçen sağlanmış yüklemlerden kanıtlı gerektirmelerle türetilemeyenlerin en küçük sayısı
  (tekrarsız kredi); kitabın puanı en iyi merceğindeki değer. **Yozlaşma kapısı G** (görevin önerisi): aile ve hücre yüklemleri yalnız
  k_F ≥ 2E (E = 114/p) ve her hücre ≥ 2 üyeliyse sayılır.
- **(ii) Bilgi-ağırlıklı puan (birincil):** I(λ) = I_B + I_boy + I_F + I_W − c(λ). I_F ve I_W, aile-içi (11 yüklem) ve iki-el (6 yüklem)
  maskelerinin **aile boyuna koşullu birlikte** sıklığının −log₂'si; I_B kitap maskesinin (biçim düzeyi) birlikte sıklığı; I_boy yalnız
  k_F = p ise −log₂ P(k_F = p). Gruplar arası çarpım bir yaklaşımdır; grup içindeki bütün cebirsel bağlar tam birlikte sıklıkla hesaplanır.
  Küçük aileler kapıyla değil aile boyuna koşullamayla bedellendirilir (2E kapısı Kur'an'ın aile boyuna tam denk düştüğü için birincil
  bilgi puanında kullanılmadı; duyarlılık değişkesi V1). c(λ) = sadelik önseli (biçim ∝ 2^{−(α+\|β\|−2)}; (1,±1) 2,46 bit … (2,±3) 5,46 bit)
  + asal eşit ağırlık (log₂ 29 = 4,86 bit). Sıklıklar her sıfırın 10⁸ kitaplık bağımsız **referans örnekleminden** (sınama kitaplarından
  ayrı tohum), aile boyu bağlamında; referans diliminde ≥ 10 isabeti olmayan arama "çözülmemiş" sayılır ve puan bir taban değere
  kırpılmaz, [alt, üst] sınırla taşınır (tek yanlı %95 Poisson–CP); S_alt'ı en yüksek belirsiz kitaplar adanmış kestirimle yeniden
  puanlanır (§5).

### 2.3 Sıfırlar, örneklemler, sıra
Tekdüze (U), M20 (birincil sıralı: robotun 64 zincirli S ≤ S_gözlenen zinciri), Mallows (q yalnız terslik sayısına ayarlı), J10 (ikincil,
veriye merkezli). Örnekleyiciler 10. denetiminkilerle bit-bit aynıdır. Sıfır başına referans 10⁸ + sınama 10⁸ kitap (J10: 2×10⁷ + 10⁷;
127 sayımı: 3×10⁷ + 10⁷). Sıra p = (k + 1)/(N + 1), k = #{puan ≥ Kur'an'ınki}; sayım puanında N = referans + sınama, bilgi puanında
N = sınama.
