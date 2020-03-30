//
// Created by user24 on 2020/3/29.
//

#ifndef CP_DS__BPUZZLE_H_
#define CP_DS__BPUZZLE_H_

#include "CNF.h"
#include "DPLL.h"

using Board = std::vector<std::vector<bool>>;

class BPuzzle {
 private:
  int size = 6;
  std::vector<std::vector<int>> table;
  CNF cnf;
  CNF result;
  struct {
    int n2 = 0;
    int sxy = 0;
    int sxyu = 0;
    int sxyub = 0;
  } offset;
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
  inline std::array<int, 2> to_pos(int id);
  inline int flatten(int x, int y) { return (x - 1) * size + y - 1; };
  int flatten(int s, int x, int y);
  int flatten(int s, int x, int y, int u);
  int flatten(int s, int x, int y, int u, int b);
  void apply_constraint1(); // no 3 consecutive same values for each row/column
  void apply_constraint2(); // number of 1 or 0 should be same for each row/column
  void apply_constraint3(); // no repeated row/column
  void print(const CNF &f);
 public:
  BPuzzle() = default;
  explicit BPuzzle(const CNF& f):cnf{f}{};
  explicit BPuzzle(std::basic_istream<char> &stream);
  void init();
  bool test_space();
  void solve();
  CNF generate(int init_size);
  void cnf_to_board();
  inline std::string cnf_string() { return cnf.to_string(); }
};

#endif //CP_DS__BPUZZLE_H_
