#ifndef SUDOKUCONTROLLER_H
#define SUDOKUCONTROLLER_H

#include <QObject>

class SudokuController : public QObject
{
    Q_OBJECT
public:
    explicit SudokuController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SUDOKUCONTROLLER_H