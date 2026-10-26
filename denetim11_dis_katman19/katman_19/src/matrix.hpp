#ifndef JOINT_STUDY_MATRIX_HPP
#define JOINT_STUDY_MATRIX_HPP

#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>

// Exact matrix kernel for the declared odd-prime field grammar.
// Row-major matrices are [a,b,c,d]. Input entries may be unreduced or negative.
// Construction precomputes whether each (trace, determinant) pair has order p^2-1.
// For this order the characteristic polynomial must be irreducible; consequently
// every matrix with that trace/determinant pair is conjugate to its companion.
class MatrixEngine {
 public:
  using Matrix = std::array<int, 4>;
  inline static constexpr std::array<int, 29> kPrimes = {
      3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
      59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113};

  MatrixEngine() { init(); }

  // Idempotent; call before sharing an engine across worker threads.
  void init() {
    if (ready_) return;
    prime_index_.fill(-1);
    for (int pi = 0; pi < static_cast<int>(kPrimes.size()); ++pi) {
      const int p = kPrimes[pi];
      prime_index_[p] = pi;
      auto& table = order_by_td_[pi];
      table.assign(p * p, 0);
      std::vector<uint8_t> square(p, 0);
      for (int r = 0; r < p; ++r) square[(r * r) % p] = 1;
      const int target = p * p - 1;
      const std::vector<int> factors = prime_factors(target);
      for (int t = 0; t < p; ++t) {
        for (int d = 0; d < p; ++d) {
          if (square[mod(t * t - 4 * d, p)]) continue;
          const Matrix companion = {0, mod(-d, p), 1, t};
          if (power(companion, target, p) != identity()) continue;
          bool good = true;
          for (int q : factors) {
            if (power(companion, target / q, p) == identity()) {
              good = false;
              break;
            }
          }
          table[t * p + d] = static_cast<uint8_t>(good);
        }
      }
    }
    ready_ = true;
  }

  bool maxorder(const Matrix& raw, int p) const {
    const int pi = index_for_prime(p);
    const Matrix m = reduce(raw, p);
    const int t = add(m[0], m[3], p);
    const int d = mod(m[0] * m[3] - m[1] * m[2], p);
    return order_by_td_[pi][t * p + d] != 0;
  }

