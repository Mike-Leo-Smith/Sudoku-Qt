//
// Created by Mike Smith on 29/08/2017.
//

#include <random>
#include "SudokuCreator.h"
#include "Sudoku.h"

Sudoku *SudokuCreator::load(int numbers[9][9])
{
    auto sudoku = new Sudoku;
    
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            auto number = numbers[row][col];
            if (sudoku->_isNumberValid(number)) {
                sudoku->_board[row][col].bits = sudoku->_convertNumberToBit(number);
                sudoku->_board[row][col].preset = true;
            } else {
                sudoku->_board[row][col].bits = 0;
                sudoku->_board[row][col].preset = false;
            }
        }
    }
    return sudoku;
}

Sudoku *SudokuCreator::random(int preferredClueCount)
{
    constexpr int minClueCount = 17;
    constexpr int maxClueCount = 70;
    
    // Constrain the number of clues to make sure the game is playable.
    auto clueCount = std::min(std::max(minClueCount, preferredClueCount), maxClueCount);
    
    std::random_device randomDevice;
    std::default_random_engine randomEngine(randomDevice());
    
    auto randomIndex = [&randomEngine](size_t total) {
        return static_cast<size_t>(randomEngine() / static_cast<double>(randomEngine.max()) * total);
    };
    
    auto attempt = [&randomIndex]() -> Sudoku * {
        auto sudoku = new Sudoku;
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                auto availableNumbers = sudoku->getAvailableNumbersForCell(row, col);
                if (availableNumbers.empty()) {
                    delete sudoku;
                    return nullptr;
                }
                sudoku->addNumberToCell(row, col, availableNumbers[randomIndex(availableNumbers.size())]);
            }
        }
        return sudoku;
    };
    
    Sudoku *finalBoard = nullptr;
    while ((finalBoard = attempt()) == nullptr) {}
    
    std::vector<int> indices(81);
    for (int i = 0; i < 81; i++) {
        indices[i] = i;
    }
    std::shuffle(indices.begin(), indices.end(), randomEngine);
    
    for (int i = 0; i < clueCount; i++) {
        finalBoard->_board[indices[i] / 9][indices[i] % 9].preset = true;
    }
    for (int i = clueCount; i < 81; i++) {
        finalBoard->_board[indices[i] / 9][indices[i] % 9].bits = 0;
    }
    
    return finalBoard;
}
