//
// Created by user24 on 2020/3/29.
//

#include "Strategy.h"

#include <random>
#include <iostream>

namespace Strategies {

int sequential(CNF &cnf) {
  for (auto literal:cnf.literals) {
    if (literal.val == undefined) {
      return literal.id;
    }
  }
  return -1;
}

int frequential(CNF &cnf) {
  std::vector<Literal> filtered;
  auto src = cnf.literals;
  std::copy_if(src.begin(), src.end(), std::back_inserter(filtered),
               [](const Literal &l) {
                 return l.val == undefined;
               });
  std::sort(filtered.begin(), filtered.end(),
            [](const Literal &a, const Literal b) {
              return a.count > b.count;
            }
  );
  if (filtered.empty()){
    return -1;
  }
  return filtered[0].id;
}

int random(CNF &cnf) {
    std::vector<Literal> filtered;
    auto src = cnf.literals;
    std::copy_if(src.begin(), src.end(), std::back_inserter(filtered),
    [](const Literal &l) {
      return l.val == undefined && l.count > 0;
    });
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, filtered.size()-1);
    return filtered[dist(mt)].id;
};

}