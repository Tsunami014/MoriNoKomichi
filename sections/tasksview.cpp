#include "widgets/taskwidget.h"
#include "widgets/svgbtnwidget.h"
#include "widgets/graphicsviewcanvas.h"
#include "sections.h"
#include "../window.h"

#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QtMath>

/*!
    \brief A QGraphicsItemGroup but with a fixed bounding box
*/
class BetterGroup : public QGraphicsItemGroup {
public:
    BetterGroup(QGraphicsItem *parent = nullptr) : QGraphicsItemGroup(parent) {}
    QRectF boundingRect() const {
        // must be overloaded, otherwise the boundingrect will only be actualized on
        // additem is actualized. This leads to the fact that the boundingrect
        // will not close around the word items after e.g., moving them.
        return childrenBoundingRect();
    }
};

/*!
    \brief Update the positioning of all the tasks and groups
*/
void updatePoss(std::vector<TaskWidget*> sections[4], BetterGroup* groups[4]) {
    const int sectPadding = 50;
    qreal sectWid = 0;
    qreal sectHei = 0;

    groups[1]->setPos(0, 0);

    // Update all groups
    for (int i = 0;i < 4;i++) {
        std::vector<unsigned int> heights = {2, 2};
        // Update all tasks in this group
        for (TaskWidget* tsk : sections[i]) {
            uint16_t idx = 0;
            unsigned int curMin = 0;
            for (uint16_t i = 0; i < heights.size(); i++) {
                if (curMin == 0 || heights[i] < curMin) {
                    curMin = heights[i];
                    idx = i;
                }
            }
            QRectF tskSze = tsk->boundingRect();
            tsk->setPos(tskSze.width()*idx, curMin);
            heights[idx] = curMin+tskSze.height()+2;
        }

        /*
        Groups
        
        0 | 1
        --+--
        2 | 3

        Groups 0 and 1 set the height, 0 and 2 set the width.
        Although, group 1 has to be re-updated at the end due to it's position possibly changing from group 2 (if group 2 is larger)
        */
        if (i != 3) {
            QRectF rect = groups[i]->boundingRect();
            if (i == 0 || i == 2) {
                sectWid = qMax(sectWid, rect.width() + sectPadding);
            }
            if (i == 0 || i == 1) {
                sectHei = qMax(sectHei, rect.height() + sectPadding);
            }
        }

        // Set group offset
        if (i > 1)  {
            QPoint offset;
            if (i == 3) {
                offset.setX(sectWid);
            }
            offset.setY(sectHei);
            groups[i]->setPos(offset);
        }
    }

    // Update the group 1 after to have the correct x value
    groups[1]->setPos(sectWid, 0);
}

void taskView(Window* wind) {
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(QColor(250, 230, 200)));

    // Temporary example sections
    std::vector<TaskWidget*> sections[4] = {
        {
            MakeTaskWidget("Hello group 1", wind, {"Task 1", "Task 2"}),
            MakeTaskWidget("Reallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallylongname", wind, {"Task 1"}),
            MakeTaskWidget("This name here is quite an extremely super long name for a task, wouldn't you agree?", wind, {"Task 1", "Task 2", "Task 3", "Task 4"}),
            MakeTaskWidget("Goodbye again, group 1", wind, {})
        },
        {MakeTaskWidget("Hello group 2", wind, {}), MakeTaskWidget("Goodbye group 2", wind, {}), MakeTaskWidget("Hello again, group 2", wind, {}), MakeTaskWidget("Goodbye again, group 2", wind, {})},
        {MakeTaskWidget("Hello group 3", wind, {}), MakeTaskWidget("Goodbye group 3", wind, {}), MakeTaskWidget("Hello again, group 3", wind, {}), MakeTaskWidget("Goodbye again, group 3", wind, {})},
        {MakeTaskWidget("Hello group 4", wind, {}), MakeTaskWidget("Goodbye group 4", wind, {}), MakeTaskWidget("Hello again, group 4", wind, {}), MakeTaskWidget("Goodbye again, group 4", wind, {})}
    };

    // Add items to groups, and everything to the screen
    BetterGroup* groups[4];
    for (int i = 0;i < 4;i++) {
        BetterGroup* group = new BetterGroup();
        // Don't intercept requests to children
        group->setHandlesChildEvents(false);
        group->setAcceptHoverEvents(false);
        group->setAcceptedMouseButtons(Qt::NoButton);

        for (TaskWidget* tsk : sections[i]) {
            scene->addItem(tsk);
            group->addToGroup(tsk);
        }
        groups[i] = group;

        scene->addItem(group);
        group->show();
    }

    // Update the positions!
    updatePoss(sections, groups);

    // Add the widgets to the screen

    GraphicsViewCanvas *view = new GraphicsViewCanvas(scene, wind);
    view->show();
    view->gotoTopLeft();
    wind->wids.push_back(Widget{view, QPoint(0, 0), QSize(100, 100)});


    svgBtnWidget *btn = new svgBtnWidget(":/assets/newBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, [wind](){newOverlay(wind);});
    btn->show();
    wind->wids.push_back(Widget{btn, QPoint(91, 91), QSize(8, 8), HEIGHT});
}
