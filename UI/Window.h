#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "GLWidget.h"

namespace Ui {
    class Widget;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
