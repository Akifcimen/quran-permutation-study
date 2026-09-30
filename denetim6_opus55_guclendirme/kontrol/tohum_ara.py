import numpy as np, subprocess, os
S = np.fromfile('../denetim5_opus55/puan/J10_iddia_mt_sifir.f32', dtype=np.float32).reshape(-1, 9)
target = S[0]
env = dict(os.environ, ROBOT_MINTR='1000', ROBOT_DISABLE="Σw=m·kG|Σv(U) kare/asal-kuvvet")
for seed in list(range(80, 101)) + list(range(1, 80)) + list(range(101, 200)):
    subprocess.run(['./robot', 'score', 'J10', str(seed), '1', '1', 'tab/J10_2M.bin', 'kontrol/ara'], env=env, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    x = np.fromfile('kontrol/ara_sifir.f32', dtype=np.float32).reshape(-1, 9)[0]
    if np.array_equal(x, target):
        print('eşleşen tohum:', seed); break
else:
    print('1–199 arasında eşleşme yok')
