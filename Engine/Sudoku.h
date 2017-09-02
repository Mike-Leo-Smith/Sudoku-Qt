//
// Created by Mike Smith on 29/08/2017.
//

#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H

#include <vector>
#include <functional>

class Sudoku
{
    friend class SudokuCreator;

    using Bits = unsigned short;
    using Numbers = std::vector<int>;
    using CellOperationFunction = std::function<void(Sudoku &, int, int)>;

    struct Cell
    {
        Bits bits;
        bool preset;
        bool marked;
    };

private:
    Cell _board[9][9];

    static Bits _convertNumberToBit(int number);
    static Numbers _convertBitsToNumbers(Bits bits);
    static int _countBits(Bits bits);
    static bool _isInBoard(int row, int col);
    static bool _isNumberValid(int number);

    unsigned short _getOccupiedBitsForCell(int row, int col) const;

public:
    Sudoku();
    ~Sudoku() = default;
    Sudoku(Sudoku &&) = default;
    Sudoku(const Sudoku &) = default;
    Sudoku &operator=(Sudoku &&) = default;
    Sudoku &operator=(const Sudoku &) = default;

    bool isSolved() const;
    void print() const;
    bool isImmutable(int row, int col) const;
    bool isMarked(int row, int col) const;
    int countNumbersInCell(int row, int col) const;
    Numbers getNumbersInCell(int row, int col) const;
    Numbers getAvailableNumbersForCell(int row, int col) const;
    Numbers getConflictiveNumbersInCell(int row, int col) const;
    void reset();
    bool toggleCellMark(int row, int col);
    bool clearNumbersInCell(int row, int col);
    bool setNumbersInCell(int row, int col, Numbers numbers);
    bool addNumberToCell(int row, int col, int number);
    bool removeNumberFromCell(int row, int col, int number);
    bool isNumberInCell(int row, int col, int number);
    bool toggleNumberInCell(int row, int col, int number);
    void forEachCell(CellOperationFunction operation);
};

#endif  // SUDOKU_SUDOKU_H
