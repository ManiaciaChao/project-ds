//
// Created by user24 on 2020/3/30.
//

#include <iostream>
#include <fstream>
#include "CNF.h"
#include "DPLL.h"
#include "BPuzzle.h"
#include <ctime>

void log_time(clock_t t1, clock_t t2, const std::string &prompt) {
  std::cout << prompt << " takes " << static_cast<double>((t2 - t1) * 1000) / CLOCKS_PER_SEC << "ms" << std::endl;
}

int main() {
  std::ifstream f;
  f.open("input");
  auto puzzle = BPuzzle{std::cin};
  std::cout<<puzzle.cnf_string()<<std::endl;
  return 0;
}