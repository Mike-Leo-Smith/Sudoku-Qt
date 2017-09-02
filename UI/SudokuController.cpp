#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QTimer>
#include "KeyboardView.h"
#include "SudokuController.h"
#include "SudokuView.h"
#include "../Engine/Sudoku.h"
#include "../Engine/SudokuCreator.h"
#include "../Engine/SudokuSolver.h"

SudokuController::SudokuController(SudokuView *sudokuView, QObject *parent)
    : QObject(parent), _sudokuView(sudokuView)
{
    connect(_sudokuView, &SudokuView::cellSelectionToggled, this, [this](int selectedRow, int selectedCol) {
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

void SudokuController::setGameRunning(bool isRunning)
{
    _gameRunning = isRunning;
    _sudokuView->setGameRunning(isRunning);
    _updateSudokuView();
}

void SudokuController::_setCurrentSudoku(const Sudoku &sudoku)
{
    QTimer::singleShot(0, this, [this, sudoku] {
        _sudoku = sudoku;
        _updateSudokuView();
        _sudokuView->deselectCell();
    });
}

void SudokuController::_updateSudokuStacks()
{
    if (!_redoStack.empty()) {
        auto copy = _undoStack.top();
        while (!_redoStack.empty()) {
            _undoStack.push(_redoStack.pop());
        }
        _undoStack.push(copy);
    }
    _undoStack.push(qMakePair(_currentOperationIndex++, _sudoku));
}

void SudokuController::_resetUndoAndRedoStack()
{
    QTimer::singleShot(0, this, [this] {
        _undoStack.clear();
        _redoStack.clear();
    });
}

void SudokuController::generateRandomSudoku(int preferredClueCount)
{
    _currentSudokuIndex++;
    _setCurrentSudoku(SudokuCreator::random(preferredClueCount));
    _resetUndoAndRedoStack();
    _updateSudokuStacks();
}

void SudokuController::generateBlankSudoku()
{
    _currentSudokuIndex++;
    _setCurrentSudoku(SudokuCreator::blank());
    _resetUndoAndRedoStack();
    _updateSudokuStacks();
}

void SudokuController::solveCurrentSudoku()
{
    _solvingProgress = QtConcurrent::run([this] {
        auto id = _currentSudokuIndex;
        auto sudoku = _sudoku;
        SudokuSolver::solve(sudoku);
        if (id == _currentSudokuIndex) {
            _setCurrentSudoku(sudoku);
            _resetUndoAndRedoStack();
        }
    });
}

void SudokuController::_updateSudokuView()
{
    QTimer::singleShot(0, _sudokuView, [this] {
        const auto markedCellColor = QColor(Qt::yellow);
        const auto sameNumberCellColor = QColor(100, 0, 60, 100);
        const auto immutableNumberColor = QColor(Qt::black);
        const auto mutableNumberColor = QColor(Qt::green).darker();

        _sudokuView->reset();
        _sudoku.forEachCell([this, &immutableNumberColor, &mutableNumberColor, &markedCellColor](Sudoku &s, int row, int col) {
            auto numbers = s.getNumbersInCell(row, col);
            auto isImmutable = s.isImmutable(row, col);
            SudokuView::NumberCell numberCell = {
                row, col, SudokuView::NumberList::fromStdVector(numbers), (isImmutable ? immutableNumberColor : mutableNumberColor)
            };
            _sudokuView->addNumberCell(numberCell);

            if (s.isMarked(row, col)) {
                _sudokuView->addHighlightedCell({ row, col, markedCellColor });
            }
        });

        auto selectedRow = _sudokuView->selectedRow();
        auto selectedCol = _sudokuView->selectedCol();
        auto addedNumbers = _sudoku.getNumbersInCell(selectedRow, selectedCol);

        if (!addedNumbers.empty()) {
            _sudoku.forEachCell([this, &sameNumberCellColor, &addedNumbers](Sudoku &s, int row, int col) {
                for (auto number : addedNumbers) {
                    if (s.isNumberInCell(row, col, number)) {
                        _sudokuView->addHighlightedCell({ row, col, sameNumberCellColor });
                        break;
                    }
                }
            });
        }

        if (_sudoku.isImmutable(selectedRow, selectedCol) || !_gameRunning) {
            emit shouldDisableKeyboard(true);
        } else {
            auto checkedKeyIDs = std::move(addedNumbers);
            if (_sudoku.isMarked(selectedRow, selectedCol)) {
                checkedKeyIDs.push_back(static_cast<int>(KeyboardView::FunctionalKeyID::mark));
            }
            emit shouldDisableKeyboard(false);
            emit shouldSetMarkedNumbers(QVector<int>::fromStdVector(checkedKeyIDs));
        }
        emit shouldDisableUndo(!_isAbleToUndo() || !_gameRunning);
        emit shouldDisableRedo(!_isAbleToRedo() || !_gameRunning);
    });
    _sudokuView->update();
}

void SudokuController::resetCurrentSudoku()
{
    _sudoku.reset();
    _resetUndoAndRedoStack();
    _updateSudokuView();
}

void SudokuController::toggleNumberInSelectedCell(int number)
{
    auto selectedRow = _sudokuView->selectedRow();
    auto selectedCol = _sudokuView->selectedCol();
    _sudoku.toggleNumberInCell(selectedRow, selectedCol, number);
    _updateSudokuStacks();
    _updateSudokuView();
}

void SudokuController::getHintsForSelectedCell()
{
    auto selectedRow = _sudokuView->selectedRow();
    auto selectedCol = _sudokuView->selectedCol();
    auto availableNumbers = _sudoku.getAvailableNumbersForCell(selectedRow, selectedCol);
    _sudoku.setNumbersInCell(selectedRow, selectedCol, availableNumbers);
    _updateSudokuStacks();
    _updateSudokuView();
}

void SudokuController::clearNumbersInSelectedCell()
{
    auto selectedRow = _sudokuView->selectedRow();
    auto selectedCol = _sudokuView->selectedCol();
    _sudoku.clearNumbersInCell(selectedRow, selectedCol);
    _updateSudokuStacks();
    _updateSudokuView();
}

void SudokuController::toggleSelectedCellMark()
{
    auto selecetedRow = _sudokuView->selectedRow();
    auto selecetedCol = _sudokuView->selectedCol();
    _sudoku.toggleCellMark(selecetedRow, selecetedCol);
    _updateSudokuStacks();
    _updateSudokuView();
}

void SudokuController::undo()
{
    qDebug() << "Asked to undo...";
    if (_isAbleToUndo()) {
        _redoStack.push(_undoStack.pop());
        _setCurrentSudoku(_undoStack.top().second);
        qDebug() << "    and succeeded.";
    }
}

void SudokuController::redo()
{
    qDebug() << "Asked to redo";
    if (_isAbleToRedo()) {
        auto operation = _redoStack.pop();
        qDebug() << "  operation" << operation.first;
        _setCurrentSudoku(operation.second);
        _undoStack.push(operation);
        qDebug() << "    and succeeded.";
    }
}

bool SudokuController::_isAbleToUndo()
{
    return _undoStack.size() > 1;
}

bool SudokuController::_isAbleToRedo()
{
    return !_redoStack.empty();
}



