#ifndef SUDOKUVIEW_H
#define SUDOKUVIEW_H

#include <QOpenGLWidget>
#include <QMap>
#include <QPoint>
#include <QSet>

class SudokuView : public QOpenGLWidget
{
    Q_OBJECT

public:
    using NumberList = QVector<int>;

    struct NumberCell {
        int row;
        int col;
        NumberList numbers;
        QColor color;
    };

    using NumberCellList = QVector<NumberCell>;

    explicit SudokuView(QWidget *parent = nullptr);
    ~SudokuView() override = default;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void setOuterGridLineWidth(int lineWidth);
    void setInnerGridLineWidth(int lineWidth);
    void setOuterGridColor(QColor color);
    void setInnerGridColor(QColor color);
    void setBackgroundColor(QColor color);

    int selectedRow() const;
    int selectedCol() const;

    void deselectCell();
    void selectCurrentCell();
    void toggleCurrentCellSelection();

    void clearAllCells();
    void addNumberCell(NumberCell numberCell);

private:
    int _boardLength = 0;
    int _regionLength = 0;
    int _cellLength = 0;
    int _outerGridLineWidth = 5;
    int _innerGridLineWidth = 3;
    QColor _innerGridColor = QColor(147, 112, 219);
    QColor _outerGridColor = QColor(123, 104, 238);
    QColor _backgroundColor = QColor(216, 236, 222);
    NumberCellList _numberCells;
    int _currentRow = -1;
    int _currentCol = -1;
    int _selectedRow = -1;
    int _selectedCol = -1;

    void _drawGrids();
    void _drawNumbers();
    void _drawNumbersInCell(const NumberCell &numberCell);

signals:
    void cellSelected(int row, int col);
    //void cellDeselected(int row, int col);
};

#endif // SUDOKUVIEW_H
