#ifndef KEYBOARDVIEW_H
#define KEYBOARDVIEW_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QHash>

class KeyboardView : public QWidget
{
    Q_OBJECT

private:
    QHash<int, QPushButton *> _buttons;

public:
    enum class FunctionalKeyID {
        clear = 0,
        mark = 10,
        hint = 11
    };

    explicit KeyboardView(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void shouldToggleNumberInCell(int val);
    void shouldClearNumbersInCell();
    void shouldToggleCellMark();
    void shouldGetHintsForCell();

public slots:
    void setSelectedButtons(QVector<int> selectedKeyIDs);
    void setDisabled(bool isDisabled);
};

#endif // KEYBOARDVIEW_H
