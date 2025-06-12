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

    // Display the main tasks menu
    mainView();
}

void Window::mainView() {
    // Delete all widgets
    while (!wids.empty()) {
        Widget wid = wids.back();
        delete wid.wid;
        wids.pop_back();
    }

    // Get the tasks
    sections = getSections(this);

    taskView(this); // Launch tasks view
    resizeElms();   // Update all the tasks to fit
}

void Window::resizeEvent(QResizeEvent *event) { resizeElms(); }

void Window::resizeElms() {
    // Pre-calculate the width and height multipliers
    float widPerc = width() / 100.0f;
    float heiPerc = height() / 100.0f;

    for (Widget wid : wids) {
        // Get new width and height
        int width = wid.size.width()*widPerc;
        int height = wid.size.height()*heiPerc;
        // Apply aspect ratio
        if (wid.sizeRatio == WIDTH) {
            height = width;
        } else if (wid.sizeRatio == HEIGHT) {
            width = height;
        }
        // Set geometry (also find new x and y positions)
        wid.wid->setGeometry(
            wid.position.x()*(widPerc - (width/100.0f)), wid.position.y()*(heiPerc - (height/100.0f)),
            width, height
        );
    }
}
