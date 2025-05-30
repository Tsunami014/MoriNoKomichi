#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>

class Graphic;

class DrawWidget : public QWidget
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
