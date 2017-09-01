//
// Created by Mike Smith on 29/08/2017.
//

#include <stack>
#include <iostream>
#include "SudokuSolver.h"
#include "Sudoku.h"

bool SudokuSolver::solve(Sudoku &sudoku)
{
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            sudoku.clearNumbersInCell(row, col);
        }
    }
    return _backtrackingEngine(sudoku);
}

bool SudokuSolver::_fillDeterministicCells(Sudoku &sudoku)
{
    bool hasDeterministicBoxes = false;

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (sudoku.countNumbersInCell(row, col) == 0) {
                auto availableNumbers = sudoku.getAvailableNumbersForCell(row, col);
                if (availableNumbers.size() == 1) {
                    sudoku.addNumberToCell(row, col, availableNumbers.front());
                    hasDeterministicBoxes = true;
                }
            }
        }
    }
    return hasDeterministicBoxes;
}

bool SudokuSolver::_backtrackingEngine(Sudoku &sudoku)
{
    Sudoku backup = sudoku;

    while (_fillDeterministicCells(sudoku)) {}

    if (sudoku.isSolved()) {
        return true;
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (sudoku.countNumbersInCell(row, col) == 0) {
                auto availableNumbers = sudoku.getAvailableNumbersForCell(row, col);
                for (auto number : availableNumbers) {
                    sudoku.addNumberToCell(row, col, number);
                    if (_backtrackingEngine(sudoku)) {
                        return true;
                    }
                    sudoku.clearNumbersInCell(row, col);
                }
                sudoku = backup;
                return false;
            }
        }
    }

    sudoku = backup;
    return false;
}
