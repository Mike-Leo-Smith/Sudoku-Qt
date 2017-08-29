//
// Created by Mike Smith on 29/08/2017.
//

#ifndef SUDOKU_SUDOKU_SOLVER_H
#define SUDOKU_SUDOKU_SOLVER_H

class Sudoku;

class SudokuSolver
{
private:
    static bool _fillDeterministicCells(Sudoku &sudoku);
    static bool _backtrackingEngine(Sudoku &sudoku);
    
public:
    SudokuSolver() = delete;
    static bool solve(Sudoku &sudoku);
};

#endif  // SUDOKU_SUDOKU_SOLVER_H
