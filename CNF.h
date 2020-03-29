//
// Created by user24 on 2020/3/29.
//
#pragma
#ifndef CP_DS_CNF_H
#define CP_DS_CNF_H

#include <vector>
#include <string>
#include <set>

enum Status {
  holdable,
  unholdable,
  pending,
  done
};

enum Value {
  negative = 0,
  positive = 1,
  undefined = -1,
};

struct Literal {
  int id;
  int pol = 0;
  Value val = undefined;
  int count = 0;
};


class CNF {
 public:
  std::vector<Literal> literals;
  std::vector<std::vector<Literal>> clauses;
  explicit CNF(std::basic_istream<char> &stream);
  CNF(const CNF &src);
  std::string to_string();
//  void print();
};

#endif //CP_DS_CNF_H
