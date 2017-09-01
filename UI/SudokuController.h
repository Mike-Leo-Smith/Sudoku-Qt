#ifndef SUDOKU_CONTROLLER_H
#define SUDOKU_CONTROLLER_H

#include <QObject>
#include <QStack>
#include <QFuture>
#include <QVector>
#include "../Engine/Sudoku.h"

class SudokuView;

class SudokuController : public QObject
{
    Q_OBJECT

private:
    using SudokuStack = QStack<Sudoku>;
    QFuture<void> _solvingProgress;

    int _playCount = 0;
    Sudoku _sudoku;
    SudokuView *_sudokuView;

    SudokuStack _undoStack;
    SudokuStack _redoStack;

private slots:
    void _updateSudokuView();
    void _setCurrentSudoku(const Sudoku &sudoku);

public:
    explicit SudokuController(SudokuView *sudokuView, QObject *parent = nullptr);
    ~SudokuController() override;

public slots:
    void generateRandomSudoku(int preferredClueCount);
    void solveCurrentSudoku();
    void resetCurrentSudoku();
    void toggleNumberInSelectedCell(int number);
    void getHintsForSelectedCell();

signals:
    void sudokuSolved(int id, Sudoku *solvedSudoku);

    void shouldSetMarkedNumbers(QVector<int> numbersInCell);
    void shouldDisableKeyboard();
};

#endif // SUDOKU_CONTROLLER_H
