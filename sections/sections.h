#ifndef SECTIONS_H
#define SECTIONS_H

#include "widgets/taskwidget.h"
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
/*! \brief The overlay for making a new task */
void newOverlay(Window* wind);                     // Located in newoverlay.cpp
/*! \brief The overlay for inspecting a task and editing things (e.g. name, subtasks, etc.) */
void taskOverlay(Window* wind, TaskWidget* task);  // Located in taskoverlay.cpp

/*! \brief Update the positions of all the tasks, and then refresh it */
void updateTaskPoss(Window* wind);                 // Located in taskoverlay.cpp
/*! \brief Add a taskwidget to a specific group */
void addItem(TaskWidget* it, uint8_t groupNum, Window* wind); // Located in taskoverlay.cpp

#endif // SECTIONS_H
