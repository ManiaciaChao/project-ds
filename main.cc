#include <iostream>
#include <fstream>
#include "CNF.h"
#include "DPLL.h"
#include <ctime>

#include "Strategy.h"

void log_time(clock_t t1, clock_t t2, const std::string &prompt) {
  std::cout << prompt << " takes " << static_cast<double>((t2 - t1) * 1000) / CLOCKS_PER_SEC << "ms" << std::endl;
}

int main() {
  std::ifstream f;
  f.open("input");
  auto t1 = clock();
  auto cnf = CNF{f};
  auto t2 = clock();
  log_time(t1,t2,"Deserialization");
  auto dpll = DPLL{cnf};
  t1 = clock();
  dpll.solve();
  t2 = clock();
  log_time(t1,t2,"Solving SAT");
//  std::cout << cnf.to_string() << std::endl;
  return 0;
}