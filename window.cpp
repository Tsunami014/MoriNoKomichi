#include "window.h"

#include "sections/sections.h"

#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

Window::Window() {
    setWindowTitle(tr("森の小道 : Forest Path"));
    resize(900, 600);
    showMaximized();

    connect(timer, &QTimer::timeout, this, &Window::tick);

    mainWid->show();

    tasksMenu();
}

void Window::resizeEvent(QResizeEvent *event) {
    resizeElms();
}
void Window::resizeElms() {
    mainWid->setGeometry(0, 0, width(), height());

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

void Window::reset(bool useWid) {
    while (!wids.empty()) {
        Widget wid = wids.back();
        delete wid.wid;
        wids.pop_back();
    }
    mainWid->setVisible(useWid);
    if (useWid) {
        timer->start(50);
    } else {
        timer->stop();
    }
}

void Window::tasksMenu() {
    reset(false);
    taskView(this);
    resizeElms();
}

void Window::gameMenu() {
    reset(true);
    QPushButton *btn = new QPushButton("tasks", this);
    connect(btn, &QPushButton::released, this, &Window::tasksMenu);
    btn->show();
    wids.push_back(Widget{btn, QPoint(25, 25), QSize(20, 8)});

    resizeElms();
}


void Window::tick() {
    int dt = qobject_cast<QTimer*>(sender())->interval();
    mainWid->animate(dt);
}
