#include "widgets/svgbtnwidget.h"
#include "widgets/taskwidget.h"
#include "sections.h"
#include "../window.h"

#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QApplication>
#include <QTextCursor>
#include <QLineEdit>
#include <QPushButton>

/*! \brief A list of all the names of the colours for each section */
static QString colourNames[4] = {
    "red", "green", "blue", "yellow"
};

/*!
    \brief The line edit for adding a new task

    It's made static for the file so it can be referenced in the button click script
*/
static QLineEdit *le = nullptr;

/*!
    \brief The button onclick func - add the task to the right section (idx), update everything and go back
*/
void btnClick(uint8_t idx, Window* wind) {
    TaskWidget* newtw = MakeTaskWidget(le->text(), wind, {});
    addItem(newtw, idx, wind);
    updateTaskPoss(wind);
    backFun(wind);
}

void newOverlay(Window* wind) {
    // Make the overlay
    OverlayWid *overlay = new OverlayWid(wind);
    overlay->show();

    // Make the back button
    SvgBtnWidget *btn = new SvgBtnWidget(":/assets/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, [wind](){backFun(wind);});
    btn->show();

    // Add the text input
    le = new QLineEdit(wind);
    le->setPlaceholderText("New task name");
    le->show();
    le->setFocus();

    // Add the 4 buttons!
    QPushButton* btns[4];
    for (uint8_t i = 0; i < 4; i++) {
        QString txt = QString("Add to the %1 section").arg(colourNames[i]);
        btns[i] = new QPushButton(txt, wind);
        QObject::connect(btns[i], &QPushButton::released, [i, wind](){ btnClick(i, wind); });
        btns[i]->show();
    }

    // Add all the widgets to both the lists
    rmWids->push_back(overlay);
    rmWids->push_back(btn);
    rmWids->push_back(le);
    for (auto btn : btns) {
        rmWids->push_back(btn);
    }
    wind->wids.push_back(Widget{overlay, QPoint(0, 0), QSize(100, 100)});
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});
    wind->wids.push_back(Widget{le, QPoint(50, 50), QSize(30, 6)});
    // The 4 buttons need to be in different spots
    wind->wids.push_back(Widget{btns[0], QPoint(20, 20), QSize(20, 10)});
    wind->wids.push_back(Widget{btns[1], QPoint(80, 20), QSize(20, 10)});
    wind->wids.push_back(Widget{btns[2], QPoint(20, 80), QSize(20, 10)});
    wind->wids.push_back(Widget{btns[3], QPoint(80, 80), QSize(20, 10)});

    wind->resizeElms(); // Update all the positionings!
}
