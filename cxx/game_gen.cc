//
// Created by user24 on 2020/3/30.
//

#include <unistd.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <set>

#include "BPuzzle.h"
#include "CNF.h"

void log_time(clock_t t1, clock_t t2, const std::string &prompt) {
  std::cout << prompt << " takes "
            << static_cast<double>((t2 - t1) * 1000) / CLOCKS_PER_SEC << "ms"
            << std::endl;
}

enum Mode { Unknwon, Solve, Generate };

int main(int argc, char *argv[]) {
  //  std::ifstream f;
  //  f.open("s8_l1_3.txt");
  //  auto puzzle = BPuzzle{f};
  //  auto puzzle = BPuzzle{std::cin};
  //  std::cout << "mapping space test " << (puzzle.test_space() ? "passes" :
  //  "fails") << std::endl;
  auto size = 0;
  auto to_pos = true;
  auto mode = Mode::Unknwon;
  for (;;) {
    if (mode != Mode::Unknwon) {
      break;
    }
    switch (getopt(argc, argv,
                   "n:g:s"))  // note the colon (:) to indicate that 'b' has a
                              // parameter and is not a switch
    {
      case 'n':
        sscanf(optarg, "%d", &size);
        // printf("switch 'n' specified %d \n", size);
        continue;

      case 's': {
        mode = Mode::Solve;
        continue;
      }

      case 'g': {
        auto boolInt = 0;
        sscanf(optarg, "%d", &boolInt);
        to_pos = boolInt ? true : false;
        // printf("parameter 'g' specified with the value %s\n", optarg);
        mode = Mode::Generate;
        continue;
      }

      // case '?':
      // case 'h':
      default:
        // printf("Help/Usage Example\n");
        break;

      case -1:
        break;
    }

    break;
  }

  if (size % 2) {
    std::cout << "illegal size! must be even" << std::endl;
    return -1;
  }

  switch (mode) {
    case Mode::Solve: {
      auto puzzle = BPuzzle{std::cin};
      auto res = puzzle.solve(false);
      std::cout << (res ? "true" : "false") << std::endl;
      break;
    }

    case Mode::Generate: {
      auto puzzle = BPuzzle{};
      auto cnf = puzzle.generate(size);
      puzzle.print(cnf, to_pos);
      break;
    }
    default:
      break;
  }
  // auto puzzle = BPuzzle{};
  // auto cnf = puzzle.generate(4);
  // puzzle.print(cnf);
  // puzzle.solve();
  //  puzzle.solve();
  return 0;
}