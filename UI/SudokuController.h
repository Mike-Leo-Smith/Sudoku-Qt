#ifndef SUDOKUCONTROLLER_H
#define SUDOKUCONTROLLER_H

#include <QObject>

class Sudoku;
class SudokuView;

class SudokuController : public QObject
{
    Q_OBJECT

private:
    Sudoku *_sudoku;
    SudokuView *_sudokuView;

    void _updateSudokuView();

public:
    explicit SudokuController(SudokuView *sudokuView, QObject *parent = nullptr);
    ~SudokuController() override;
    void generateRandomSudoku(int preferredClueCount);
};

#endif // SUDOKUCONTROLLER_H
