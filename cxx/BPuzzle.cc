//
// Created by user24 on 2020/3/29.
//

#include "BPuzzle.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <stack>
#include <string>

std::vector<std::vector<int>> all_combinations(int N, int K) {
  std::string bitmask(K, 1);  // K leading 1's
  bitmask.resize(N, 0);       // N-K trailing 0's
  std::vector<std::vector<int>> combinations;
  do {  // permute bitmask
    std::vector<int> combination;
    for (int i = 0; i < N; ++i) {  // [0..N-1] integers
      if (bitmask[i]) {
        combination.push_back(i + 1);
      }
    }
    combinations.push_back(combination);
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
  return combinations;
}

std::vector<std::vector<int>> n2_offset_table(int size) {
  auto combinations = all_combinations(size, 2);
  auto table = std::vector<std::vector<int>>{};
  table.resize(size + 1);
  table.clear();
  for (auto &sub : table) {
    sub.resize(size + 1);
    sub.clear();
  }
  int i = 0;
  for (const auto &combination : combinations) {
    auto x = combination[0];
    auto y = combination[1];
    table[x][y] = i;
    table[y][x] = i;
    i++;
  }
  return table;
}

void BPuzzle::init() {
  for (int i = 0; i < size * size; i++) {
    cnf.literals.push_back(Literal{i});
  }
  auto combinations = all_combinations(size, 2);
  table = std::vector<std::vector<int>>{};
  table.resize(size + 1);
  //  table.clear();
  for (auto &sub : table) {
    sub.resize(size + 1);
    //    sub.clear();
  }
  int i = 0;
  for (const auto &combination : combinations) {
    auto x = combination[0];
    auto y = combination[1];
    table[x][y] = i;
    table[y][x] = i;
    i++;
  }
  offset.n2 = combinations.size();
  offset.sxy = size * size;
  offset.sxyu = offset.sxy + 2 * offset.n2;
  offset.sxyub = offset.sxyu + offset.n2 * 2 * size;
  apply_constraint1();
  apply_constraint2();
  apply_constraint3();
}

BPuzzle::BPuzzle(std::basic_istream<char> &stream) {
  int x, y, val, pol;
  stream >> size;
  init();
  while (!stream.eof()) {
    stream >> x;
    stream >> y;
    stream >> val;
    pol = val == 1 ? 1 : -1;
    cnf.add_clause({Literal{flatten(x, y), pol}});
  }
}

std::array<int, 2> BPuzzle::to_pos(int id) {
  if (id >= size * size) {
    return {0, 0};
  } else {
    int x = (id + 1) % size;
    int y = (id / size) + 1;
    if (x == 0) {
      x = size;
    }
    return {x, y};
  }
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
 * (s,x,y,u,b) -> size^2 + 2 * C(size, 2) + 2 * size * C(size, 2) + [1, 4 * size
 * * C(size, 2)]
 * */
int BPuzzle::flatten(int s, int x, int y) {
  return offset.sxy + 2 * table[x][y] + s - 1;
}
int BPuzzle::flatten(int s, int x, int y, int u) {
  auto n2_size = all_combinations(size, 2).size();
  return offset.sxyu + table[x][y] * 2 * size + 2 * (u - 1) + s - 1;
}
int BPuzzle::flatten(int s, int x, int y, int u, int b) {
  auto n2_size = all_combinations(size, 2).size();
  return offset.sxyub + table[x][y] * 4 * size + 4 * (u - 1) + 2 * (s - 1) + b;
}

void BPuzzle::apply_constraint1() {
  auto polars = {1, -1};
  for (auto polar : polars) {
    for (int l = 1; l <= size; l++) {
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
  for (auto polar : polars) {
    for (int i = 1; i <= size; i++) {
      auto combinations = all_combinations(size, size / 2 + 1);
      for (const auto &combination : combinations) {
        Clause r_clause, c_clause;
        for (auto var : combination) {
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
  cnf.literals.resize(offset.sxyub + offset.n2 * size * 4);
  for (int s = 1; s <= 2; s++) {
    for (int x = 1; x <= size; x++) {
      for (int y = x + 1; y <= size; y++) {
        Clause clause3_1;
        clause3_1.push_back(Literal{flatten(s, x, y), -1});
        for (int u = 1; u <= size; u++) {
          clause3_1.push_back(Literal{flatten(s, x, y, u), -1});
          Clause clause3_2 = {Literal{flatten(s, x, y), 1},
                              Literal{flatten(s, x, y, u), 1}};
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
          clause_4_3.push_back(Literal{flatten(s, x, y, u, 0), 1});
          clause_4_3.push_back(Literal{flatten(s, x, y, u, 1), 1});
          cnf.add_clause(clause_4_1);
          cnf.add_clause(clause_4_2);
          cnf.add_clause(clause_4_3);
        }
        cnf.add_clause(clause3_1);
      }
    }
  }
}

bool has(const std::set<int> &set, int x) { return set.find(x) != set.end(); }
bool BPuzzle::test_space() {
  auto set = std::set<int>{};

  for (int x = 1; x <= size; x++) {
    for (int y = x + 1; y <= size; y++) {
      if (has(set, flatten(x, y))) {
        printf("x:%d y:%d -> %d\n", x, y, flatten(x, y));
        return false;
      }
      set.insert(flatten(x, y));
      for (int s = 1; s <= 2; s++) {
        if (has(set, flatten(s, x, y))) {
          printf("s:%d x:%d y:%d -> %d\n", s, x, y, flatten(s, x, y));
          return false;
        }
        set.insert(flatten(s, x, y));
        for (int u = 1; u <= size; u++) {
          if (has(set, flatten(s, x, y, u))) {
            printf("s:%d x:%d y:%d u:%d -> %d\n", s, x, y, u,
                   flatten(s, x, y, u));
            return false;
          }
          set.insert(flatten(s, x, y, u));
          for (int b = 0; b < 2; b++) {
            if (has(set, flatten(s, x, y, u, b))) {
              printf("s:%d x:%d y:%d u:%d b:%d -> %d\n", s, x, y, u, b,
                     flatten(s, x, y, u, b));
              return false;
            }
            set.insert(flatten(s, x, y, u, b));
          }
        }
      }
    }
  }
  return true;
}

void BPuzzle::print(const CNF &f, bool convert_to_pos) {
  putchar('[');
  std::for_each(f.literals.begin(), f.literals.end(), [&](const Literal &l) {
    if (l.id >= size * size) {
      return;
    }
    if (convert_to_pos) {
      auto pos = to_pos(l.id);
      printf("[%d, %d, %d]", pos[0], pos[1], l.val);
    } else {
      printf("%d", l.val);
    }

    if (l.id + 1 < size * size) {
      putchar(',');
    }
  });
  putchar(']');
}

bool BPuzzle::solve(bool should_print) {
  auto dpll = DPLL{cnf};
  auto status = dpll.solve();
  if (status == done) {
    if (should_print) {
      result = dpll.result_cnf;
      print(result);
    }
    return true;
  } else {
    return false;
  }
}

CNF BPuzzle::generate(int init_size) {
  size = init_size;
  cnf = CNF{};  // cnf is clean;
  init();
  std::random_device rd;
  std::mt19937 e(rd());
  std::bernoulli_distribution p;
  std::uniform_int_distribution<int> random_pos(1, size);
  int len = size * 2;
  std::vector<Clause> assigned = {};
  while (true) {
    auto cur_cnf = cnf;  // avoid pollute clean cnf
    for (int i = 0; i < size * 2; i++) {
      while (true) {
        int rd_p = p(e) ? 1 : -1;
        int rd_x = random_pos(e);
        int rd_y = random_pos(e);
        int rd_id = flatten(rd_x, rd_y);
        if (cur_cnf.literals[rd_id].pol != 0) {
          continue;
        }
        auto literal = Literal{rd_id, rd_p};
        cur_cnf.add_clause({literal});
        break;
      }
    }
    auto dpll = DPLL{cur_cnf};
    if (dpll.solve() == Status::done) {
      // however, current cnf are dirty and need to be dropped
      for (int i = 0; i < size * size; i++) {
        auto &src_l = dpll.result_cnf.literals[i];
        auto pol = src_l.val == positive ? 1 : -1;
        assigned.push_back({Literal{src_l.id, pol}});
      }
      break;  // this selections works
    }
  }
  // here cnf is already filled, so this is an end game
  // now we are gonna dig holes in the board to get an initial game
  // assigned literals are stored at vector `assigned` in the form of clause
  auto cur_cnf = cnf;
  for (const auto &clause : assigned) {
    cur_cnf.add_clause(clause);  // assigned values
  }
  //  auto dpll = DPLL{cur_cnf};
  //  auto status = dpll.solve(); // should holdable till now
  //  printf("%d", status);
  auto &clauses = cur_cnf.clauses;
  for (auto ri = clauses.rbegin(); (*ri)[0].id < size * size; ri++) {
    auto &literal = (*ri)[0];
    literal.pol = -literal.pol;  // assigned opposite values
    auto dpll = DPLL{cur_cnf};
    auto status = dpll.solve();
    if (status == unholdable) {  // it can be dug out, if unholdable
      //      printf("remove %d\n",literal.id);
      // link:
      // https://stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator
      clauses.erase(std::next(ri).base());  // could be tricky here
      ri++;
    }
  }
  for (auto ri = clauses.rbegin(); (*ri)[0].id < size * size; ri++) {
    auto &literal = (*ri)[0];
    // printf("%d %d\n",literal.id,literal.pol);
    cur_cnf.literals[literal.id].val = literal.pol == 1 ? positive : negative;
  }
  cnf = cur_cnf;
  return cnf;
}