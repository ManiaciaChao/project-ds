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

BPuzzle::BPuzzle() {
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
  offset_sxy = size * size;
  offset_sxyu = offset_sxy + 2*table.size();
  offset_sxyub = offset_sxyu + table.size() * 2 * size;
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
int BPuzzle::flatten(int s, int x, int y,int u,int b) {
  auto n2_size = all_combinations(size, 2).size();
  return offset_sxyub + table[x][y] * 4 * size + 4 * (u-1) + 2 * (s-1) + b;
}

void BPuzzle::generate_cnf(std::string str) {
//  cnf.literals
}

void BPuzzle::constraint1() {
  auto polars = {1, -1};
  for (auto polar:polars) {
    for (int l = 1; l <= size - 2; l++) {
      for (int s = 1; s <= size - 2; s++) {
        Clause r_clause, c_clause;
        for (int i = 0; i < 3; i++) {
          r_clause.push_back(Literal{flatten(l, s + i), polar});
          cnf.literals[flatten(l, s + i)].count++;
          c_clause.push_back(Literal{flatten(s + i, l), polar});
          cnf.literals[flatten(s + i, l)].count++;
        }
        cnf.clauses.push_back(r_clause);
        cnf.clauses.push_back(c_clause);
      }
    }
  }
}

void BPuzzle::constraint2() {
  auto polars = {1, -1};
  for (auto polar:polars) {
    for (int i = 1; i <= size; i++) {
      auto combinations = all_combinations(size, size / 2 + 1);
      for (const auto &combination:combinations) {
        for (auto var:combination) {
          Clause r_clause, c_clause;
          r_clause.push_back(Literal{flatten(i, var), polar});
          cnf.literals[flatten(i, var)].count++;
          c_clause.push_back(Literal{flatten(var, i), polar});
          cnf.literals[flatten(var, i)].count++;
          cnf.clauses.push_back(r_clause);
          cnf.clauses.push_back(c_clause);
        }
      }
    }
  }
}

void BPuzzle::constraint3() {
  auto id_start = size * size;


}

