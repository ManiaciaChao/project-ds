//
// Created by user24 on 2020/3/30.
//

#include <iostream>
#include <fstream>
#include "CNF.h"
#include "BPuzzle.h"
#include <ctime>
#include <set>

void log_time(clock_t t1, clock_t t2, const std::string &prompt) {
  std::cout << prompt << " takes " << static_cast<double>((t2 - t1) * 1000) / CLOCKS_PER_SEC << "ms" << std::endl;
}

int main() {
//  std::ifstream f;
//  f.open("s8_l1_3.txt");
//  auto puzzle = BPuzzle{f};
//  auto puzzle = BPuzzle{std::cin};
//  std::cout << "mapping space test " << (puzzle.test_space() ? "passes" : "fails") << std::endl;

  auto puzzle = BPuzzle{};
  puzzle.generate(4);
//  puzzle.solve();
  return 0;
}