#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QTimer>
#include "SudokuController.h"
#include "SudokuView.h"
#include "../Engine/Sudoku.h"
#include "../Engine/SudokuCreator.h"
#include "../Engine/SudokuSolver.h"

SudokuController::SudokuController(SudokuView *sudokuView, QObject *parent)
    : QObject(parent), _sudokuView(sudokuView)
{
    connect(_sudokuView, &SudokuView::cellSelected, this, [this](int selectedRow, int selectedCol) {
        if (_sudoku.isImmutable(selectedRow, selectedCol)) {
            _sudokuView->deselectCell();
        }
        _updateSudokuView();
    });
}

SudokuController::~SudokuController()
{
    _solvingProgress.waitForFinished();
}

void SudokuController::_setCurrentSudoku(const Sudoku &sudoku)
{
    QTimer::singleShot(0, this, [this, sudoku] {
        _sudoku = sudoku;
        _sudokuView->deselectCell();
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
                auto isPreset = _sudoku.isImmutable(row, col);
                SudokuView::NumberCell numberCell = {
                    row, col, SudokuView::NumberList::fromStdVector(numbers), (isPreset ? QColor(60, 50, 70) : Qt::red)
                };
                _sudokuView->addNumberCell(numberCell);
            }
        }

        auto selectedRow = _sudokuView->selectedRow();
        auto selectedCol = _sudokuView->selectedCol();

        if (_sudoku.isImmutable(selectedRow, selectedCol)) {
            emit shouldDisableKeyboard();
        } else {
            auto numbersInCell = _sudoku.getNumbersInCell(selectedRow, selectedCol);
            emit shouldSetMarkedNumbers(QVector<int>::fromStdVector(numbersInCell));
        }
    });
    _sudokuView->update();
}

void SudokuController::resetCurrentSudoku()
{
    _sudoku.reset();
    _updateSudokuView();
}

void SudokuController::toggleNumberInSelectedCell(int number)
{
    int selectedRow = _sudokuView->selectedRow();
    int selectedCol = _sudokuView->selectedCol();

    _sudoku.toggleNumberInCell(selectedRow, selectedCol, number);
    _updateSudokuView();
}

void SudokuController::getHintsForSelectedCell()
{
    int selectedRow = _sudokuView->selectedRow();
    int selectedCol = _sudokuView->selectedCol();

    auto availableNumbers = _sudoku.getAvailableNumbersForCell(selectedRow, selectedCol);
    _sudoku.setNumbersInCell(selectedRow, selectedCol, availableNumbers);
    _updateSudokuView();
}
