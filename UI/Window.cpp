#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include "Window.h"
#include "ui_Window.h"
#include "SudokuController.h"

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    _sudokuController = new SudokuController(ui->sudokuView, this);
    _initializeGame();

    connect(ui->newgameButton, &QPushButton::clicked, this, &Window::_startGame);
    connect(ui->difficultyChoices, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged), this, &Window::_changeDifficulty);
    connect(ui->resetButton, &QPushButton::clicked, this, &Window::_restartGame);
    connect(ui->solveButton, &QPushButton::clicked, this, &Window::_solveGame);
    connect(ui->undoButton, &QPushButton::clicked, _sudokuController, &SudokuController::undo);
    connect(ui->redoButton, &QPushButton::clicked, _sudokuController, &SudokuController::redo);
    connect(ui->pauseButton, &QPushButton::clicked, this, [this] {
        ui->pauseButton->text() == "Pause" ? _pauseGame() : _resumeGame();
    });
    connect(_sudokuController, &SudokuController::shouldSetMarkedNumbers, ui->keyboardView, &KeyboardView::setSelectedButtons);
    connect(_sudokuController, &SudokuController::shouldDisableKeyboard, ui->keyboardView, &KeyboardView::setDisabled);
    connect(_sudokuController, &SudokuController::shouldDisableRedo, ui->redoButton, &QPushButton::setDisabled);
    connect(_sudokuController, &SudokuController::shouldDisableUndo, ui->undoButton, &QPushButton::setDisabled);

    connect(ui->keyboardView, &KeyboardView::shouldToggleCellMark, _sudokuController, &SudokuController::toggleSelectedCellMark);
    connect(ui->keyboardView, &KeyboardView::shouldToggleNumberInCell, _sudokuController, &SudokuController::toggleNumberInSelectedCell);
    connect(ui->keyboardView, &KeyboardView::shouldClearNumbersInCell, _sudokuController, &SudokuController::clearNumbersInSelectedCell);
    connect(ui->keyboardView, &KeyboardView::shouldGetHintsForCell, _sudokuController, &SudokuController::getHintsForSelectedCell);
    connect(this, &Window::shouldGameRun, _sudokuController, &SudokuController::setGameRunning);
}

Window::~Window()
{
    delete ui;
}

void Window::keyPressEvent(QKeyEvent *event)
{
    ui->sudokuView->keyPressEvent(event);
    ui->keyboardView->keyPressEvent(event);
    QWidget::keyPressEvent(event);
}

void Window::closeEvent(QCloseEvent *event)
{
    _pauseGame();
    auto answer = QMessageBox::warning(this, "Warning", "Are you sure to exit?", QMessageBox::Ok, QMessageBox::Cancel);
    if (answer == QMessageBox::Ok) {
        event->accept();
    } else {
        event->ignore();
    }
}

int Window::_clueCountForSelectedDifficulty() const
{
    return 71 - 7 * ui->difficultyChoices->currentIndex();
}

void Window::_initializeGame()
{
    _sudokuController->generateBlankSudoku();
    ui->resetButton->setDisabled(true);
    ui->pauseButton->setText("Pause");
    ui->pauseButton->setDisabled(true);
    ui->solveButton->setDisabled(true);
    ui->stopwatchDisplay->reset();
    emit shouldGameRun(false);
}

void Window::_restartGame()
{
    _pauseGame();
    auto answer = QMessageBox::warning(this, "Warning", "Are you sure to restart the current game?", QMessageBox::Ok, QMessageBox::Cancel);

    if (answer == QMessageBox::Ok) {
        _sudokuController->resetCurrentSudoku();
        ui->resetButton->setEnabled(true);
        ui->pauseButton->setText("Pause");
        ui->pauseButton->setEnabled(true);
        ui->solveButton->setEnabled(true);
        ui->stopwatchDisplay->restart();
        emit shouldGameRun(true);
    }
}

void Window::_startGame()
{
    _sudokuController->generateRandomSudoku(_clueCountForSelectedDifficulty());
    ui->resetButton->setEnabled(true);
    ui->pauseButton->setText("Pause");
    ui->pauseButton->setEnabled(true);
    ui->solveButton->setEnabled(true);
    ui->stopwatchDisplay->restart();
    emit shouldGameRun(true);
}

void Window::_pauseGame()
{
    ui->pauseButton->setText("Resume");
    ui->stopwatchDisplay->stop();
    emit shouldGameRun(false);
}

void Window::_resumeGame()
{
    ui->pauseButton->setText("Pause");
    ui->stopwatchDisplay->start();
    emit shouldGameRun(true);
}

void Window::_solveGame()
{
    _sudokuController->solveCurrentSudoku();
    ui->pauseButton->setText("Pause");
    ui->pauseButton->setDisabled(true);
    ui->resetButton->setEnabled(true);
    ui->solveButton->setDisabled(true);
    ui->stopwatchDisplay->stop();
    emit shouldGameRun(false);
}

void Window::_changeDifficulty(int newDifficulty)
{
    if (newDifficulty == _currentDifficulty) {
        return;
    }
    _pauseGame();
    auto message = "Are you sure to start a new game with the selected difficulty?";
    auto answer = QMessageBox::information(this, "Message", message, QMessageBox::Ok | QMessageBox::Cancel);
    if (answer == QMessageBox::Ok) {
        _currentDifficulty = newDifficulty;
        _initializeGame();
    } else {
        ui->difficultyChoices->setCurrentIndex(_currentDifficulty);
    }
}
