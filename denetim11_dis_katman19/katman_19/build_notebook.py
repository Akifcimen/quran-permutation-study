"""Build, execute and export the reviewable analysis notebook.

This re-analyzes retained simulation outputs; it does not silently rerun or
extend the fixed Monte Carlo budget. run_study.sh regenerates those outputs.
"""
from pathlib import Path
import json
import os
import sys

ROOT = Path(__file__).resolve().parent
if (ROOT / ".deps").exists():
    sys.path.insert(0, str(ROOT / ".deps"))
    os.environ["PYTHONPATH"] = str(ROOT / ".deps") + os.pathsep + os.environ.get("PYTHONPATH", "")
os.environ.setdefault("MPLCONFIGDIR", str(ROOT / ".cache/matplotlib"))
os.environ.setdefault("IPYTHONDIR", str(ROOT / ".cache/ipython"))
os.environ.setdefault("JUPYTER_RUNTIME_DIR", str(ROOT / ".cache/jupyter"))
for key in ["MPLCONFIGDIR", "IPYTHONDIR", "JUPYTER_RUNTIME_DIR"]:
    Path(os.environ[key]).mkdir(parents=True, exist_ok=True)

import nbformat
from nbclient import NotebookClient
from nbconvert import HTMLExporter

md = nbformat.v4.new_markdown_cell
code = nbformat.v4.new_code_cell
cells = [
md("""# Ortak işlem kuralları, 19 ve metinsel sayı dayanağı

**25 Ekim 2026 — tamamlanmış hesapların yeniden üretilebilir incelemesi**

Bu defter üç karşılaştırma modelindeki toplam **3.000.000 sınama** örneğini ve
**600.000 ayrı eğitim** örneğini inceler. Sayı, işlem biçimi, katmanlar arası
parametre paylaşımı, iki ayet sayımı ve matris sunumları aynı sonlu aramada
yarıştırılmıştır. `protocol.json` yeni sonuçlar görülmeden dondurulmuştur;
gözlenen 19 örüntüsü daha önce bilindiğinden bu bir keşif sonrası çalışmadır.

**Temel yorum sınırı:** Puan bir sıralama ölçüsüdür. $2^{-T}$ bir p-değeri değildir.
P-değeri, aynı tam aramanın uygulandığı bağımsız kontrol örneklerinden hesaplanır.
Bu modeller metnin bütün üretim tarihini veya bütün olası matematiksel aramaları
temsil etmez. Metin dayanağı karşılaştırmayı koşullayan yan bilgidir.
"""),
code("""from pathlib import Path
import json, hashlib
import numpy as np
import pandas as pd
from IPython.display import display, Image, Markdown, Math
from analyze import analyze, MODELS, POLICIES, MODEL_LABELS, POLICY_LABELS
ROOT = Path.cwd()
assert (ROOT / 'protocol.json').exists(), 'Defteri paket kök dizininden çalıştırın.'
protocol = json.loads((ROOT/'protocol.json').read_text())
stored = json.loads((ROOT/'results/summary.json').read_text())
summary = analyze(compress=False, plots=True)
assert summary == stored, 'Saklanan sonuçlarla yeniden hesaplanan sonuçlar farklı.'
print('Saklanan sonuçlar bağımsız dosya okumalarıyla yeniden üretildi.')
print('Protokol SHA-256:', hashlib.sha256((ROOT/'protocol.json').read_bytes()).hexdigest())
print('Kaynak commit:', protocol['source_commit'])
"""),
md("""## 1. Birincil sınama sonucu

Eşitlikler aşım sayısına dahildir. Monte Carlo sıra p-değeri
$(1+k)/(1+B)$ olarak hesaplanır. Binom güven aralıkları, sabit eğitim tablolarına
koşullu kuyruk sıklığının belirsizliğini gösterir. Sıfır aşım, sıfır olasılık
anlamına gelmez. Farklı modellerin örnekleri tek bir havuzda birleştirilmez.
"""),
code("""rows = []
for model in MODELS:
    x = summary['models'][model]['policies']['mixture']
    rows.append({'Model': MODEL_LABELS[model], 'Gözlenen puan': x['observed_score'],
                 'En yüksek kontrol': x['control_max'], 'Eşit/aşan': x['hits'],
                 'Kontrol sayısı': x['n'], 'MC p': x['p_plus_one'],
                 '%95 tek yönlü üst sınır': x['upper95_one_sided'],
                 '%95 iki yönlü aralık': x['cp95']})
display(pd.DataFrame(rows))
print('Üç modele karşı en büyük birincil p:', summary['robust_max_p']['mixture'])
display(Image(filename=str(ROOT/'figures/primary_distributions.png')))
"""),
md(r"""## 2. Gözlenen aritmetik yapı ve iki sayımın ayrılması

$n$ konum, $a$ ayet sayısıdır. $E=\{n:n+a\;\mathrm{çift}\}$,
$F=\{n:19\mid n+a\}$, $H=\{n:19\mid n-a\}$.
Sabit toplama şablonu
$L(K)=\left[\begin{smallmatrix}\sum_K n&\sum_K a\\\sum_{K^c}n&\sum_{K^c}a\end{smallmatrix}\right]$
iki farklı seçim kümesine uygulanır: $P=L(E)$, $R=L(F)$.
Gözlenen 127 seçeneği yalnız özgün dokuzuncu sûrenin ayet sayısını iki azaltır.
Kaynak Arapça metin dosyaları değiştirilmemiştir.
"""),
code("""import observed_structure
observed_structure.main()
structure = json.loads((ROOT/'results/observed_structure.json').read_text())
rows = []
for variant in ['129', '127']:
    x = structure[variant]
    rows.append({'Sayım': variant, 'Ayet toplamı': x['total_a'], '|E|': x['E_count'],
                 'E n-toplamı': x['E_sum_n'], 'E dışı a-toplamı': x['notE_sum_a'],
                 '|F|': x['F_count'], '|H|': x['H_count'], '|F∪H|': x['union_count'],
                 'Birleşim a-toplamı': x['union_sum_a'], 'P mertebesi': x['orders']['P'],
                 'R mertebesi': x['orders']['R'], 'PR+RP mertebesi': x['orders']['PR_plus_RP'],
                 '(PR−RP)²=I': x['commutator_square_is_I']})
display(pd.DataFrame(rows))
"""),
md("""### Dört hücre

$F$ içindeki $q=(n+a)/19$ katsayıları aynı çift/tek ayrımının iki değişkende
uygulanmasıyla dört hücreye ayrılır. Her hücre üç üyeli, toplamlar 20, 18, 19,
19'dur. Genel merkez örüntüsü ile merkezin aynı modül olan 19'a eşitliği farklı
yü klem bitleriyle tutulur; bunlardan çıkan toplamlar yeniden bağımsız başarı
olarak sayılmaz.
""".replace('yü klem','yüklem')),
code("""display(pd.DataFrame([{'n+a paritesi': c['t_parity'], 'n paritesi': c['n_parity'],
                      'Sûre konumları': [r[0] for r in c['records']], 'q değerleri': c['q'],
                      'q toplamı': c['q_sum']} for c in structure['127']['cells']]))
for variant in ['129','127']:
    x = structure[variant]
    print(variant, 'P mod19 =', np.array(x['P_mod19']).reshape(2,2).tolist(),
          'R mod19 =', np.array(x['R_mod19']).reshape(2,2).tolist(),
          '(PR−RP)² =', np.array(x['commutator_squared']).reshape(2,2).tolist())
"""),
md("""## 3. Aynı parametrelerin yeniden kullanılması

Her kontrol, 14 doğrusal ifade, 29 tek asal, iki sayım ve sekiz eşdeğerlik
sınıfına indirgenmiş matris sunumuna erişir. Etkin dört koşul grubunun bütün
altkümeleri ve bütün küme bölüntüleri karşılaştırılır. Bir blok içindeki gruplar
aynı parametreleri kullanır ve seçim maliyetini bir kez öder. Bu nedenle ortak
işlem dili ölçüme doğrudan dahildir. Birincil kazananların tüm ayrıntıları:
"""),
code("""for model in MODELS:
    win = summary['models'][model]['policies']['mixture']['winning_program']
    display(Markdown('**'+MODEL_LABELS[model]+'**'))
    print(json.dumps(win, ensure_ascii=False, indent=2))
decomp_path = ROOT/'review/observed_decomposition.json'
if decomp_path.exists():
    decomposition = json.loads(decomp_path.read_text())
    print('Ayrı sayım ve kod maliyeti dökümü: review/observed_decomposition.json')
"""),
md("""## 4. Metindeki sayılar ve 19'un katkısı

Tam Arapça metin, açık tam sayı ifadelerinin tanımlanmış dilsel sınıfı için
taranmıştır. Birleşik sayılar tek değer olarak ele alınır. Ayet adresleri,
örtük ikiller, ebced ve ifade edilmeyen aritmetik sonuçlar aday üretmez.
Tarama bütün muhtemel nicelik anlatımlarını kapsadığı iddiasında değildir.

Aday asal değerler **3, 5, 7, 11, 19**; sayının kendisinin tartışıldığı genel
bağlam sınıfı **3, 5, 7, 19**'dur. 74:30–31, 19'un ve ardından sayının işlevinin
anılmasını belgeler. 2:196 ve 18:22 benzer genel kurala giren başka sayılar
sağlar. İnceleme hipotezden haberdar model desteğiyle yapılmıştır; kör insan
paneli değildir.

Birincil ağırlık: 29 asal üzerinde eşit dağılım, açık sayı değerleri üzerinde
eşit dağılım ve sayı tartışması kümesinde eşit dağılımın üçte birer karışımı.
Bu kural 19'a verilen seçim maliyetini değiştirir. Metinde 19'un bulunması,
sıralama permütasyonlarında sabittir; ayrıca çarpılacak bağımsız olasılık değildir.
"""),
code("""weights = pd.read_csv(ROOT/'semantic/semantic_weights.csv')
display(weights)
print('19 ağırlığı ve kod maliyeti katkısı:', summary['text_weight_19'])
rows=[]
for model in MODELS:
    for policy, label in zip(POLICIES, POLICY_LABELS):
        x=summary['models'][model]['policies'][policy]
        rows.append({'Model':MODEL_LABELS[model], 'Metin kuralı':label,
                     'Gözlenen puan':x['observed_score'], 'Eşit/aşan':x['hits'], 'MC p':x['p_plus_one']})
display(pd.DataFrame(rows))
display(Image(filename=str(ROOT/'figures/text_policy_sensitivity.png')))
"""),
md("""## 5. Katman katkılarının ölçüm sınırı

E1, bütün grupların aynı sayımda herhangi birer parametreyle başarısı;
E2, aynı ifade ve asalda ortak başarı; E3, merkezin bu asala eşitliği;
E4, bu asalın sayı tartışması kümesinde bulunmasıdır. E4⊆E3⊆E2⊆E1.
Bir önceki düzeyden hiç örnek yoksa sonraki düzeyin koşullu nadirliği bu
denemede ölçülmemiştir. Bu, sıfır koşullu olasılık olarak doldurulmaz.
"""),
code("""diagrows=[]
extrows=[]
for model in MODELS:
    d=summary['models'][model]
    for name, x in d['diagnostics'].items():
        diagrows.append({'Model':MODEL_LABELS[model],'Tanı':name,'Sayı':x['hits'],'Deneme':x['n']})
    for name,x in d['conditional_extensions'].items():
        extrows.append({'Model':MODEL_LABELS[model],'Koşullu tanı':name,'Pay':x['hits'],'Payda':x['n'],
                        'Oran':x['rate'],'%95 tek yönlü üst sınır':x['upper95_one_sided']})
display(pd.DataFrame(diagrows))
display(pd.DataFrame(extrows))
"""),
md("""## 6. Doğrulama ve yeniden çalıştırma

- Veri sayımları depo ve Tanzil arasında eşleşti.
- 122.701 matris iz/determinant durumu bağımsız kesin aritmetikle doğrulandı.
- Puan araması 19 yapay örneğin altı politikasında tam taramayla eşleşti.
- Eşit uzunlukları içeren küçük örneklerde koşullu örnekleyici kesin olarak
  kontrol edildi; gerçek örneklerde iki terslik koşulu da doğrulandı.
- Paralel tohumlandırmada bulunan örtüşme sorunu sonuçlara bakılmadan düzeltildi;
  ilk eğitim tabloları emekliye ayrıldı, bütün eğitim yeniden üretildi.
- `review/` altındaki kayıtlar test kapsamını ve düzeltmeleri açıklar.

Paket kökünde `bash run_study.sh` bütün eğitim ve sınamayı baştan çalıştırır.
`python analyze.py --no-compress` saklanan sonuçları tekrar analiz eder.
`python build_notebook.py` bu defteri tekrar çalıştırıp HTML'ye aktarır.
Donanım ve OpenMP takım büyüklüğü kaydedilmiştir. Aynı rastgele diziler için
sekiz iş parçacığı ve kayıtlı tohumlar kullanılmalıdır.

### Birincil kaynaklar

- [Kaynak veri deposu ve sabit commit](https://github.com/Akifcimen/quran-permutation-study/tree/51fe1c0c36f2d481369480800aa84582e0a099e8)
- [Tanzil kaynak metni](https://tanzil.net/download/)
- [Müddessir 74:30–31](https://quran.com/tr/muddessir/30-31)
- [Bakara 2:196](https://quran.com/tr/bakara/196)
- [Kehf 18:22](https://quran.com/tr/kehf/22)
- [Grünwald: Minimum Description Length tutorial](https://arxiv.org/html/math/0406077v1)
- [Phipson ve Smyth: Permutation p-values should never be zero](https://arxiv.org/abs/1603.05766)

**Sonuçların kapsamı:** Sonlu aile ve belirtilen karşılaştırma modelleri içinde
nadirlik; bütün tarihsel araştırma seçimlerinin düzeltilmiş olasılığı veya
metnin kaynağına ilişkin bir posterior olasılık değildir. Tam katman zincirini
destekleyen kontrol örnekleri yoksa her yeni katmanın ayrı çarpanı belirlenemez.
"""),
]

