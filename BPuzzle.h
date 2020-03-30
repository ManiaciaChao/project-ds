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
  std::vector<std::vector<int>> table;
  int offset_n2 = 0;
  int offset_sxy = 0;
  int offset_sxyu = 0;
  int offset_sxyub = 0;
  Board board{};
 private:
/**
  * s -> {1, 2}
  * x -> [1, size]
  * y -> [1, size]
  * u -> [1, size]
  * b -> {0, 1}
  * (x,y) -> [1, size^2]
  * (s,x,y) -> size^2 + [1, 2 * C(size, 2)]
  * (s,x,y,u) -> size^2 + 2 * C(size, 2) + [1, 2 * size * C(size, 2)]
  * (s,x,y,u,b) -> size^2 + 2 * C(size, 2) + 2 * size * C(size, 2) + [1, 4 * size * C(size, 2)]
  * */
  inline int flatten(int x, int y) { return (x - 1) * size + y - 1; };
  int flatten(int s, int x, int y);
  int flatten(int s, int x, int y, int u);
  int flatten(int s, int x, int y, int u, int b);

  void constraint1(); // no 3 consecutive same values for each row/column
  void constraint2(); // number of 1 or 0 should be same for each row/column
  void constraint3(); // no repeated row/column
 public:
  BPuzzle();
  explicit BPuzzle(CNF &cnf) : cnf(cnf) {}
  void init();
  void generate_cnf(std::string str);
};

#endif //CP_DS__BPUZZLE_H_
