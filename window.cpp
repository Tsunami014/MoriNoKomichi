#include "window.h"

#include <QLabel>
#include <QPushButton>

Window::Window() {
    setWindowTitle(tr("Window title!"));
    showMaximized();

    connect(timer, &QTimer::timeout, this, &Window::tick);

    wid->setGeometry(0, 0, width(), height());
    wid->show();

    tasksMenu();
    setLayout(layout);
}

void Window::resizeEvent(QResizeEvent *event) {
    wid->setGeometry(0, 0, width(), height());
}

void Window::reset(bool useWid) {
    QLayoutItem* item;
    while ( ( item = layout->layout()->takeAt( 0 ) ) != NULL ) {
        delete item->widget();
        delete item;
    }
    wid->setVisible(useWid);
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
    layout->addWidget(btn, 0, 1);
}

void Window::gameMenu() {
    reset(true);
    QPushButton *btn = new QPushButton("tasks", this);
    connect(btn, &QPushButton::released, this, &Window::tasksMenu);
    layout->addWidget(btn, 1, 1);
}


void Window::tick() {
    int dt = qobject_cast<QTimer*>(sender())->interval();
    wid->animate(dt);
}
