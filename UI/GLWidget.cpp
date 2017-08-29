#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void GLWidget::initializeGL()
{
    glClearColor(0.3, 0.4, 0.5, 1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST | GL_MULTISAMPLE);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    {
        glColor3f(0.6, 0.5, 0.7);
        glVertex2d(-0.5, -0.5);
        glVertex2d(0.5, -0.5);
        glVertex2d(0.5, 0.5);
        glVertex2d(-0.5, 0.5);
    }
    glEnd();
}
