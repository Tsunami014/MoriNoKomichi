#include "widgets/svgbtnwidget.h"
#include "sections.h"
#include "../window.h"

#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QApplication>
#include <QTextCursor>

void newOverlay(Window* wind) {
    std::vector<QWidget*>* rmWids = new std::vector<QWidget*>;     //< The list of widgets to remove when going back to the prev page (all of the ones to be created)
    auto backFun = [wind, rmWids](){removeOverlay(wind, rmWids);}; //< Define a func for 'going back to the prev page' which is used in multiple spots

    // Make the overlay
    OverlayWid *overlay = new OverlayWid(backFun, wind);
    overlay->show();

    // Make the back button
    svgBtnWidget *btn = new svgBtnWidget(":/assets/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, backFun);
    btn->show();

    // Add all the widgets to both the lists
    rmWids->push_back(overlay);
    rmWids->push_back(btn);
    wind->wids.push_back(Widget{overlay, QPoint(-1, -1), QSize(102, 102)});
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});

    wind->resizeElms(); // Update all the positionings!
}
