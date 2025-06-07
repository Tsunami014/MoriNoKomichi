#include "window.h"
#include "taskloading.h"
#include "sections/sections.h"

#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

Window::Window() {
    // Create the window with a predefined title.
    setWindowTitle(tr("森の小道 : Forest Path"));
    // The resize then maximise ensures that on minimise the window will be the correct size, not some random incorrect value
    resize(900, 600);
    showMaximized();

    // Get the tasks
    sections = getSections(this);

    // Start by displaying the tasksMenu
    tasksMenu();
}

void Window::resizeEvent(QResizeEvent *event) {
    resizeElms();
}

/*!
    \brief Resize all the elements
    
    The units are in percentages of the screen width/height respectively (/100) and can be ints or floats.
*/
void Window::resizeElms() {
    float widPerc = width() / 100.0f;
    float heiPerc = height() / 100.0f;
    for (Widget wid : wids) {
        int width = wid.size.width()*widPerc;
        int height = wid.size.height()*heiPerc;
        if (wid.sizeRatio == WIDTH) {
            height = width;
        } else if (wid.sizeRatio == HEIGHT) {
            width = height;
        }
        wid.wid->setGeometry(
            wid.position.x()*widPerc, wid.position.y()*heiPerc,
            width, height
        );
    }
}

/*!
    \brief Delete all widgets in preparation for creating a screen
*/
void Window::reset() {
    // Delete all widgets
    while (!wids.empty()) {
        Widget wid = wids.back();
        delete wid.wid;
        wids.pop_back();
    }
}

/*!
    \brief Launch the task menu

    This menu has a display of all tasks, click on task to edit and add task.
*/
void Window::tasksMenu() {
    reset();
    taskView(this);
    resizeElms();
}
