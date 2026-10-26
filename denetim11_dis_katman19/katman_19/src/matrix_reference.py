"""Independent exact Python reference for the bounded matrix grammar.

This reference uses direct matrix multiplication and the full GL(2,p) group
order; it does not use the C++ kernel's trace/determinant shortcut for flags.
No study simulation or outcome file is read here.
"""

PRIMES = (3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
          59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113)
IDENTITY = (1, 0, 0, 1)


def prime_factors(value):
    result = []
    q = 2
    while q * q <= value:
        if value % q == 0:
            result.append(q)
            while value % q == 0:
                value //= q
        q += 1
    if value > 1:
        result.append(value)
    return result


def multiply(a, b, p):
    return ((a[0] * b[0] + a[1] * b[2]) % p,
            (a[0] * b[1] + a[1] * b[3]) % p,
            (a[2] * b[0] + a[3] * b[2]) % p,
            (a[2] * b[1] + a[3] * b[3]) % p)


def power(matrix, exponent, p):
    answer = IDENTITY
    base = tuple(v % p for v in matrix)
    while exponent:
        if exponent & 1:
            answer = multiply(answer, base, p)
        base = multiply(base, base, p)
        exponent >>= 1
    return answer


def matrix_order(matrix, p):
    """Return the exact multiplicative order, or None for a singular matrix."""
    matrix = tuple(v % p for v in matrix)
    if (matrix[0] * matrix[3] - matrix[1] * matrix[2]) % p == 0:
        return None
    group_order = (p * p - 1) * (p * p - p)
    assert power(matrix, group_order, p) == IDENTITY
    order = group_order
    for q in prime_factors(group_order):
        while order % q == 0 and power(matrix, order // q, p) == IDENTITY:
            order //= q
    return order


def view(matrix, row_flip=0, column_flip=0):
    return tuple(matrix[(r ^ row_flip) * 2 + (c ^ column_flip)]
                 for r in range(2) for c in range(2))


def flags(matrix_p, matrix_r, p, p_row=0, p_col=0, r_row=0, r_col=0):
    matrix_p = tuple(v % p for v in view(matrix_p, p_row, p_col))
    matrix_r = tuple(v % p for v in view(matrix_r, r_row, r_col))
    pr = multiply(matrix_p, matrix_r, p)
    rp = multiply(matrix_r, matrix_p, p)
    anticommutator = tuple((v + w) % p for v, w in zip(pr, rp))
    commutator = tuple((v - w) % p for v, w in zip(pr, rp))
    target = p * p - 1
    return ((matrix_order(matrix_p, p) == target) |
            ((matrix_order(matrix_r, p) == target) << 1) |
            ((matrix_order(anticommutator, p) == target) << 2) |
            ((multiply(commutator, commutator, p) == IDENTITY) << 3))


def canonical_view(p_row, p_col, r_row, r_col):
    return ((p_col ^ p_row) << 2) | ((r_row ^ p_row) << 1) | (r_col ^ p_row)


def eval8(matrix_p, matrix_r, p):
    packed = 0
    for pc in range(2):
        for rr in range(2):
            for rc in range(2):
                index = pc * 4 + rr * 2 + rc
                packed |= flags(matrix_p, matrix_r, p, 0, pc, rr, rc) << (4 * index)
    return packed


def companion_maxorder_polynomial(trace, determinant, p):
    """Independent quotient-polynomial check, including reducible polynomials.

    A pair (u,v) represents u+v*x modulo x^2-trace*x+determinant.
    Unlike the C++ table builder this routine does not filter discriminants.
    """
    def pmul(a, b):
        vv = a[1] * b[1]
        return ((a[0] * b[0] - determinant * vv) % p,
                (a[0] * b[1] + a[1] * b[0] + trace * vv) % p)

    def ppow(exponent):
        answer, base = (1, 0), (0, 1)
        while exponent:
            if exponent & 1:
                answer = pmul(answer, base)
            base = pmul(base, base)
            exponent >>= 1
        return answer

    target = p * p - 1
    return (ppow(target) == (1, 0) and
            all(ppow(target // q) != (1, 0) for q in prime_factors(target)))
