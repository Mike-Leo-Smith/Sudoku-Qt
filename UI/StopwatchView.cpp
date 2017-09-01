#include "StopwatchView.h"

StopwatchView::StopwatchView(QWidget *parent)
    : QLCDNumber(parent), _refreshTimer(new QTimer(this)), _accumulatedMilliseconds(0), _isStopped(0)
{
    _refreshTimer->setInterval(100);
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
    _isStopped = true;
    _accumulatedMilliseconds += _elapsedTimeCounter.elapsed();
    _refreshDisplay();
}

void StopwatchView::_refreshDisplay()
{
    auto elapsedTime = _isStopped ? _accumulatedMilliseconds : _accumulatedMilliseconds + _elapsedTimeCounter.elapsed();
    auto roundedTime = int(elapsedTime / 100) * 0.1;
    display(roundedTime);
}
