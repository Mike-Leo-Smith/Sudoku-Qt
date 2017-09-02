#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include "BannerView.h"

BannerView::BannerView(QWidget *parent) : QWidget(parent)
{
    auto dropShadowEffect = new QGraphicsDropShadowEffect(this);
    dropShadowEffect->setBlurRadius(30);
    dropShadowEffect->setOffset(10);
    setGraphicsEffect(dropShadowEffect);
}

void BannerView::display(QString text, QColor color, QSize size)
{
    _text = text;
    _color = color;
    resize(size);
    show();
    update();
}

void BannerView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    auto fontSize = width() / _text.length();
    auto bannerHeight = fontSize * 2;

    QPainter painter(this);
    QFont font;
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(120, 170, 150, 70));
    painter.drawRect(0, (height() - bannerHeight) / 2, width(), bannerHeight);
    font.setPixelSize(fontSize);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(_color);
    painter.drawText(rect(), Qt::AlignCenter, _text);
}
