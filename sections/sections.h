#ifndef SECTIONS_H
#define SECTIONS_H

#include "widgets/taskwidget.h"
#include "../window.h"

// Located in sections.cpp:
/*! \brief A widget which is just a slightly transparent box, meant to be an overlay. On click it runs a function. */
class OverlayWid : public QWidget {
public:
    OverlayWid(std::function<void()> clickFun, QWidget* parent = nullptr);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    std::function<void()> clickFunc;
};
void removeOverlay(Window* wind, std::vector<QWidget*>* wids);

void taskView(Window* wind);                       // Located in tasksview.cpp
void newOverlay(Window* wind);                     // Located in newoverlay.cpp
void taskOverlay(Window* wind, TaskWidget* task);  // Located in taskoverlay.cpp
/*! \brief Update the positions of all the tasks */
void updateTaskPoss(Window* wind);                 // Located in taskoverlay.cpp

#endif // SECTIONS_H