# Keep the direct arithmetic output concise while still executing its audit.
cells[5].source = cells[5].source.replace("observed_structure.main()", "import contextlib, io\nwith contextlib.redirect_stdout(io.StringIO()):\n    observed_structure.main()")
nb = nbformat.v4.new_notebook(cells=cells, metadata={
    "kernelspec": {"display_name": "Python 3", "language": "python", "name": "python3"},
    "language_info": {"name": "python", "version": "3.12"},
})
path = ROOT / "katman_19_analizi.ipynb"
nbformat.write(nb, path)
client = NotebookClient(nb, timeout=180, kernel_name="python3", resources={"metadata":{"path":str(ROOT)}})
client.execute()
nbformat.validate(nb)
assert not any(o.get('output_type')=='error' for c in nb.cells if c.cell_type=='code' for o in c.outputs)
nbformat.write(nb, path)
exporter = HTMLExporter(template_name="lab", embed_images=True,
                        extra_template_basedirs=[str(ROOT / ".deps/share/jupyter/nbconvert/templates")],
                        extra_template_paths=[str(ROOT / ".deps/share/jupyter/nbconvert/templates")])
body, resources = exporter.from_notebook_node(nb)
(ROOT / "katman_19_analizi.html").write_text(body, encoding="utf-8")
record = {"status":"passed", "cells":len(nb.cells), "code_cells":sum(c.cell_type=='code' for c in nb.cells),
          "executed_cells":sum(c.cell_type=='code' and c.execution_count is not None for c in nb.cells),
          "errors":0, "html_bytes":len(body.encode('utf-8')), "image_outputs":sum('image/png' in o.get('data',{}) for c in nb.cells if c.cell_type=='code' for o in c.outputs)}
(ROOT/'review/notebook_validation.json').write_text(json.dumps(record,indent=2)+'\n')
print(json.dumps(record,indent=2))
