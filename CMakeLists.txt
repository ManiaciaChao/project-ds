cmake_minimum_required(VERSION 3.16)
project(cp_ds)

set(CMAKE_CXX_STANDARD 17)

add_executable(cp_ds main.cc CNF.cc CNF.h DPLL.cc DPLL.h Strategy.cc Strategy.h)

add_executable(bin_puzzle game_gen.cc BPuzzle.cc BPuzzle.h CNF.cc CNF.h DPLL.cc DPLL.h Strategy.cc Strategy.h)
