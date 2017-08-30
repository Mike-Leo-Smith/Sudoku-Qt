#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include "Window.h"
#include "ui_Window.h"
#include "SudokuController.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    _sudokuController = new SudokuController(ui->sudokuView, this);
    connect(this, SIGNAL(generateRandomSudoku(int)), _sudokuController, SLOT(onGenerateRandomSudoku(int)));
    connect(ui->resetButton, SIGNAL(clicked(bool)), this, SLOT(onResetButtonClicked()));
    connect(ui->difficultyChoices, SIGNAL(currentIndexChanged(int)), this, SLOT(onDifficultySet(int)));
    emit generateRandomSudoku(clueCountForDifficulty(ui->difficultyChoices->currentIndex()));
}

Window::~Window()
{
    delete ui;
}

int Window::clueCountForDifficulty(int difficultyChoiceIndex) const
{
    return 71 - 7 * difficultyChoiceIndex;
}

void Window::onResetButtonClicked()
{
    emit generateRandomSudoku(clueCountForDifficulty(ui->difficultyChoices->currentIndex()));
}

void Window::onDifficultySet(int newDifficulty)
{
    if (newDifficulty == _currentDifficulty) {
        return;
    }

    auto message = "Do you want to start a new game with this new difficulty?";
    auto answer = QMessageBox::information(this, "Message", message, QMessageBox::Ok | QMessageBox::Cancel);
    if (answer == QMessageBox::Ok) {
        _currentDifficulty = newDifficulty;
        emit generateRandomSudoku(clueCountForDifficulty(newDifficulty));
    } else {
        ui->difficultyChoices->setCurrentIndex(_currentDifficulty);
    }
}
