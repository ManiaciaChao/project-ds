cmake_minimum_required(VERSION 3.16)
project(cp_ds)

set(CMAKE_CXX_STANDARD 17)


add_executable(sat_solver
        cxx/sat_solver.cc
        cxx/CNF.cc
        cxx/CNF.h
        cxx/DPLL.cc
        cxx/DPLL.h
        cxx/Strategy.cc
        cxx/Strategy.h)

add_executable(bin_puzzle
        cxx/game_gen.cc
        cxx/BPuzzle.cc
        cxx/BPuzzle.h
        cxx/CNF.cc
        cxx/CNF.h
        cxx/DPLL.cc
        cxx/DPLL.h
        cxx/Strategy.cc
        cxx/Strategy.h)
