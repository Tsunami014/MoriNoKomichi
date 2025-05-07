#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

class Graphic;

class DrawWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
DrawWidget(Graphic *graphics, QWidget *parent);

public slots:
    void animate(int dt);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Graphic *graphics;
    int elapsed;
};

#endif
