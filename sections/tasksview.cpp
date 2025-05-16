#include "widgets/taskwidget.h"
#include "widgets/playerviewwidget.h"
#include "widgets/svgbtnwidget.h"
#include "widgets/graphicsviewcanvas.h"
#include "../window.h"

#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QtMath>

void taskView(Window* wind) {
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(QColor(250, 230, 200)));

    std::vector<TaskWidget*> sections[4] = {
        {new TaskWidget("Hello group 1", wind), new TaskWidget("Goodbye group 1", wind), new TaskWidget("Hello again, group 1", wind), new TaskWidget("Goodbye again, group 1", wind)},
        {new TaskWidget("Hello group 2", wind), new TaskWidget("Goodbye group 2", wind), new TaskWidget("Hello again, group 2", wind), new TaskWidget("Goodbye again, group 2", wind)},
        {new TaskWidget("Hello group 3", wind), new TaskWidget("Goodbye group 3", wind), new TaskWidget("Hello again, group 3", wind), new TaskWidget("Goodbye again, group 3", wind)},
        {new TaskWidget("Hello group 4", wind), new TaskWidget("Goodbye group 4", wind), new TaskWidget("Hello again, group 4", wind), new TaskWidget("Goodbye again, group 4", wind)}
    };

    int sectPadding = 50;
    qreal sectWid = 0;
    qreal sectHei = 0;

    QGraphicsItemGroup* group1;

    for (int i = 0;i < 4;i++) {
        QGraphicsItemGroup* group = new QGraphicsItemGroup();
        // Don't intercept requests to children
        group->setHandlesChildEvents(false);
        group->setAcceptHoverEvents(false);
        group->setAcceptedMouseButtons(Qt::NoButton);

        scene->addItem(group);
        group->show();

        std::vector<unsigned int> heights = {2, 2};
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
            tsk->setPos(TaskWidget::paddedWid*idx, curMin);
            heights[idx] = curMin+tskSze.height()+2;
            scene->addItem(tsk);
            group->addToGroup(tsk);
        }

        if (i == 1)  {
            group1 = group; // Update position after to get correct position
        } else {
            QPoint offset;
            if (i == 3) {
                offset.setX(sectWid);
            }
            if (i == 2 || i == 3) {
                offset.setY(sectHei);
            }
            group->setPos(offset);
        }

        if (i != 3) {
            QRectF rect = group->boundingRect();
            if (i == 0 || i == 2) {
                sectWid = qMax(sectWid, rect.width() + sectPadding);
            } else if (i == 0 || i == 1) {
                sectHei = qMax(sectHei, rect.height() + sectPadding);
            }
        }
    }

    // Update the sections[1] after to have the correct x value
    group1->setPos(sectWid, 0);

    GraphicsViewCanvas *view = new GraphicsViewCanvas(scene, wind);
    view->show();
    view->gotoTopLeft();
    wind->wids.push_back(Widget{view, QPoint(13, 0), QSize(67, 100)});

    playerViewWidget *playerView = new playerViewWidget(wind);
    playerView->show();
    wind->wids.push_back(Widget{playerView, QPoint(81, 1), QSize(19, 48)});

    svgBtnWidget *btn = new svgBtnWidget(":/assets/UI/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, &Window::gameMenu);
    btn->show();
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});
}
