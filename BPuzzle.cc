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
      for (const auto& combination:combinations) {
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
  // 15711

}