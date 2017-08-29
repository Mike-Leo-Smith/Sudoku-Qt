#include "Window.h"
#include "ui_widget.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Window::~Window()
{
    delete ui;
}
