#include <QPushButton>
#include <QString>
#include <QLayout>
#include "KeyboardView.h"

KeyboardView::KeyboardView(QWidget *parent) : QWidget(parent)
{
    auto outerLayout = new QVBoxLayout(this);
    for (int row = 0; row < 3; row++) {
        auto rowLayout = new QHBoxLayout;
        for (int col = 0; col < 3; col++) {
            auto number = row * 3 + col + 1;
            auto button = new QPushButton(QString::number(number), this);
            button->setCheckable(true);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            rowLayout->addWidget(button);
            _buttons.insert(number, button);
            connect(button, &QPushButton::clicked, this, [this, number]() {
                emit buttonClicked(number);
            });
        }
        outerLayout->addLayout(rowLayout);
    }
}

void KeyboardView::setSelectedButtons(QVector<int> selectedNumbers)
{
    setEnabled(true);
    for (auto button : _buttons) {
        button->setChecked(false);
    }

    for (auto number : selectedNumbers) {
        if (_buttons.contains(number)) {
            _buttons[number]->setChecked(true);
        }
    }
}

void KeyboardView::setDisabled(bool isDisabled)
{
    if (isDisabled) {
        for (auto button : _buttons) {
            button->setChecked(false);
        }
    }
    QWidget::setDisabled(isDisabled);
}
