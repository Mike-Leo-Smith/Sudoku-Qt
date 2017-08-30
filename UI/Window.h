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
    ~Window();

private:
    Ui::Widget *ui;
    SudokuController *_sudokuController;
    int _currentDifficulty;

    int _clueCountForSelectedDifficulty() const;
    void _setDifficulty(int newDifficulty);
};

#endif // WIDGET_H
