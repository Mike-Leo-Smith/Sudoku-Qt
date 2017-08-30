#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include "SudokuView.h"

SudokuView::SudokuView(QWidget *parent) : QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

void SudokuView::setOuterGridLineWidth(int lineWidth)
{
    _outerGridLineWidth = lineWidth;
    update();
}

void SudokuView::setInnerGridLineWidth(int lineWidth)
{
    _innerGridLineWidth = lineWidth;
    update();
}

void SudokuView::setOuterGridColor(QColor color)
{
    _outerGridColor = color;
    update();
}

void SudokuView::setInnerGridColor(QColor color)
{
    _innerGridColor = color;
    update();
}

void SudokuView::setBackgroundColor(QColor color)
{
    _backgroundColor = color;
    update();
}

void SudokuView::clearAllCells()
{
    _numberCells.clear();
    update();
}

void SudokuView::addNumberCell(NumberCell numberCell)
{
    _numberCells.push_back(numberCell);
}

void SudokuView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    _drawGrids();
    _drawNumbers();
}

void SudokuView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    if (width() == _boardLength + _outerGridLineWidth && height() == _boardLength + _outerGridLineWidth) {
        return;
    }

    auto roundedWidth = (width() - _outerGridLineWidth) / 18 * 18;
    auto roundedHeight = (height() - _outerGridLineWidth) / 18 * 18;

    _boardLength = std::min(roundedWidth, roundedHeight);
    _regionLength = _boardLength / 3;
    _cellLength = _boardLength / 9;

    resize(_boardLength + _outerGridLineWidth, _boardLength + _outerGridLineWidth);

    update();
}

void SudokuView::mouseMoveEvent(QMouseEvent *event)
{
    _rowUnderMouse = (event->y() - _outerGridLineWidth * 0.5) / _cellLength;
    _colUnderMouse = (event->x() - _outerGridLineWidth * 0.5) / _cellLength;
    update();
}

void SudokuView::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    emit cellSelected(_rowUnderMouse, _colUnderMouse);
}

void SudokuView::_displayNumbersInCell(const NumberCell &numberCell)
{
    QString numberString = "";
    int counter = 0;
    for (auto number : numberCell.numbers) {
        while (++counter < number) {
            numberString.append(" ");
        }
        numberString.append(QString::number(number));
    }

    qreal numberSize = 0;

    if (numberString.length() > 1) {
        numberSize = _cellLength / 3;
    } else {
        numberSize = _cellLength;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);
    if (_rowUnderMouse == numberCell.row && _colUnderMouse == numberCell.col) {
        painter.setPen(numberCell.color.lighter());
    } else {
        painter.setPen(numberCell.color);
    }
    painter.translate(_outerGridLineWidth * 0.5 + numberCell.col * _cellLength, _outerGridLineWidth * 0.5 + numberCell.row * _cellLength);

    QFont font;
    font.setPixelSize(numberSize);
    font.setLetterSpacing(QFont::AbsoluteSpacing, numberSize * 0.1);
    painter.setFont(font);
    QTextOption textOption(Qt::AlignCenter);
    textOption.setWrapMode(QTextOption::WordWrap);
    painter.drawText(QRect(0, 0, _cellLength, _cellLength), numberString, textOption);
}

void SudokuView::_drawGrids()
{
    QPainter painter(this);
    painter.translate(_outerGridLineWidth * 0.5, _outerGridLineWidth * 0.5);

    // Draw background.
    painter.setBrush(_backgroundColor);
    painter.drawRect(0, 0, _boardLength, _boardLength);
    painter.setBrush(Qt::NoBrush);

    painter.setPen(QPen(_innerGridColor, _innerGridLineWidth));

    // Vertical inner grids.
    for (int x = _cellLength; x < _boardLength; x += _cellLength) {
        painter.drawLine(x, 0, x, _boardLength);
    }

    // Horizontal inner grids.
    for (int y = _cellLength; y < _boardLength; y += _cellLength) {
        painter.drawLine(0, y, _boardLength, y);
    }

    painter.setPen(QPen(_outerGridColor, _outerGridLineWidth));

    // Vertical outer grids.
    for (int x = 0; x <= _boardLength; x += _regionLength) {
        painter.drawLine(x, 0, x, _boardLength);
    }

    // Horizontal outer grids.
    for (int y = 0; y <= _boardLength; y += _regionLength) {
        painter.drawLine(0, y, _boardLength, y);
    }

    // Hightlight the cell under the mouse.
    if (_rowUnderMouse >= 0 && _rowUnderMouse < 9 && _colUnderMouse >= 0 && _colUnderMouse < 9) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.translate(_colUnderMouse * _cellLength + _innerGridLineWidth * 0.5, _rowUnderMouse * _cellLength + _innerGridLineWidth * 0.5);
        painter.drawRect(0, 0, _cellLength - _innerGridLineWidth, _cellLength - _innerGridLineWidth);
        painter.setBrush(_backgroundColor.lighter());
        painter.drawRect(0, 0, _cellLength - _innerGridLineWidth * 2, _cellLength - _innerGridLineWidth * 2);
    }
}

void SudokuView::_drawNumbers()
{
    for (const auto &numberCell : _numberCells) {
        if (!numberCell.numbers.empty()) {
            _displayNumbersInCell(numberCell);
        }
    }
}
