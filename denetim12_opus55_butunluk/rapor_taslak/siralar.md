## 4. Kur'an'ın sırası (her sıfır, her puan)

Sıra p = (k + 1)/(N + 1). **Bilgi puanı:** k_alt / k_nokta / k_üst = S_alt / S_nokta / S_üst değeri Kur'an'ın puanına eşit ya da
büyük olan sınama kitabı sayısı (N = 10⁸ sınama kitabı; tablolar ayrı tohumlu 10⁸ kitaplık referans örnekleminden); p_nokta k_nokta'dan,
köşeli parantezde k_nokta için %95 Clopper–Pearson ve çözülmemiş aramalardan doğan belirsizlik aralığı [(k_alt+1)/(N+1),
(k_üst+1)/(N+1)] (§5). **Sayım puanı (kapı G):** k / N, N = referans + sınama = 2×10⁸. J10 ikincildir (N_T = 10⁷, sayımda 3×10⁷).

[ANA_TABLO]

- **Uzunluk-sıralı sıfırlarda (M20, Mallows)** Kur'an'ın bütünlüğü her iki puanda **~3×10⁻⁷ mertebesindedir** (2,3–4,7×10⁻⁷);
  tanımları çok farklı iki puan (yüklem sayısı; birlikte sıklık) aynı mertebeyi verir. Veriye merkezli J10 (ikincil) de aynı mertebededir.
- **Tekdüzede** bilgi puanında [U_BILGI], sayım puanında 4,5×10⁻⁷. Tekdüze sıfır Kur'an'ın uzunluk sıralı dizilişini de rastgele
  saydığı için bilgi puanında daha nadirdir; sayım puanında ise tekdüzenin tepesi yozlaşmış (sıfır toplamlı) ailelerle dolar (§6.2) —
  sonradan S0 bunları temizleyince tekdüze 3×10⁻⁸, M20 9×10⁻⁸, Mallows 6×10⁻⁸ olur (§7.3).
- **Bilgi puanının belirsizlik aralığı geniştir** (üst uç ~1,6–1,8×10⁻⁴): referans diliminde hiç görülmemiş maske taşıyan ~16.000–18.000
  kitabın üst sınırı ∞'dur. Bunların neredeyse hepsi eşiğin çok altındadır (S_alt medyanı ~8,5 bit; eşiği geçmek için çözülmemiş
  aramanın %95 üst sınırından medyan 2¹⁹–2²² kat daha nadir olması gerekir); eşiğe 3 bitten yakın olanlar tekdüzede 4, M20'de 25,
  Mallows'ta 31 kitaptır ve S_alt sırasıyla en üsttekiler adanmış kestirimle yeniden puanlandı (tekdüzede bir sıra sapmasıyla; §5). Aralığın üst ucu bir sıklık kestirimi değildir.
- **M20 zincir kümelenmesi** (ön-kayıt §4.2; `sonuc/sira/M129_zincir.txt`): bilgi isabetleri — adanmış düzeltmeden önceki 25 k_nokta
  kitabı; düzeltilmiş 22 bunların alt kümesidir — 25 farklı zincirde (zincir başına en çok 1); sayım isabetleri 59 → 57 zincir (iki
  zincirde ikişer: aynı zincirin ardışık emisyonları). Kümelenme ihmal edilebilir.
