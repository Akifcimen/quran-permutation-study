# Ek — sayacın dil kapasitesi (31 Ekim 2026)

**Durum.** Akış 16:24:41Z'de M20 düzey 1'in başında durdu. `kontrol_b` tek çağrıda en çok 1.024 dil kabul ediyor (`int cap=1024;`, dizi boyutu). Düzey 1'de 1.988 plasebo dili ve Kur'an'ın dili vardı; program "çok dil" diyerek çıktı. Hiçbir kontrol bölütü sayılmadı, hiçbir sonuç görülmedi.

**Değişiklik.** `yama/kontrol_b.c`'de kapasite 4.096'ya çıkarıldı. Değişen yalnız bu sayıdır. Dizilerin boyutunu belirler; 1.024'ten az dil yüklenen her koşunun çıktısı aynıdır. Dil adları, sayım ve aday kayıtları değişmez; aday kaydındaki dil numarası yalnız Kur'an'ın dilleri için yazılır ve onlar listenin başındadır.

**Sınama (yeniden başlatmadan önce).** Yeni sayaç, kapı çalışmasının Gc M20 düzey 1'ini (bölüt 1–4, 200 dil) birebir vermelidir (süre alanı dışında). Ayrıca K×10'un düzey 1'inde, kapı çalışmasında da bulunan 200 plasebonun sayımları kapı çalışmasınınkilerle aynı çıkmalıdır (ön-kayıt §2'deki yeniden üretim sınaması).
