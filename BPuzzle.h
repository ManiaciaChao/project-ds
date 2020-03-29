//
// Created by user24 on 2020/3/29.
//

#ifndef CP_DS__BPUZZLE_H_
#define CP_DS__BPUZZLE_H_

#include "CNF.h"

using Board = std::vector<std::vector<bool>>;

class BPuzzle {
 private:
  int size = 4;
  CNF cnf;
  Board board {};
 private:
  inline int flatten(int i, int j) { return (i - i) * size + j - 1; };
  void constraint1(); // no 3 consecutive same values for each row/column
  void constraint2(); // number of 1 or 0 should be same for each row/column
  void constraint3(); // no repeated row/column
 public:
  BPuzzle() : cnf(CNF{}) {}
  explicit BPuzzle(CNF &cnf) : cnf(cnf) {}
  void init();
  void generate_cnf(std::string str);
};

#endif //CP_DS__BPUZZLE_H_
