//
// Created by user24 on 2020/3/29.
//

#include <iostream>
#include "DPLL.h"
Status DPLL::bcp(CNF &cnf) {
  if (cnf.clauses.empty()) {
    return Status::holdable;
  }
  bool has_unit = true;
  while (has_unit) {
    has_unit = false;
    for (auto const &clause:cnf.clauses) {
      if (clause.empty()) {
        return Status::unholdable;
      } else if (clause.size() == 1) {
        has_unit = true;
        auto &unit = clause[0];
        cnf.literals[unit.id].val = unit.pol > 0 ? positive : negative;
        cnf.literals[unit.id].count = 0;

        Status result = assign(cnf, unit.id);
        if (result == Status::holdable || result == Status::unholdable) {
          return result;
        }
        break; // exit the loop to check for another unit clause from the start
      }
    }
  };

  return Status::pending; // if reached here, the unit resolution ended normally
}

// 5000ms
Status DPLL::assign(CNF &cnf, int id) {
  Value val = cnf.literals[id].val;
  auto &clauses = cnf.clauses;
  for (auto clause = clauses.begin(); clause != clauses.end(); clause++) {
    for (auto literal = clause->begin(); literal != clause->end(); literal++) {
      if (id != literal->id) {
        continue;
      }
      if (val == (literal->pol > 0)) {
        clauses.erase(clause);
        clause--;
        if (cnf.clauses.empty()) {
          return Status::holdable;
        }
        break;
      } else {
        clause->erase(literal);
        literal--;
        if (clauses.empty()) {
          return Status::unholdable;
        }
        break;
      }
    }
  }
  return Status::pending;
}

Status DPLL::perform_dpll(CNF &cnf) {
  Status status = bcp(cnf);
  if (status == holdable) {
    save_result(cnf, status);
    return Status::done;
  } else if (status == unholdable) {
    return Status::pending;
  }
  int var = choose(cnf);
  Value boolean[] = {positive, negative};
  for (int j = 0; j < 2; j++) {
    CNF cur_cnf = cnf;
    auto &var_ref = cur_cnf.literals[var];
    var_ref.val = boolean[(var_ref.pol >= 0) ? j : ((j + 1) % 2)];
    cur_cnf.literals[var].count = 0;
    status = assign(cur_cnf, var);
    if (status == holdable) {
      save_result(cur_cnf, status);
      return Status::done;
    } else if (status == unholdable) {
      continue;
    }
    status = perform_dpll(cur_cnf);
    if (status == done) {
      return status;
    }
  }
  return Status::pending;
}

void DPLL::save_result(CNF &cnf, int status) {
  if (status == holdable) {
    result_cnf = cnf;
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
  auto status = perform_dpll(origin_cnf);
  if (status == pending) {
    save_result(origin_cnf, Status::unholdable);
  }
}