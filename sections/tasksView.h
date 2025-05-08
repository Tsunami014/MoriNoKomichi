#ifndef TASKSVIEW_H
#define TASKSVIEW_H

#include "../window.h"

#include <QPushButton>

void taskView(Window* wind) {
    QPushButton *btn = new QPushButton("<-", wind);
    wind->connect(btn, &QPushButton::released, wind, &Window::gameMenu);
    btn->show();
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(5, 5), HEIGHT});
}

#endif // TASKSVIEW_H
