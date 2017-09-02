#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>
#include <QKeyEvent>
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

void SudokuView::reset()
{
    _numberCells.clear();
    _highlightedCells.clear();
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
    _drawHighlightedCells();
    _highlightSelectedCell();
    _highlightCurrentCell();
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
    _currentRow = (event->y() - _outerGridLineWidth * 0.5) / _cellLength;
    _currentCol = (event->x() - _outerGridLineWidth * 0.5) / _cellLength;
    update();
}

void SudokuView::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    toggleCurrentCellSelection();
    update();
}

void SudokuView::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key pressed!";

    if (_currentRow < 0 || _currentRow >= 9 || _currentCol < 0 || _currentCol >= 9) {
        _currentRow = 0;
        _currentCol = 0;
    } else {
        switch (event->key()) {
        case Qt::Key_Up:
            if (_currentRow > 0) { _currentRow--; }
            break;
        case Qt::Key_Down:
            if (_currentRow < 8) { _currentRow++; }
            break;
        case Qt::Key_Left:
            if (_currentCol > 0) { _currentCol--; }
            break;
        case Qt::Key_Right:
            if (_currentCol < 8) { _currentCol++; }
            break;
        case Qt::Key_Space:
            toggleCurrentCellSelection();
            break;
        default:
            break;
        }
    }
    update();
}

void SudokuView::_drawNumbersInCell(const NumberCell &numberCell)
{
    qreal numberSize = _cellLength;
    if (numberCell.numbers.size() > 1) {
        numberSize /= 3.0;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing);
    painter.setPen(numberCell.color);
    painter.translate(_outerGridLineWidth * 0.5 + numberCell.col * _cellLength, _outerGridLineWidth * 0.5 + numberCell.row * _cellLength);

    QFont font;
    font.setPixelSize(numberSize);
    painter.setFont(font);

    QTextOption textOption(Qt::AlignCenter);
    textOption.setWrapMode(QTextOption::WordWrap);

    if (numberCell.numbers.size() > 1) {
        for (auto number : numberCell.numbers) {
            auto rowInCell = (number - 1) / 3;
            auto colInCell = (number - 1) % 3;
            auto textOffset = QPoint(colInCell * numberSize, rowInCell * numberSize);
            auto textSize = QSize(numberSize, numberSize);
            painter.drawText(QRect(textOffset, textSize), QString::number(number), textOption);
        }
    } else {
        painter.drawText(QRect(0, 0, _cellLength, _cellLength), QString::number(numberCell.numbers.front()), textOption);
    }
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
}

void SudokuView::_highlightCurrentCell()
{
    const auto currentCellColor = QColor(250, 250, 250, 150);
    _highlightCell({ _currentRow, _currentCol, currentCellColor });
}

void SudokuView::_highlightSelectedCell()
{
    const auto selectedCellColor = QColor(50, 50, 50, 150);
    _highlightCell({ _selectedRow, _selectedCol, selectedCellColor });
}

void SudokuView::_drawNumbers()
{
    for (const auto &numberCell : _numberCells) {
        if (!numberCell.numbers.empty()) {
            _drawNumbersInCell(numberCell);
        }
    }
}

void SudokuView::_highlightCell(const HighlightedCell &highlightedCell)
{
    auto row = highlightedCell.row;
    auto col = highlightedCell.col;
    auto color = highlightedCell.color;
    auto frameLength = _cellLength - _innerGridLineWidth;
    auto backgroundBrush = QBrush(color);
    const auto frameColor = QColor(60, 60, 60, 200);
    auto framePen = QPen(frameColor, _innerGridLineWidth);
    framePen.setCapStyle(Qt::RoundCap);

    QPainter painter(this);
    painter.translate(_outerGridLineWidth * 0.5, _outerGridLineWidth * 0.5);
    painter.translate(col * _cellLength + _innerGridLineWidth * 0.5, row * _cellLength + _innerGridLineWidth * 0.5);

    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        painter.save();
        painter.setBrush(backgroundBrush);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, frameLength, frameLength);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(framePen);
        painter.drawLine(_innerGridLineWidth * 0.5, frameLength, frameLength, frameLength);
        painter.drawLine(frameLength, _innerGridLineWidth * 0.5, frameLength, frameLength);
        painter.restore();
    }
}

void SudokuView::_drawHighlightedCells()
{
    for (const auto &highlightedCell : _highlightedCells) {
        _highlightCell(highlightedCell);
    }
}

void SudokuView::addHighlightedCell(HighlightedCell highlightedCell)
{
    _highlightedCells.push_back(highlightedCell);
}

int SudokuView::selectedRow() const
{
    return _selectedRow;
}

int SudokuView::selectedCol() const
{
    return _selectedCol;
}

void SudokuView::deselectCell()
{
    _selectedRow = -1;
    _selectedCol = -1;
}

void SudokuView::selectCurrentCell()
{
    _selectedRow = _currentRow;
    _selectedCol = _currentCol;
    emit cellSelected(_currentRow, _currentCol);
}

void SudokuView::toggleCurrentCellSelection()
{
    if (_selectedRow == _currentRow && _selectedCol == _currentCol) {
        deselectCell();
    } else {
        selectCurrentCell();
    }
}
