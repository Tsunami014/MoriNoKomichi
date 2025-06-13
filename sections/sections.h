#ifndef SECTIONS_H
#define SECTIONS_H

#include "widgets/taskwidget.h"
#include "widgets/graphicsviewcanvas.h"
#include "../window.h"

// Located in sections.cpp:
/*!
    \brief A widget which is just a slightly transparent box, meant to be an overlay. On click it runs a function.
*/
class OverlayWid : public QWidget {
public:
    OverlayWid(Window* wind);

protected:
    /*! \brief Runs the backfunc */
    void mouseReleaseEvent(QMouseEvent *event) override;
    /*! \brief Draws the slightly transparent black rectangle covering the screen */
    void paintEvent(QPaintEvent *event) override;

private:
    Window* wind;
};

// Various variables used in multiple places (defined in sections.cpp)
/*!
    \brief The list of widgets to remove when going back to the prev page (all of the ones to be created)
*/
extern std::vector<QWidget*>* rmWids;
/*!
    \brief A func for 'going back to the prev page' which is used in multiple places
*/
extern std::function<void(Window*)> backFun;

/*! \brief The main tasks view */
void taskView(Window* wind);                       // Located in tasksview.cpp
/*! \brief A large task overlay with the help task. Not editable */
void helpOverlay(Window* wind);                    // Located in helpoverlay.cpp
/*! \brief The overlay for making a new task */
void newOverlay(Window* wind);                     // Located in newoverlay.cpp
/*! \brief The overlay for inspecting a task and editing things (e.g. name, subtasks, etc.) */
void taskOverlay(Window* wind, TaskWidget* task);  // Located in taskoverlay.cpp

// Located in taskoverlay.cpp
/*! \brief Update the group of a task to a different one */
void updateTaskGroup(TaskWidget* tsk, uint8_t newGroupNum, Window* wind);
/*! \brief Update the positions of all the tasks, and then refresh it */
void updateTaskPoss(Window* wind);
/*! \brief Add a taskwidget to a specific group */
void addItem(TaskWidget* it, uint8_t groupNum, Window* wind);
/*! \brief Remove a taskwidget */
void removeItem(TaskWidget* tsk, Window* wind);
/*! \brief Sort all the sections of taskwidgets based off their priority */
void sortTasks(Window* wind);


/*!
We create our own graphics view for multiple reasons:
    1. To have a transparent background; where on click, it goes back to the previous screen
    2. Because the space key has issues
    3. So resizing the window updates the big task widget display
    4. So resizing of the big task widget updates the display
    5. To remove horizontal scrolling and zoom

This class is made for use with a big task widget
*/
class NewGraphicsView : public GraphicsViewCanvas {
public:
    NewGraphicsView(QGraphicsScene* scene, BigTaskWidget* wid, Window* window);

    /*! \brief Updates the scene rect based off of the childrens' bounding boxes */
    void updateRect();

protected:
    /*! \brief If click on bg, run the click func (goes back to prev page) */
    void mousePressEvent(QMouseEvent* event) override;
    bool event(QEvent *ev) override;
    /*! \brief Remove x scrolling */
    void offsetPos(int x, int y) override;
    /*! \brief Update bg sizing on resize */
    void resizeEvent(QResizeEvent *event) override;
    /*! \brief Remove zoom */
    void zoom(int delta);
private:
    /*! \brief The parent Window */
    Window* wind;
    /*! \brief The big task widget to feed the spaces into */
    BigTaskWidget* bigW = nullptr;
};

#endif // SECTIONS_H
