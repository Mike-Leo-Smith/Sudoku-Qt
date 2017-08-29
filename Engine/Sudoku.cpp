//
// Created by Mike Smith on 29/08/2017.
//

#include <iostream>
#include "Sudoku.h"

Sudoku::Sudoku()
{
    for (auto &row : _board) {
        for (auto &cell : row) {
            cell.bits = 0;
            cell.preset = false;
        }
    }
}

Sudoku::Bits Sudoku::_convertNumberToBit(int number)
{
    return static_cast<Bits>(_isNumberValid(number) ? (1 << (number - 1)) : 0);
}

Sudoku::Numbers Sudoku::_convertBitsToNumbers(Bits bits)
{
    std::vector<int> numbers;
    for (int i = 1; i <= 9; i++) {
        if ((_convertNumberToBit(i) & bits) != 0) {
            numbers.emplace_back(i);
        }
    }
    return std::move(numbers);
}

int Sudoku::_countBits(Bits bits)
{
    // Get the lower 10 bits in case of unexpected higher part.
    constexpr int lowerTenBitsMask = 0x3ff;
    bits &= lowerTenBitsMask;
    
    int count = 0;
    while (bits != 0) {
        bits &= (bits - 1);
        count++;
    }
    return count;
}

bool Sudoku::_isInBoard(int row, int col)
{
    return (row >= 0 && row < 9 && col >= 0 && col < 9);
}

bool Sudoku::_isNumberValid(int number)
{
    return (number >= 1 && number <= 9);
}

std::vector<int> Sudoku::getNumbersInCell(int row, int col) const
{
    if (!_isInBoard(row, col)) {
        return {};
    }
    return _convertBitsToNumbers(_board[row][col].bits);
}

bool Sudoku::isPreset(int row, int col) const
{
    return _isInBoard(row, col) && _board[row][col].preset;
}

bool Sudoku::clearNumbersInCell(int row, int col)
{
    if (!isPreset(row, col)) {
        _board[row][col].bits = 0;
        return true;
    }
    return false;
}

bool Sudoku::addNumberToCell(int row, int col, int number)
{
    if (!isPreset(row, col) && _isNumberValid(number)) {
        _board[row][col].bits |= _convertNumberToBit(number);
        return true;
    }
    return false;
}

bool Sudoku::removeNumberFromCell(int row, int col, int number)
{
    if (!isPreset(row, col) && _isNumberValid(number)) {
        _board[row][col].bits &= ~_convertNumberToBit(number);
        return true;
    }
    return false;
}

std::vector<int> Sudoku::getConflictiveNumbersInCell(int row, int col) const
{
    if (!_isInBoard(row, col)) {
        return {};
    }
    return _convertBitsToNumbers(_getOccupiedBitsForCell(row, col) & _board[row][col].bits);
}

std::vector<int> Sudoku::getAvailableNumbersForCell(int row, int col) const
{
    if (!_isInBoard(row, col)) {
        return {};
    }
    return _convertBitsToNumbers(~_getOccupiedBitsForCell(row, col));
}

inline Sudoku::Bits Sudoku::_getOccupiedBitsForCell(int row, int col) const
{
    Bits occupiedBits = 0;
    
    // Occupied bits in the row.
    for (int c = 0; c < 9; c++) {
        if (c != col) {
            occupiedBits |= _board[row][c].bits;
        }
    }
    
    // Occupied bits in the column.
    for (int r = 0; r < 9; r++) {
        if (r != row) {
            occupiedBits |= _board[r][col].bits;
        }
    }
    
    // Occupied bits in the sub-region.
    int startingRow = row / 3 * 3;
    int startingCol = col / 3 * 3;
    for (int r = startingRow; r < startingRow + 3; r++) {
        for (int c = startingCol; c < startingCol + 3; c++) {
            occupiedBits |= _board[r][c].bits;
        }
    }
    
    return occupiedBits;
}

bool Sudoku::isSolved() const
{
    // Check whether there is exactly one number in each cell.
    for (const auto &row : _board) {
        for (const auto cell : row) {
            if (_countBits(cell.bits) != 1) {
                return false;
            }
        }
    }
    
    // Check conflicts in rows.
    for (const auto &row : _board) {
        Bits occupied = 0;
        for (const auto &cell : row) {
            occupied |= cell.bits;
        }
        if (_countBits(occupied) != 9) {
            return false;
        }
    }
    
    // Check conflicts in columns.
    for (int col = 0; col < 9; col++) {
        Bits occupied = 0;
        for (const auto &row : _board) {
            occupied |= row[col].bits;
        }
        if (_countBits(occupied) != 9) {
            return false;
        }
    }
    
    // Check conflicts in sub-regions.
    for (int regionRow = 0; regionRow < 9; regionRow += 3) {
        for (int regionCol = 0; regionCol < 9; regionCol += 3) {
            Bits occupied = 0;
            for (int row = regionRow; row < regionRow + 3; row++) {
                for (int col = regionCol; col < regionCol + 3; col++) {
                    occupied |= _board[row][col].bits;
                }
            }
            if (_countBits(occupied) != 9) {
                return false;
            }
        }
    }
    
    // All tests passed, puzzle solved.
    return true;
}

int Sudoku::countNumbersInCell(int row, int col) const
{
    if (!_isInBoard(row, col)) {
        return 0;
    }
    return _countBits(_board[row][col].bits);
}

void Sudoku::print() const
{
    int count = 0;
    std::cout << "┌───────┬───────┬───────┐\n";
    for (const auto &row : _board) {
        std::cout << "│";
        for (const auto &cell : row) {
            auto availableNumbers = _convertBitsToNumbers(cell.bits);
            std::cout << " ";
            if (availableNumbers.empty()) {
                std::cout << " ";
            } else if (availableNumbers.size() == 1) {
                std::cout << availableNumbers.front();
            } else {
                std::cout << "?";
            }
            count++;
            if (count % 3 == 0) {
                std::cout << " │";
            }
        }
        
        if (count % 27 == 0) {
            if (count == 81) {
                std::cout << "\n└───────┴───────┴───────┘";
            } else {
                std::cout << "\n├───────┼───────┼───────┤";
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}
