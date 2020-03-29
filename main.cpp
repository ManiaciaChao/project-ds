#include <iostream>
#include <fstream>
#include "CNF.h"
#include "DPLL.h"
#include <ctime>

#include "Strategy.

void log_time(clock_t t1, clock_t t2, const std::string &prompt) {
  std::cout << prompt << " takes " << static_cast<double>((t2 - t1) * 1000) / CLOCKS_PER_SEC << "ms" << std::endl;
}

int main() {
//  std::ifstream f;
//  f.open("input");
  auto t1 = clock();
  auto cnf = CNF{std::cin};
  auto t2 = clock();
  log_time(t1,t2,"Deserialization");
  t1 = clock();
  auto dpll = DPLL{cnf};
  dpll.solve();
  t2 = clock();
  log_time(t1,t2,"Solving SAT");
  return 0;
}