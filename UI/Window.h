#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
    class Widget;
}

class SudokuController;

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window() override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::Widget *ui;
    SudokuController *_sudokuController;
    int _currentDifficulty;

    int _clueCountForSelectedDifficulty() const;

private slots:
    void _initializeGame();
    void _startGame();
    void _pauseGame();
    void _restartGame();
    void _resumeGame();
    void _changeDifficulty(int newDifficulty);
};

#endif // WIDGET_H
