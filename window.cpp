#include "window.h"

#include "sections/sections.h"

#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

Window::Window() {
    setWindowTitle(tr("森の小道 : Forest Path"));
    resize(900, 600);
    showMaximized();

    tasksMenu();
}

void Window::resizeEvent(QResizeEvent *event) {
    resizeElms();
}
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

void Window::reset() {
    while (!wids.empty()) {
        Widget wid = wids.back();
        delete wid.wid;
        wids.pop_back();
    }
}

void Window::tasksMenu() {
    reset();
    taskView(this);
    resizeElms();
}
