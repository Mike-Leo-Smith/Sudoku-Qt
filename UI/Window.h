#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include "GameState.h"
#include "BannerView.h"

namespace Ui {
    class Widget;
}

class SudokuController;

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window() override;

    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::Widget *ui;
    SudokuController *_sudokuController = nullptr;
    BannerView *_bannerView = nullptr;
    int _currentDifficulty;
    int _clueCountForSelectedDifficulty() const;
    GameState _gameState = GameState::initial;
    QMediaPlayer *_soundEffect = nullptr;

private slots:
    void _initializeGame();
    void _finishGame();
    void _startGame();
    void _pauseGame();
    void _restartGame();
    void _resumeGame();
    void _solveGame();
    void _changeDifficulty(int newDifficulty);
    void _setGameState(GameState state);
    bool _askForConfirmation(QString message);

signals:
    void shouldGameRun(bool);
    void gameStateChanged(GameState gameState);
};

#endif // WIDGET_H
