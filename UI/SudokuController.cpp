#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include "SudokuController.h"
#include "SudokuView.h"
#include "../Engine/Sudoku.h"
#include "../Engine/SudokuCreator.h"
#include "../Engine/SudokuSolver.h"

SudokuController::SudokuController(SudokuView *sudokuView, QObject *parent)
    : QObject(parent), _sudoku(nullptr), _sudokuView(sudokuView) {}

SudokuController::~SudokuController()
{
    delete _sudoku;
}

void SudokuController::generateRandomSudoku(int preferredClueCount)
{
    qDebug() << "!!!";
    delete _sudoku;
    _sudoku = SudokuCreator::random(preferredClueCount);
    auto copy = *_sudoku;
    QFuture<bool> future = QtConcurrent::run([&copy](){return SudokuSolver::solve(copy);});
    if (future.result()) {
        //*_sudoku = copy;
    }
    _sudoku->addNumberToCell(5, 6, 7);
    _sudoku->addNumberToCell(5, 6, 6);

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
                row, col, SudokuView::NumberList::fromStdVector(numbers), (isPreset ? QColor(60, 50, 70) : Qt::red)
            };
            _sudokuView->addNumberCell(numberCell);
        }
    }
    _sudokuView->update();
}
