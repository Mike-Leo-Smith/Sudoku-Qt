#include <QPushButton>
#include <QString>
#include <QLayout>
#include <QDebug>
#include <QPixmap>
#include <QKeyEvent>
#include "KeyboardView.h"

KeyboardView::KeyboardView(QWidget *parent) : QWidget(parent)
{
    auto buttonWithText = [this](QString text = "") {
        auto button = new QPushButton(text, this);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setCheckable(true);
        return button;
    };

    auto registerButton = [this](QPushButton *button, int id, QLayout *layout) {
        layout->addWidget(button);
        _buttons.insert(id, button);
    };

    auto outerLayout = new QVBoxLayout(this);
    for (int row = 0; row < 3; row++) {
        auto rowLayout = new QHBoxLayout;
        for (int col = 0; col < 3; col++) {
            int number = row * 3 + col + 1;
            auto button = buttonWithText(QString::number(number));
            registerButton(button, number, rowLayout);
            connect(button, &QPushButton::clicked, this, [this, number] {
                emit shouldToggleNumberInCell(number);
            });
        }
        outerLayout->addLayout(rowLayout);
    }

    auto functionalKeyLayout = new QHBoxLayout;
    outerLayout->addLayout(functionalKeyLayout);

    auto clearButton = buttonWithText();
    clearButton->setIcon(QIcon(":/Images/Clear.png"));
    clearButton->setIconSize(clearButton->size());
    registerButton(clearButton, static_cast<int>(FunctionalKeyID::clear), functionalKeyLayout);
    connect(clearButton, &QPushButton::clicked, this, &KeyboardView::shouldClearNumbersInCell);

    auto markButton = buttonWithText();
    markButton->setIcon(QIcon(":/Images/Flag.png"));
    markButton->setIconSize(markButton->size());
    registerButton(markButton, static_cast<int>(FunctionalKeyID::mark), functionalKeyLayout);
    connect(markButton, &QPushButton::clicked, this, &KeyboardView::shouldToggleCellMark);

    auto hintButton = buttonWithText();
    hintButton->setIcon(QIcon(":/Images/Hint.png"));
    hintButton->setIconSize(hintButton->size());
    registerButton(hintButton, static_cast<int>(FunctionalKeyID::hint), functionalKeyLayout);
    connect(hintButton, &QPushButton::clicked, this, &KeyboardView::shouldGetHintsForCell);
}

void KeyboardView::setSelectedButtons(QVector<int> selectedKeyIDs)
{
    for (auto button : _buttons) {
        button->setChecked(false);
    }
    qDebug() << selectedKeyIDs;
    for (auto number : selectedKeyIDs) {
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

void KeyboardView::keyPressEvent(QKeyEvent *event)
{
    if (isEnabled()) {
        auto key = event->key();
        if (key >= Qt::Key_1 && key <= Qt::Key_9) {
            _buttons[key - Qt::Key_0]->click();
        }
    }
}
