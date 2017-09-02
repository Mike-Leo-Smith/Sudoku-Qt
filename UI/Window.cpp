#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>
#include <QPixmap>
#include <QSizePolicy>
#include <QMediaPlayer>
#include "Window.h"
#include "ui_Window.h"
#include "SudokuController.h"
#include "BannerView.h"

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    _sudokuController = new SudokuController(ui->sudokuView, this);
    _bannerView = new BannerView(this);

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
    connect(this, &Window::gameStateChanged, _sudokuController, &SudokuController::setGameState);
    connect(_sudokuController, &SudokuController::sudokuSolved, this, &Window::_finishGame);
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
    if (_askForConfirmation("Are you sure to exit?")) {
        event->accept();
    } else {
        event->ignore();
    }
}

int Window::_clueCountForSelectedDifficulty() const
{
    return 71 - 7 * ui->difficultyChoices->currentIndex();
}

bool Window::_askForConfirmation(QString message)
{
    if (_gameState == GameState::inGame) {
        _pauseGame();
    }
    auto answer = QMessageBox::warning(this, "Warning", message, QMessageBox::Ok, QMessageBox::Cancel);
    return (answer == QMessageBox::Ok);
}

void Window::_setGameState(GameState state)
{
    if (_gameState == state) {
        return;
    }
    _gameState = state;
    emit gameStateChanged(state);
}

void Window::_finishGame()
{
    _pauseGame();
    _setGameState(GameState::ended);
    _bannerView->display("Solved!", QColor(250, 70, 70), ui->sudokuView->size());
}

void Window::_initializeGame()
{
    _sudokuController->generateBlankSudoku();
    ui->resetButton->setDisabled(true);
    ui->pauseButton->setText("Pause");
    ui->pauseButton->setDisabled(true);
    ui->solveButton->setDisabled(true);
    ui->stopwatchDisplay->reset();
    _setGameState(GameState::initial);
    _bannerView->display("Sudoku", QColor(60, 133, 250), ui->sudokuView->size());
}

void Window::_restartGame()
{
    if (_askForConfirmation("Are you sure to restart the current game?")) {
        _sudokuController->resetCurrentSudoku();
        ui->resetButton->setEnabled(true);
        ui->pauseButton->setText("Pause");
        ui->pauseButton->setEnabled(true);
        ui->solveButton->setEnabled(true);
        ui->stopwatchDisplay->restart();
        _setGameState(GameState::inGame);
        _bannerView->hide();
    }
}

void Window::_startGame()
{
    if (_gameState == GameState::initial || _gameState == GameState::ended || _askForConfirmation("Are you sure to start a new game?")) {
        _sudokuController->generateRandomSudoku(_clueCountForSelectedDifficulty());
        ui->resetButton->setEnabled(true);
        ui->pauseButton->setText("Pause");
        ui->pauseButton->setEnabled(true);
        ui->solveButton->setEnabled(true);
        ui->stopwatchDisplay->restart();
        _setGameState(GameState::inGame);
        _bannerView->hide();
    }
}

void Window::_pauseGame()
{
    ui->pauseButton->setText("Resume");
    ui->stopwatchDisplay->stop();
    _setGameState(GameState::paused);
    _bannerView->display("Paused", QColor(250, 200, 140), ui->sudokuView->size());
}

void Window::_resumeGame()
{
    ui->pauseButton->setText("Pause");
    ui->stopwatchDisplay->start();
    _setGameState(GameState::inGame);
    _bannerView->hide();
}

void Window::_solveGame()
{
    if (_gameState != GameState::ended && _askForConfirmation("Are you sure to have a look at the answer?")) {
        _sudokuController->solveCurrentSudoku();
        ui->pauseButton->setText("Pause");
        ui->pauseButton->setDisabled(true);
        ui->resetButton->setEnabled(true);
        ui->solveButton->setDisabled(true);
        ui->stopwatchDisplay->stop();
        _setGameState(GameState::ended);
        _bannerView->hide();
    }
}

void Window::_changeDifficulty(int newDifficulty)
{
    if (newDifficulty == _currentDifficulty) {
        return;
    }

    if (_gameState == GameState::initial || _gameState == GameState::ended || _askForConfirmation("Are you sure to switch to the selected difficulty?")) {
        _currentDifficulty = newDifficulty;
        _initializeGame();
    } else {
        ui->difficultyChoices->setCurrentIndex(_currentDifficulty);
    }
}
