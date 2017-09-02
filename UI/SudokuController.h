#ifndef SUDOKU_CONTROLLER_H
#define SUDOKU_CONTROLLER_H

#include <QObject>
#include <QStack>
#include <QFuture>
#include <QVector>
#include "../Engine/Sudoku.h"
#include "GameState.h"

class SudokuView;

class SudokuController : public QObject
{
    Q_OBJECT

private:
    using SudokuStack = QStack<QPair<int, Sudoku>>;
    QFuture<void> _solvingProgress;

    int _currentOperationIndex = 0;
    int _currentSudokuIndex = 0;

    GameState _gameState = GameState::initial;

    Sudoku _sudoku;
    SudokuView *_sudokuView;

    SudokuStack _undoStack;
    SudokuStack _redoStack;

private slots:
    void _updateSudokuView();
    void _setCurrentSudoku(const Sudoku &sudoku);
    void _updateSudokuStacks();
    void _resetUndoAndRedoStack();
    bool _isAbleToUndo();
    bool _isAbleToRedo();

public:
    explicit SudokuController(SudokuView *sudokuView, QObject *parent = nullptr);
    ~SudokuController() override;

public slots:
    void generateBlankSudoku();
    void generateRandomSudoku(int preferredClueCount);
    void solveCurrentSudoku();
    void resetCurrentSudoku();
    void toggleNumberInSelectedCell(int number);
    void getHintsForSelectedCell();
    void clearNumbersInSelectedCell();
    void toggleSelectedCellMark();
    void setGameState(GameState state);
    void undo();
    void redo();

signals:
    void sudokuSolved();
    void shouldSetMarkedNumbers(QVector<int> numbersInCell);
    void shouldDisableKeyboard(bool);
    void shouldDisableUndo(bool);
    void shouldDisableRedo(bool);
};

#endif // SUDOKU_CONTROLLER_H
