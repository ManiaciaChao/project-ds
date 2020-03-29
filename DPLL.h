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
  CNF cnf;
  Strategy choose = Strategies::frequential;
 private:
  static Status bcp(CNF &);
  static Status assign(CNF &,int);
  static void print(CNF &, int);
  static Status perform_dpll(CNF &);
 public:
  explicit DPLL(CNF & f) : cnf(f){}
  DPLL(CNF & f, Strategy c) : cnf(f), choose(std::move(c)){}
  void solve();
};

#endif //CP_DS__DPLL_H_
