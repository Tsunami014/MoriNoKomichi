#include "widgets/taskwidget.h"
#include "widgets/bigtaskwidget.h"
#include "widgets/svgbtnwidget.h"
#include "widgets/inputwidget.h"
#include "sections.h"
#include "../window.h"

#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QApplication>
#include <QTextCursor>

/*! \brief A function to add new subtasks, used by the buttons */
void addNewSubtask(Window* wind, BigTaskWidget* bigW, QString txt) {
    bigW->todos.push_back(new TodoGraphicObject(txt, true, bigW));
    bigW->updateChildren(true, true);
}

void taskOverlay(Window* wind, TaskWidget* task) {
    // Make the overlay
    OverlayWid *overlay = new OverlayWid(wind);
    overlay->show();

    // Make the graphics scene with a generated big task widget
    QGraphicsScene* scene = new QGraphicsScene();
    BigTaskWidget* bigW = task->toBigWidget();
    scene->addItem(bigW);
    NewGraphicsView *view = new NewGraphicsView(scene, bigW, wind);
    view->show();
    view->gotoTopLeft();

    // Make the back btn
    SvgBtnWidget *btn = new SvgBtnWidget(":/assets/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, [wind](){backFun(wind);});
    btn->show();

    // Make the delete btn
    SvgBtnWidget *btn2 = new SvgBtnWidget(":/assets/binBtn.svg", wind);
    // On click delete the task and go back
    wind->connect(btn2, &QPushButton::released, wind, [wind, task](){
        removeItem(task, wind); // Delete task from sections
        backFun(wind); // Remove overlay
    });
    btn2->show();

    // Make the add sub-task input box
    InputWidget* newSubtask = new InputWidget(wind);
    newSubtask->setPlaceholderText("New subtask");
    // Run the new subtask func on enter pressed
    QObject::connect(newSubtask, &QLineEdit::returnPressed, [wind, bigW, newSubtask](){
        addNewSubtask(wind, bigW, newSubtask->text());
        newSubtask->setText("");
    });
    newSubtask->show();

    // Find section number of task
    auto sect = std::make_shared<uint8_t>(255);
    for (uint8_t idx = 0; idx < 4; idx++) {
        auto s = wind->sections[idx];
        auto result = std::find(s.begin(), s.end(), task);
        if (result != s.end()) {
            *sect = idx;
            break;
        }
    }

    // Add the 4 buttons!
    auto btns = std::make_shared<std::array<SvgBtnWidget*, 4>>();
    for (uint8_t i = 0; i < 4; i++) {
        QString pth = QString(":/assets/addTo%1.svg").arg(i);
        (*btns)[i] = new SvgBtnWidget(pth, wind);
        QObject::connect((*btns)[i], &QPushButton::released, [i, wind, task, sect, btns](){
            (*btns)[*sect]->show();
            updateTaskGroup(task, i, wind);
            (*btns)[i]->hide();
            *sect = i;
        });
        if (i != *sect) {
            (*btns)[i]->show();
        }
    }

    // Add all the widgets to the rmWids and wind->wids lists
    rmWids->push_back(overlay);
    rmWids->push_back(view);
    rmWids->push_back(btn);
    rmWids->push_back(btn2);
    rmWids->push_back(newSubtask);
    for (auto btn : *btns) {
        rmWids->push_back(btn);
    }
    wind->wids.push_back(Widget{overlay, QPoint(0, 0), QSize(100, 100)});
    wind->wids.push_back(Widget{view, QPoint(50, 0), QSize(50, 100)});
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});
    wind->wids.push_back(Widget{btn2, QPoint(99, 1), QSize(8, 8), HEIGHT});
    wind->wids.push_back(Widget{newSubtask, QPoint(95, 50), QSize(18, 6)});
    // The 4 buttons need to be in different spots
    wind->wids.push_back(Widget{(*btns)[0], QPoint(5, 84), QSize(10, 10)});
    wind->wids.push_back(Widget{(*btns)[1], QPoint(16, 84), QSize(10, 10)});
    wind->wids.push_back(Widget{(*btns)[2], QPoint(5, 95), QSize(10, 10)});
    wind->wids.push_back(Widget{(*btns)[3], QPoint(16, 95), QSize(10, 10)});

    wind->resizeElms(); // Update all the positionings!
}
