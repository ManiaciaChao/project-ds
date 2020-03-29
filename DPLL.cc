//
// Created by user24 on 2020/3/29.
//

#include <iostream>
#include "DPLL.h"

Status DPLL::bcp(CNF &cnf) {
  bool has_unit = false;
  if (cnf.clauses.empty()) {
    return Status::holdable;
  }
  has_unit = true;
  while (has_unit) {
    has_unit = false;
    for (auto &clause:cnf.clauses) {
      if (clause.empty()) {
        return Status::unholdable;
      }
      if (clause.size() == 1) {
        has_unit = true;
        auto unit = clause[0];
        auto val = unit.pol > 0 ? positive : negative;
        cnf.literals[unit.id].count = 0;
        Status status = assign(cnf, unit.id);
        if (status == holdable || status == unholdable) {
          return status;
        }
        break;
      }
    }
  }
  return Status::pending;
}

Status DPLL::assign(CNF &cnf, int id) {
  Value val = cnf.literals[id].val;
  for (auto clause = cnf.clauses.begin(); clause != cnf.clauses.end(); clause++) {
    for (auto literal = clause->begin(); literal != clause->end(); literal++) {
      if (id == literal->id && val == (literal->pol > 0)) {
        cnf.clauses.erase(clause);
        clause--;
        if (cnf.clauses.empty()) {
          return Status::holdable;
        }
        break;
      } else if (id == literal->id && val != (literal->pol > 0)) {
        clause->erase(literal);
        literal--;
        if (cnf.clauses.empty()) {
          return Status::unholdable;
        }
        break;
      }
    }
  }
  return Status::pending;
}

Status DPLL::performDPLL(CNF &cnf) {
  int status = bcp(cnf);
  if (status == holdable) {
    print(cnf, status);
    return Status::done;
  } else if (status == unholdable) {
    return Status::pending;
  }
  int var = 0;
  for (auto literal:cnf.literals) {
    if (literal.val == undefined) {
      var = literal.id;
    }
  }

  std::vector boolean = {positive, negative};
  for (int j = 0; j < 2; j++) {
    CNF cur_cnf = cnf;
    if (cur_cnf.literals[var].pol >= 0) {
      cur_cnf.literals[var].val = boolean[j];
    } else {
      cur_cnf.literals[var].val = boolean[(j + 1) % 2];
    }
    cur_cnf.literals[var].count = 0;
    Status transform_result = assign(cur_cnf, var);
    if (transform_result == Status::holdable) {
      print(cur_cnf, transform_result);
      return Status::done;
    } else if (transform_result == Status::unholdable) {
      continue;
    }
    Status dpll_result = performDPLL(cur_cnf);
    if (dpll_result == Status::done) {
      return dpll_result;
    }
  }
  return Status::pending;
}

void DPLL::print(CNF &cnf, int status) {
  if (status == holdable) {
    std::cout << "SAT" << std::endl;
    std::for_each(cnf.literals.begin(), cnf.literals.end(), [](const Literal &l) {
      std::cout << (l.val ? "" : "-") << l.id + 1 << " ";
    });
    std::cout << " 0";
  } else {
    std::cout << "UNSAT";
  }
  std::cout << std::endl;
}

void DPLL::solve() {
  auto status = performDPLL(cnf);
  if (status == pending) {
    print(cnf, Status::unholdable);
  }
}