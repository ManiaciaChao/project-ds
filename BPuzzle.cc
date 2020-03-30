//
// Created by user24 on 2020/3/29.
//

#include <algorithm>
#include <iostream>
#include <string>
#include "BPuzzle.h"
#include "CNF.h"

std::vector<std::vector<int>> all_combinations(int N, int K) {
  std::string bitmask(K, 1); // K leading 1's
  bitmask.resize(N, 0); // N-K trailing 0's
  std::vector<std::vector<int>> combinations;
  do { // permute bitmask
    std::vector<int> combination;
    for (int i = 0; i < N; ++i) { // [0..N-1] integers
      if (bitmask[i]) {
        combination.push_back(i + 1);
      }
    }
    combinations.push_back(combination);
    std::cout << std::endl;
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
  return combinations;
}
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

std::vector<std::vector<int>> n2_offset_table(int size) {
  auto combinations = all_combinations(size, 2);
  auto table = std::vector<std::vector<int>>{};
  table.resize(size + 1);
  table.clear();
  for (auto &sub:table) {
    sub.resize(size + 1);
    sub.clear();
  }
  int i = 0;
  for (const auto &combination:combinations) {
    auto x = combination[0];
    auto y = combination[1];
    table[x][y] = i;
    table[y][x] = i;
    i++;
  }
  return table;
}

void BPuzzle::init() {
  cnf = CNF{};
  auto combinations = all_combinations(size, 2);
  table = std::vector<std::vector<int>>{};
  table.resize(size + 1);
  table.clear();
  for (auto &sub:table) {
    sub.resize(size + 1);
    sub.clear();
  }
  int i = 0;
  for (const auto &combination:combinations) {
    auto x = combination[0];
    auto y = combination[1];
    table[x][y] = i;
    table[y][x] = i;
    i++;
  }
  offset_n2 = combinations.size();
  offset_sxy = size * size;
  offset_sxyu = offset_sxy + 2 * offset_n2;
  offset_sxyub = offset_sxyu + offset_n2 * 2 * size;
}

BPuzzle::BPuzzle(std::basic_istream<char> &stream) {
  int x, y, val, pol;
  stream >> size;
  while (!stream.eof()) {
    stream >> x;
    stream >> y;
    stream >> val;
    pol = val == 1 ? 1 : -1;
    cnf.add_clause({Literal{flatten(x, y),pol}});
  }
  init();
//  apply_constraint1();
//  apply_constraint2();
//  apply_constraint3();
}

// (s,x,y) -> size^2 + [1, 2 * C(size, 2)]
int BPuzzle::flatten(int s, int x, int y) {
  return offset_sxy + 2 * table[x][y] + s - 1;
}
// (s,x,y,u) -> size^2 + 2 * C(size, 2) + [1, 2 * size * C(size, 2)]
int BPuzzle::flatten(int s, int x, int y, int u) {
  auto n2_size = all_combinations(size, 2).size();
  return offset_sxyu + table[x][y] * 2 * size + 2 * (u - 1) + s - 1;
}
// (s,x,y,u,b) -> size^2 + 2 * C(size, 2) + 2 * size * C(size, 2) + [1, 4 * size * C(size, 2)]
int BPuzzle::flatten(int s, int x, int y, int u, int b) {
  auto n2_size = all_combinations(size, 2).size();
  return offset_sxyub + table[x][y] * 4 * size + 4 * (u - 1) + 2 * (s - 1) + b;
}

void BPuzzle::generate_cnf(std::string str) {
//  cnf.literals

}

void BPuzzle::apply_constraint1() {
  auto polars = {1, -1};
  for (auto polar:polars) {
    for (int l = 1; l <= size - 2; l++) {
      for (int s = 1; s <= size - 2; s++) {
        Clause r_clause, c_clause;
        for (int i = 0; i < 3; i++) {
          r_clause.push_back(Literal{flatten(l, s + i), polar});
          c_clause.push_back(Literal{flatten(s + i, l), polar});
        }
        cnf.add_clause(r_clause);
        cnf.add_clause(c_clause);
      }
    }
  }
}

void BPuzzle::apply_constraint2() {
  auto polars = {1, -1};
  for (auto polar:polars) {
    for (int i = 1; i <= size; i++) {
      auto combinations = all_combinations(size, size / 2 + 1);
      for (const auto &combination:combinations) {
        Clause r_clause, c_clause;
        for (auto var:combination) {
          r_clause.push_back(Literal{flatten(i, var), polar});
          c_clause.push_back(Literal{flatten(var, i), polar});
        }
        cnf.add_clause(r_clause);
        cnf.add_clause(c_clause);
      }
    }
  }
}

void BPuzzle::apply_constraint3() {
//  s -> {1, 2}
//  x -> [1, size]
//  y -> [1, size]
//  u -> [1, size]
//  b -> {0, 1}
  cnf.literals.resize(offset_sxyub + offset_n2 * size * 4);
  for (int s = 1; s <= 2; s++) {
    for (int x = 1; x <= size; x++) {
      for (int y = 1; y <= size; y++) {
        Clause clause3_1;
        clause3_1.push_back(Literal{flatten(s, x, y), -1});
        for (int u = 1; u <= size; u++) {
          clause3_1.push_back(Literal{flatten(s, x, y, u), -1});
          Clause clause3_2 = {
              Literal{flatten(s, x, y), 1},
              Literal{flatten(s, x, y, u), 1}
          };
          cnf.add_clause(clause3_2);
          for (int b = 0; b < 2; b++) {
            auto pol = b == 1 ? 1 : -1;
            Clause clause_5_1, clause_5_2, clause_5_3;
            clause_5_1.push_back(Literal{flatten(x, u), pol});
            clause_5_1.push_back(Literal{flatten(s, x, y, u, b), -pol});
            clause_5_2.push_back(Literal{flatten(y, u), pol});
            clause_5_2.push_back(Literal{flatten(s, x, y, u, b), -pol});
            clause_5_3.push_back(Literal{flatten(x, u), -pol});
            clause_5_3.push_back(Literal{flatten(y, u), -pol});
            clause_5_3.push_back(Literal{flatten(s, x, y, u, b), pol});
            cnf.add_clause(clause_5_1);
            cnf.add_clause(clause_5_2);
            cnf.add_clause(clause_5_3);
          }
          Clause clause_4_1, clause_4_2, clause_4_3;
          clause_4_1.push_back(Literal{flatten(s, x, y, u), 1});
          clause_4_1.push_back(Literal{flatten(s, x, y, u, 0), -1});
          clause_4_2.push_back(Literal{flatten(s, x, y, u), 1});
          clause_4_2.push_back(Literal{flatten(s, x, y, u, 1), -1});
          clause_4_3.push_back(Literal{flatten(s, x, y, u), -1});
          clause_4_1.push_back(Literal{flatten(s, x, y, u, 0), 1});
          clause_4_2.push_back(Literal{flatten(s, x, y, u, 1), 1});
          cnf.add_clause(clause_4_1);
          cnf.add_clause(clause_4_2);
          cnf.add_clause(clause_4_3);
        }
        cnf.add_clause(clause3_1);
      }
    }
  }

}
