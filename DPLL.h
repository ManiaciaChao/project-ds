//
// Created by user24 on 2020/3/29.
//

#ifndef CP_DS__DPLL_H_
#define CP_DS__DPLL_H_

#include <functional>
#include <utility>
#include "CNF.h"
#include "Strategy.h"

class DPLL {
 private:
  CNF origin_cnf;
  Strategy choose = Strategies::frequential;
 public:
  CNF result_cnf;
 private:
  static Status bcp(CNF &);
  static Status assign(CNF &, int);
  Status perform_dpll(CNF &);
  void save_result(CNF &, int);
 public:
  explicit DPLL(const CNF &f) : origin_cnf(f) {}
  DPLL(CNF &f, Strategy c) : origin_cnf(f), choose(std::move(c)) {}
  void solve();
};

#endif //CP_DS__DPLL_H_
