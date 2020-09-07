#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <functional>
#include <unordered_map>
#include "CNF.h"
#include "DPLL.h"
#include "Strategy.h"

void log_time(clock_t t1, clock_t t2, const std::string &prompt) {
  std::cout << prompt << " takes " << static_cast<double>((t2 - t1) * 1000) / CLOCKS_PER_SEC << "ms" << std::endl;
}

// sat_solver filename [-s r/f/l]
// sat_solver [-s r/f/l] < filename
int main(int argc, char *argv[]) {
  char s = 'f';
  std::string filename;
  bool use_std = true;
  bool print_cnf = false;
  std::unordered_map<char, Strategy> strategies{
      {'r', Strategies::random}, {'f', Strategies::frequential}, {'l', Strategies::linear}
  };
  if (argv[1] != nullptr) {
    if (strcmp(argv[1], "-s") == 0 && argv[2] != nullptr) {
      s = argv[2][0];
    } else {
      filename = argv[1];
      std::cout << filename << std::endl;
      use_std = false;
      if (argv[2] != nullptr &&
          strcmp(argv[2], "-s") == 0 &&
          argv[3] != nullptr) {
        s = argv[3][0];
      }
    }
  }
  if (strcmp(argv[argc - 1], "-c") == 0) {
    print_cnf = true;
  }
  if (use_std && !filename.empty()) {
    return 1;
  }
  auto cnf = CNF{};
  auto t1 = clock();
  if (use_std) {
    cnf = CNF{std::cin};
  } else {
    std::ifstream f;
    f.open(filename);
    cnf = CNF{f};
  }
  auto t2 = clock();
  if (print_cnf){
    std::cout << cnf.to_string();
  }
  log_time(t1, t2, "Deserialization");
  auto dpll = DPLL{cnf, strategies[s]};
  t1 = clock();
  auto status = dpll.solve();
  t2 = clock();
  std::ofstream fileout;
  std::ostream *stream = &std::cout;
  if (!use_std) {
    fileout.open(filename + ".res");
    stream = &fileout;
  }

  if (status == done) {
    *stream << "1" << std::endl;
    std::for_each(dpll.result_cnf.literals.begin(), dpll.result_cnf.literals.end(), [&](const Literal &l) {
      *stream << (l.val ? "" : "-") << l.id + 1 << " ";
    });
  } else {
    *stream << "0";
  }
  *stream << std::endl;
  *stream << static_cast<double>((t2 - t1) * 1000) / CLOCKS_PER_SEC << std::endl;
  log_time(t1, t2, "Solving SAT");
  return 0;
}