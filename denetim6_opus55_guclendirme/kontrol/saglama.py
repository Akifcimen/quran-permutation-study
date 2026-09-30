# S1–S4 sağlamaları: denetçinin kayıtlı puan dizileri, her iş parçacığının ilk K kitabında birebir yeniden üretilmeli.
import numpy as np, subprocess, os, sys
K = 1000
KIRMA = '../denetim5_opus55/puan'
D2 = "Σw=m·kG|Σv(U) kare/asal-kuvvet"
DX = D2 + "|MX:"
CASES = [  # (etiket, orijinal robot, yamalı robot, sıfır, tohum, th, tablo, disable, kayıtlı dizi)
  ('T_J10_iddia_nomx', 'robot4', 'robot4_m', 'J10', 42, 8, 'tab/T_J10_2M.bin', DX, 'T_J10_iddia_nomx'),
  ('T_U_iddia_nomx',   'robot4', 'robot4_m', 'U',   44, 8, 'tab/T_U_2M.bin',   DX, 'T_U_iddia_nomx'),
  ('D_J10_iddia_nomx', 'robot2', 'robot2_m', 'J10', 22, 8, 'tab/D_J10_2M.bin', DX, 'D_J10_iddia_nomx'),
  ('J10_iddia_mt',     'robot',  'robot_m',  'J10', 90, 7, 'tab/J10_2M.bin',   D2, 'J10_iddia_mt'),
]
def run(binary, null, seed, th, tab, dis, out, mods=None):
    env = dict(os.environ, ROBOT_MINTR='1000', ROBOT_DISABLE=dis)
    if mods: env['ROBOT_MODS'] = mods
    subprocess.run([f'./{binary}', 'score', null, str(seed), str(K*th), str(th), tab, out], env=env, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return np.fromfile(out + '_sifir.f32', dtype=np.float32).reshape(-1, 9), np.loadtxt(out + '_kuran.txt')
ok_all = True
for tag, orig, pat, null, seed, th, tab, dis, saved in CASES:
    S = np.fromfile(f'{KIRMA}/{saved}_sifir.f32', dtype=np.float32).reshape(-1, 9)
    per = len(S) // th
    ref = np.concatenate([S[i*per:i*per+K] for i in range(th)])
    Qref = np.loadtxt(f'{KIRMA}/{saved}_kuran.txt')
    A, QA = run(orig, null, seed, th, tab, dis, f'kontrol/{tag}_orij')
    B, QB = run(pat, null, seed, th, tab, dis, f'kontrol/{tag}_yama')
    C, QC = run(pat, null, seed, th, tab, dis, f'kontrol/{tag}_m19', mods='19')
    s1 = np.array_equal(A, ref) and np.allclose(QA, Qref)
    s2 = np.array_equal(A, B) and np.allclose(QA, QB)
    s3 = np.array_equal(C[:, 3:6], A[:, 3:6]) and bool(np.all(C[:, 1] <= A[:, 1] + 1e-6))
    s4 = abs(QC[1] - QA[1]) < 1e-9 and abs(QC[4] - QA[4]) < 1e-9
    ok_all &= s1 and s2 and s3 and s4
    print(f'{tag:18s} S1(kayıtlı dizi)={s1}  S2(yama=orijinal)={s2}  S3(sabit sütun aynı, en-iyi≤tam)={s3}  S4(Kur\'an T2 {QA[1]:.2f}→{QC[1]:.2f})={s4}'
          f'  | {len(ref)} kitap; m=19 kısıtıyla en-iyi T2 düşen kitap: {int(np.sum(C[:,1] < A[:,1]-1e-6))}')
print('TÜM SAĞLAMALAR:', 'GEÇTİ' if ok_all else 'KALDI')
