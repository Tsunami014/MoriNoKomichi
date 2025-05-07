#ifndef WINDOW_H
#define WINDOW_H

#include "graphics.h"
#include "drawWidget.h"

#include <vector>
#include <QWidget>
#include <QTimer>
#include <QGridLayout>
#include <QResizeEvent>

struct Widget {
    QWidget* wid;
    QPoint position;
    QSize size;
};

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
    void resizeElms();

private:
    Graphic graphics;
    DrawWidget *mainWid = new DrawWidget(&graphics, this);
    std::vector<Widget> wids;
    QTimer *timer = new QTimer(this);
};

#endif
