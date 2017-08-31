#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QTimer>
#include "SudokuController.h"
#include "SudokuView.h"
#include "../Engine/Sudoku.h"
#include "../Engine/SudokuCreator.h"
#include "../Engine/SudokuSolver.h"

SudokuController::SudokuController(SudokuView *sudokuView, QObject *parent)
    : QObject(parent), _sudokuView(sudokuView) {}

SudokuController::~SudokuController()
{
    _solvingProgress.waitForFinished();
}

void SudokuController::_setCurrentSudoku(const Sudoku &sudoku)
{
    QTimer::singleShot(0, this, [this] {
        _sudoku = sudoku;
        _updateSudokuView();
    });
}

void SudokuController::generateRandomSudoku(int preferredClueCount)
{
    _playCount++;
    _setCurrentSudoku(SudokuCreator::random(preferredClueCount));
}

void SudokuController::solveCurrentSudoku()
{
    _solvingProgress = QtConcurrent::run([this] {
        auto id = _playCount;
        auto sudoku = _sudoku;
        SudokuSolver::solve(sudoku);
        if (id == _playCount) {
            _setCurrentSudoku(sudoku);
        }
    });
}

void SudokuController::_updateSudokuView()
{
    QTimer::singleShot(0, _sudokuView, [this] {
        _sudokuView->clearAllCells();
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                auto numbers = _sudoku.getNumbersInCell(row, col);
                auto isPreset = _sudoku.isPreset(row, col);
                SudokuView::NumberCell numberCell = {
                    row, col, SudokuView::NumberList::fromStdVector(numbers), (isPreset ? QColor(60, 50, 70) : Qt::red)
                };
                _sudokuView->addNumberCell(numberCell);
            }
        }
    });
    _sudokuView->update();
}
