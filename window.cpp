#include "window.h"

#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

Window::Window() {
    setWindowTitle(tr("Window title!"));
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

    float widPerc = width() / 100;
    float heiPerc = height() / 100;
    for (Widget wid : wids) {
        wid.wid->setGeometry(
            wid.position.x()*widPerc, wid.position.y()*heiPerc,
            wid.size.width()*widPerc, wid.size.height()*heiPerc
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
    QPushButton *btn = new QPushButton("<-", this);
    connect(btn, &QPushButton::released, this, &Window::gameMenu);
    btn->show();
    wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8)});

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
