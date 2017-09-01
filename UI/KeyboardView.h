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
    explicit KeyboardView(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void buttonClicked(int val);

public slots:
    void setSelectedButtons(QVector<int> selectedNumbers);
    void setDisabled(bool isDisabled);
};

#endif // KEYBOARDVIEW_H
