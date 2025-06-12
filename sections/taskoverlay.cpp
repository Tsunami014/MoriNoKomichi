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

    // Make the back btn
    SvgBtnWidget *btn = new SvgBtnWidget(":/assets/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, [wind](){backFun(wind);});
    btn->show();

    // Make the delete btn
    SvgBtnWidget *btn2 = new SvgBtnWidget(":/assets/binBtn.svg", wind);
    // On click delete the task and go back
    wind->connect(btn2, &QPushButton::released, wind, [wind, task](){
        // Delete task from sections
        for (auto& s : wind->sections) {
            auto result = std::find(s.begin(), s.end(), task);
            if (result != std::end(s)) {
                s.erase(result);
                break;
            }
        }

        backFun(wind); // Remove overlay
        delete task; // Delete
        updateTaskPoss(wind); // And update!
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

    // Add all the widgets to the rmWids and wind->wids lists
    rmWids->push_back(overlay);
    rmWids->push_back(view);
    rmWids->push_back(btn);
    rmWids->push_back(btn2);
    rmWids->push_back(newSubtask);
    wind->wids.push_back(Widget{overlay, QPoint(0, 0), QSize(100, 100)});
    wind->wids.push_back(Widget{view, QPoint(50, 0), QSize(50, 100)});
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});
    wind->wids.push_back(Widget{btn2, QPoint(99, 1), QSize(8, 8), HEIGHT});
    wind->wids.push_back(Widget{newSubtask, QPoint(95, 50), QSize(18, 6)});

    wind->resizeElms(); // Update all the positionings!
}
