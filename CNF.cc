//
// Created by user24 on 2020/3/29.
//

#include <iostream>
#include "CNF.h"

CNF::CNF(std::basic_istream<char> &stream) {
  char c;
  int literal_len, clause_len;
  while (!stream.eof()) {
    stream >> c;
    if (c == 'c') { // 'c' for comment
      stream.ignore(256, '\n');
    } else {
      stream.ignore(5); // 5 is the length of string " cnf "
      break;
    }
  }
  stream >> literal_len;
  stream >> clause_len;
  literals.resize(literal_len);
  clauses.resize(clause_len);

  for (int i = 0; i < literals.size(); i++) {
    literals[i] = Literal{i};
  }

  int literal;
  for (auto &clause:clauses) {
    while (true) {
      stream >> literal;
      if (literal != 0) {
        auto pol = literal > 0 ? 1 : -1;
        clause.push_back(Literal{abs(literal) - 1,pol});
        literals[abs(literal) - 1].count++;
        literals[abs(literal) - 1].pol += pol;
      } else { // 0 for EOL
        break;
      }
    }
  }
}

CNF::CNF(const CNF &src) {
  literals = src.literals;
  clauses = src.clauses;
}
std::string CNF::to_string() {
  std::string str = "p cnf ";
  str += std::to_string(literals.size())
      + " "
      + std::to_string(clauses.size())
      + "\n";
  for (const auto &clause:clauses) {
    for (const auto &literal:clause) {
      if (literal.pol<0) {
        str += "-";
      }
      str += std::to_string(literal.id+1) + " ";
    }
    str += "0\n";
  }
  return str;
}
void CNF::add_clause(const Clause &clause) {
  clauses.push_back(clause);
  for (const auto &literal:clause) {
    literals[literal.id].count++;
    literals[literal.id].pol += literal.pol;
  }

}
#include "CNF.h"