  // Eight canonical views cover all 16 view pairs under simultaneous conjugacy.
  // P's row flip is zero; index = P_colflip*4 + R_rowflip*2 + R_colflip.
  // The nibble at 4*index contains:
  //   bit 0: ord(P)=p^2-1
  //   bit 1: ord(R)=p^2-1
  //   bit 2: ord(PR+RP)=p^2-1
  //   bit 3: (PR-RP)^2=I
  uint32_t eval(int pidx, Matrix raw_p, Matrix raw_r) const {
    if (pidx < 0 || pidx >= static_cast<int>(kPrimes.size()))
      throw std::out_of_range("MatrixEngine prime index");
    const int p = kPrimes[pidx];
    const Matrix a = reduce(raw_p, p);
    const Matrix b = reduce(raw_r, p);
    const auto& table = order_by_td_[pidx];

    const int tp[2] = {add(a[0], a[3], p), add(a[1], a[2], p)};
    const int tr[2] = {add(b[0], b[3], p), add(b[1], b[2], p)};
    const int dp0 = mod(a[0] * a[3] - a[1] * a[2], p);
    const int dr0 = mod(b[0] * b[3] - b[1] * b[2], p);
    const int dp[2] = {dp0, dp0 == 0 ? 0 : p - dp0};
    const int dr[2] = {dr0, dr0 == 0 ? 0 : p - dr0};
    const unsigned op[2] = {table[tp[0] * p + dp[0]], table[tp[1] * p + dp[1]]};
    const unsigned orr[2] = {table[tr[0] * p + dr[0]], table[tr[1] * p + dr[1]]};

    // u[v] = tr(P * view_v(R)) for P in its original view.
    // For P with columns swapped, the corresponding trace is u[v XOR 2].
    const int u[4] = {
        (a[0] * b[0] + a[1] * b[2] + a[2] * b[1] + a[3] * b[3]) % p,
        (a[0] * b[1] + a[1] * b[3] + a[2] * b[0] + a[3] * b[2]) % p,
        (a[0] * b[2] + a[1] * b[0] + a[2] * b[3] + a[3] * b[1]) % p,
        (a[0] * b[3] + a[1] * b[1] + a[2] * b[2] + a[3] * b[0]) % p};

    uint32_t packed = 0;
    for (int pc = 0; pc < 2; ++pc) {
      int coefficient[2], constant[2], comm_target[2];
      for (int rt = 0; rt < 2; ++rt) {
        coefficient[rt] = (tp[pc] * tr[rt]) % p;
        constant[rt] = mod(tp[pc] * tp[pc] * dr[rt] + tr[rt] * tr[rt] * dp[pc], p);
        comm_target[rt] = mod(4 * dp[pc] * dr[rt] + 1, p);
      }
      for (int rv = 0; rv < 4; ++rv) {
        const int rt = (rv >> 1) ^ (rv & 1);
        const int product_trace = u[rv ^ (pc << 1)];
        const int s_trace = add(product_trace, product_trace, p);
        // det(PR+RP) = u^2 - tr(P)tr(R)u + tr(P)^2det(R) + tr(R)^2det(P).
        const int s_det = mod(product_trace * product_trace - coefficient[rt] * product_trace + constant[rt], p);
        // det(PR+RP) + det(PR-RP) = 4det(P)det(R), and tr(PR-RP)=0.
        // Thus (PR-RP)^2=I iff det(PR+RP)=4det(P)det(R)+1.
        const unsigned flags = op[pc] | (orr[rt] << 1) |
                               (static_cast<unsigned>(table[s_trace * p + s_det]) << 2) |
                               (static_cast<unsigned>(s_det == comm_target[rt]) << 3);
        const int view = pc * 4 + rv;
        packed |= static_cast<uint32_t>(flags) << (4 * view);
      }
    }
    return packed;
  }

  static int canonical_view(int p_row, int p_col, int r_row, int r_col) {
    return ((p_col ^ p_row) << 2) | ((r_row ^ p_row) << 1) | (r_col ^ p_row);
  }

 private:
  bool ready_ = false;
  std::array<int, 114> prime_index_{};
  std::array<std::vector<uint8_t>, 29> order_by_td_;

  static int mod(int value, int p) {
    const int r = value % p;
    return r < 0 ? r + p : r;
  }

  static int add(int x, int y, int p) {
    const int result = x + y;
    return result >= p ? result - p : result;
  }

  int index_for_prime(int p) const {
    if (p < 0 || p >= static_cast<int>(prime_index_.size()) || prime_index_[p] < 0)
      throw std::invalid_argument("MatrixEngine only supports its 29 declared odd primes");
    return prime_index_[p];
  }

  static Matrix reduce(const Matrix& raw, int p) {
    return {mod(raw[0], p), mod(raw[1], p), mod(raw[2], p), mod(raw[3], p)};
  }

  static constexpr Matrix identity() { return {1, 0, 0, 1}; }

  static Matrix multiply(const Matrix& a, const Matrix& b, int p) {
    return {(a[0] * b[0] + a[1] * b[2]) % p,
            (a[0] * b[1] + a[1] * b[3]) % p,
            (a[2] * b[0] + a[3] * b[2]) % p,
            (a[2] * b[1] + a[3] * b[3]) % p};
  }

  static Matrix power(Matrix base, int exponent, int p) {
    Matrix answer = identity();
    while (exponent != 0) {
      if (exponent & 1) answer = multiply(answer, base, p);
      exponent >>= 1;
      if (exponent != 0) base = multiply(base, base, p);
    }
    return answer;
  }

  static std::vector<int> prime_factors(int value) {
    std::vector<int> factors;
    for (int q = 2; q * q <= value; ++q) {
      if (value % q != 0) continue;
      factors.push_back(q);
      do { value /= q; } while (value % q == 0);
    }
    if (value > 1) factors.push_back(value);
    return factors;
  }
};

#endif
