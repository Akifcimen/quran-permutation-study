# Ek — (B) 3.000.000 koşusunun sürdürülmesi ve dondurmadan sonra yazılan çözümleme kodu (31 Ekim 2026, 16:25Z)

1. **Çözümleme kodu.** `kos/analiz.py` ön-kayıt dondurulduktan sonra yazıldı (16:00Z civarı). Ön-kaydın §2–§3'teki tanımlarını uygular: dilim ortalamaları, ilk − son dilim farkı, bölütler arası standart hata, delta yöntemiyle oran aralıkları. Hiçbir sonuca bakılmadan yazıldı.

2. **(B) 3.000.000 koşusunun sürdürülmesi.** Sayaç, kapı çalışmasının yarış düzeltmeli `kontrol_h.c`'siydi. O düzeltmede kilit bütün sıfırlarda `orn_init`'i sarıyordu. Uzun ısınmada kurulum bölüt başına ~1 s sürdüğü için 12 iş parçacığı kurulumu sırayla bekledi; koşu tek çekirdekte yürüdü.
   - Koşu 274 bölütte durduruldu.
   - İlk sürdürme denemesi yanlışlıkla yamasız kaynakla derlendi; 303. bölütte durduruldu. Günlükteki "bitti" satırları geçersizdir; günlüğe not düşüldü.
   - Ardından kilit yalnız L sıfırı için olacak biçimde yamalanmış kaynakla sürdürüldü (`src/kontrol_h.c`: `if(SIF=='L')` kilit).
   - Kilit sonuçları etkilemez. Bunu sınamak için bölüt 1–12, ilk (tam kilitli) koşunun çıktısıyla yamalı sayaçla yeniden sayıldı: 0 farklı.

3. **Kapı çalışması ve K×10 için not.** Yarış düzeltmeli sayaçlarda tam kilit, normal ısınmada (30.000) kuruluma bölüt başına milisaniyeler ekler; sayımları etkilemez. K×10 çalışmasında kilit yalnız L için olacak biçimde kullanılır.
