//
// Created by user24 on 2020/3/29.
//

#ifndef CP_DS__STRATEGY_H_
#define CP_DS__STRATEGY_H_

#include <functional>
#include "CNF.h"

using Strategy = std::function<int(CNF &)>;

namespace Strategies {

int sequential(CNF &cnf);
int shuffled(CNF &cnf);
int frequential(CNF &cnf);

}

#endif //CP_DS__STRATEGY_H_
