#include <QFuture>
#include "SudokuController.h"
#include "SudokuView.h"
#include "../Engine/Sudoku.h"
#include "../Engine/SudokuCreator.h"
#include "../Engine/SudokuSolver.h"

SudokuController::SudokuController(SudokuView *sudokuView, QObject *parent)
    : QObject(parent), _sudoku(nullptr), _sudokuView(sudokuView)
{

}

void SudokuController::onGenerateRandomSudoku(int preferredClueCount)
{
    delete _sudoku;
    _sudoku = SudokuCreator::random(preferredClueCount);
    //QFuture<bool> future = QtConcurrent::run([]{SudokuSolver::solve(*_sudoku);});
    _updateSudokuView();
}

void SudokuController::_updateSudokuView()
{
    _sudokuView->clearAllCells();
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            auto numbers = _sudoku->getNumbersInCell(row, col);
            auto isPreset = _sudoku->isPreset(row, col);
            SudokuView::NumberCell numberCell = {
                .row = row,
                .col = col,
                .color = isPreset ? QColor(60, 50, 70) : Qt::red,
                .numbers = SudokuView::NumberList::fromStdVector(numbers)
            };
            _sudokuView->addNumberCell(numberCell);
        }
    }
    _sudokuView->update();
}
