#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <QWidget>
#include <QTimer>
#include <QGridLayout>
#include <QResizeEvent>

enum SizeAspect {
    REGULAR = 0,
    HEIGHT = 1,
    WIDTH = 2
};

struct Widget {
    QWidget* wid;
    QPointF position;
    QSizeF size;
    SizeAspect sizeRatio = REGULAR;
};

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

    std::vector<Widget> wids;
    void tasksMenu();
    void resizeElms();

protected:
    void reset();
    void resizeEvent(QResizeEvent *event);
};

#endif
