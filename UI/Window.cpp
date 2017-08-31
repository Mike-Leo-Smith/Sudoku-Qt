#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include "Window.h"
#include "ui_Window.h"
#include "SudokuController.h"

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    _sudokuController = new SudokuController(ui->sudokuView, this);
    _sudokuController->generateRandomSudoku(_clueCountForSelectedDifficulty());

    connect(ui->resetButton, &QPushButton::clicked, [this] {
        _sudokuController->generateRandomSudoku(_clueCountForSelectedDifficulty());
    });
    connect(ui->difficultyChoices, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged), [this](int newDifficulty){
        _setDifficulty(newDifficulty);
    });
    connect(ui->solveButton, &QPushButton::clicked, _sudokuController, &SudokuController::solveCurrentSudoku);
}

Window::~Window()
{
    delete ui;
}

int Window::_clueCountForSelectedDifficulty() const
{
    return 71 - 7 * ui->difficultyChoices->currentIndex();
}

void Window::_setDifficulty(int newDifficulty)
{
    if (newDifficulty == _currentDifficulty) {
        return;
    }

    auto message = "Do you want to start a new game with the selected difficulty?";
    auto answer = QMessageBox::information(this, "Message", message, QMessageBox::Ok | QMessageBox::Cancel);
    if (answer == QMessageBox::Ok) {
        _currentDifficulty = newDifficulty;
        _sudokuController->generateRandomSudoku(_clueCountForSelectedDifficulty());
    } else {
        ui->difficultyChoices->setCurrentIndex(_currentDifficulty);
    }
}
