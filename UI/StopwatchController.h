#ifndef STOPWATCHVIEW_H
#define STOPWATCHVIEW_H

#include <QLCDNumber>
#include <QTimer>
#include <QTime>

class StopwatchController : public QLCDNumber
{
    Q_OBJECT

private:
    QTime _elapsedTimeCounter;
    QTimer *_refreshTimer;
    int _accumulatedMilliseconds;
    bool _isStopped;

public:
    explicit StopwatchController(QWidget *parent = nullptr);
    ~StopwatchController() override;

signals:

private:
    void _refreshDisplay();

public slots:
    void reset();
    void restart();
    void stop();
    void start();
};

#endif // STOPWATCHVIEW_H
