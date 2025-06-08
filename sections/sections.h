#ifndef SECTIONS_H
#define SECTIONS_H

#include "widgets/taskwidget.h"
#include "../window.h"

// Located in sections.cpp:
/*! \brief A widget which is just a slightly transparent box, meant to be an overlay. On click it runs a function. */
class OverlayWid : public QWidget {
public:
    OverlayWid(Window* wind);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Window* wind;
};
void removeOverlay(Window* wind, std::vector<QWidget*>* wids);

// Various variables used in multiple places
extern std::vector<QWidget*>* rmWids;        //< The list of widgets to remove when going back to the prev page (all of the ones to be created)
extern std::function<void(Window*)> backFun; //< Define a func for 'going back to the prev page' which is used in multiple spots


void taskView(Window* wind);                       // Located in tasksview.cpp
void newOverlay(Window* wind);                     // Located in newoverlay.cpp
void taskOverlay(Window* wind, TaskWidget* task);  // Located in taskoverlay.cpp
/*! \brief Update the positions of all the tasks */
void updateTaskPoss(Window* wind);                 // Located in taskoverlay.cpp
/*! \brief Add a taskwidget to a group */
void addItem(TaskWidget* it, uint8_t groupNum, Window* wind); // Located in taskoverlay.cpp

#endif // SECTIONS_H
