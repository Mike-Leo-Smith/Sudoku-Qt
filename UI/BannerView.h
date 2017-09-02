#ifndef BANNERVIEW_H
#define BANNERVIEW_H

#include <QWidget>
#include <QString>
#include <QSize>

class BannerView : public QWidget
{
    Q_OBJECT

private:
    QString _text;
    QColor _color;

public:
    explicit BannerView(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void display(QString text, QColor color, QSize size);
};

#endif // BANNERVIEW_H
