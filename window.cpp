#include "drawWidget.h"
#include "window.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>

Window::Window()
{
    setWindowTitle(tr("Window title!"));

    DrawWidget *wid = new DrawWidget(&graphics, this);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(wid, 0, 1);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, wid, &DrawWidget::animate);
    timer->start(50);
}
