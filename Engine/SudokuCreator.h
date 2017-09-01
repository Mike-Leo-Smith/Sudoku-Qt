//
// Created by Mike Smith on 29/08/2017.
//

#ifndef SUDOKU_SUDOKU_CREATOR_H
#define SUDOKU_SUDOKU_CREATOR_H

class Sudoku;

class SudokuCreator
{
public:
    SudokuCreator() = delete;   // Make only static calls available.
    static Sudoku load(int numbers[9][9]);
    static Sudoku random(int preferredClueCount);
    static Sudoku blank();
};

#endif  // SUDOKU_SUDOKU_CREATOR_H
