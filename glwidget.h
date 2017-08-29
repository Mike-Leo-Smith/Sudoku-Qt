#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget
{
public:
    explicit GLWidget(QWidget *parent);

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

};

#endif // GLWIDGET_H
