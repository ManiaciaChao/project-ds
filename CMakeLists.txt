cmake_minimum_required(VERSION 3.16)
project(cp_ds)

set(CMAKE_CXX_STANDARD 17)

add_executable(cp_ds main.cpp CNF.cc CNF.h DPLL.cc DPLL.h Strategy.cc Strategy.h BPuzzle.cc BPuzzle.h)

