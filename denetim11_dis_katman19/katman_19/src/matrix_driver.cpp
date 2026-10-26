#include "matrix.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  MatrixEngine engine;
  if (argc == 2 && std::string(argv[1]) == "--tables") {
    for (int p : MatrixEngine::kPrimes) {
      std::cout << p << ' ';
      for (int t = 0; t < p; ++t)
        for (int d = 0; d < p; ++d)
          std::cout << (engine.maxorder({0, -d, 1, t}, p) ? '1' : '0');
      std::cout << '\n';
    }
    return 0;
  }
  int p;
  MatrixEngine::Matrix a{}, b{};
  while (std::cin >> p >> a[0] >> a[1] >> a[2] >> a[3]
                         >> b[0] >> b[1] >> b[2] >> b[3]) {
    int pidx = 0;
    while (pidx < 29 && MatrixEngine::kPrimes[pidx] != p) ++pidx;
    if (pidx == 29) return 2;
    std::cout << engine.eval(pidx, a, b) << '\n';
  }
}
