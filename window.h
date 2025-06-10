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
    
    Positioned using my positioning; percent of screen.

    Coordinates can be floats or ints.

    Can have a size aspect ratio (see `SizeAspect`)
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

    /*!
        \brief The widgets that are displayed

        This list is iterated over to position all the widgets correctly in the `resizeElms` func.
    */
    std::vector<Widget> wids;
    /*! \brief Resize all the elements according to the screen size */
    void resizeElms();

    /*!
        \brief The 4 sections that tasks are added to

        Stored in the Window class as it is used all over the place
    */
    std::array<std::vector<TaskWidget*>, 4> sections;

protected:
    /*! \brief Delete all widgets */
    void reset();

    /*! \brief Just a wrapper for the `resizeElms` func */
    void resizeEvent(QResizeEvent *event);
};

#endif
