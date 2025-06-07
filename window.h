#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <QWidget>
#include <QTimer>
#include <QGridLayout>
#include <QResizeEvent>

class TaskWidget; // Forward reference

/*!
    \brief Size aspect ratio

    If not REGULAR, sets the other value to this. E.g. if WIDTH, sets height to width.
*/
enum SizeAspect {
    REGULAR = 0,
    HEIGHT = 1,
    WIDTH = 2
};

/*!
    \brief A positioned widget
    
    Positioned using my positioning; percent of screen
*/
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

    std::array<std::vector<TaskWidget*>, 4> sections;

protected:
    void reset();
    void resizeEvent(QResizeEvent *event);
};

#endif
