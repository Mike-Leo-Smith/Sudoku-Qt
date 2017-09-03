#include "StopwatchView.h"

StopwatchView::StopwatchView(QWidget *parent)
    : QLCDNumber(parent), _refreshTimer(new QTimer(this)), _accumulatedMilliseconds(0), _isStopped(0)
{
    _refreshTimer->setInterval(20);
    connect(_refreshTimer, &QTimer::timeout, this, &StopwatchView::_refreshDisplay);
}

StopwatchView::~StopwatchView()
{
    delete _refreshTimer;
}

void StopwatchView::reset()
{
    _refreshTimer->stop();
    _isStopped = true;
    _accumulatedMilliseconds = 0;
    _refreshDisplay();
}

void StopwatchView::restart()
{
    _isStopped = 0;
    _accumulatedMilliseconds = 0;
    _elapsedTimeCounter.restart();
    _refreshTimer->start();
    _refreshDisplay();
}

void StopwatchView::start()
{
    _refreshTimer->start();
    _isStopped = false;
    _elapsedTimeCounter.restart();
    _refreshDisplay();
}

void StopwatchView::stop()
{
    _refreshTimer->stop();
    if (!_isStopped) {
        _isStopped = true;
        _accumulatedMilliseconds += _elapsedTimeCounter.elapsed();
    }
    _refreshDisplay();
}

void StopwatchView::_refreshDisplay()
{
    auto elapsedTime = _isStopped ? _accumulatedMilliseconds : _accumulatedMilliseconds + _elapsedTimeCounter.elapsed();
    int milliseconds = elapsedTime % 1000;
    int tensOfMilliseconds = milliseconds / 10;
    int seconds = elapsedTime / 1000 % 60;
    int minutes = elapsedTime / 1000 / 60;
    auto timeForDisplay = QString("%1:%2:%3")
                          .arg(minutes, 2, 10, QLatin1Char('0'))
                          .arg(seconds, 2, 10, QLatin1Char('0'))
                          .arg(tensOfMilliseconds, 2, 10, QLatin1Char('0'));
    display(timeForDisplay);
}
