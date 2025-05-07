#ifndef WINDOW_H
#define WINDOW_H

#include "graphics.h"
#include "drawWidget.h"

#include <QWidget>
#include <QTimer>
#include <QGridLayout>
#include <QResizeEvent>

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

    void tick();

protected:
    void reset(bool useWid);

    void tasksMenu();
    void gameMenu();

    void resizeEvent(QResizeEvent *event);

private:
    Graphic graphics;
    QGridLayout *layout = new QGridLayout();
    DrawWidget *wid = new DrawWidget(&graphics, this);
    QTimer *timer = new QTimer(this);
};

#endif
