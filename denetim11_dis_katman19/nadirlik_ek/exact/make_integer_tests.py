#!/usr/bin/env python3
"""Deterministically regenerate independent Python-bigint quotient fixtures."""
import hashlib
from pathlib import Path
import random
import struct
rng=random.Random(202610250691)
output=Path(__file__).resolve().parent/'integer_test_vectors.bin'
def limbs(x):return[(x>>(64*j))&((1<<64)-1)for j in range(3)]
with output.open('wb')as fh:
    for i in range(100000):
        bits=rng.randrange(1,172);denominator=rng.randrange(1,1<<bits);quotient=rng.randrange(720)
        remainder=(0,denominator-1,rng.randrange(denominator))[i%3]
        numerator=denominator*quotient+remainder
        fh.write(struct.pack('<10Q',*limbs(denominator),*limbs(numerator),quotient,*limbs(remainder)))
print(output.name,hashlib.sha256(output.read_bytes()).hexdigest())
