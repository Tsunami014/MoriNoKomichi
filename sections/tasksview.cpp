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

void taskView(Window* wind) {
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(QColor(250, 230, 200)));

    std::vector<TaskWidget*> sections[4] = {
        {new TaskWidget("Hello group 1"), new TaskWidget("Goodbye group 1"), new TaskWidget("Hello again, group 1"), new TaskWidget("Goodbye again, group 1")},
        {new TaskWidget("Hello group 2"), new TaskWidget("Goodbye group 2"), new TaskWidget("Hello again, group 2"), new TaskWidget("Goodbye again, group 2")},
        {new TaskWidget("Hello group 3"), new TaskWidget("Goodbye group 3"), new TaskWidget("Hello again, group 3"), new TaskWidget("Goodbye again, group 3")},
        {new TaskWidget("Hello group 4"), new TaskWidget("Goodbye group 4"), new TaskWidget("Hello again, group 4"), new TaskWidget("Goodbye again, group 4")}
    };

    int sectPadding = 10;
    int sectWid = 0;
    int sectHei = 0;

    for (int i = 0;i < 4;i++) {
        QGraphicsItemGroup* group = new QGraphicsItemGroup();
        // Don't intercept requests to children
        group->setHandlesChildEvents(false);
        group->setAcceptHoverEvents(false);
        std::vector<int> heights = {2, 2};
        for (TaskWidget* tsk : sections[i]) {
            int idx = 0;
            uint16_t curMin = 0;
            for (int i = 0; i < heights.size(); i++) {
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
        scene->addItem(group);
        if (i == 0) {
            QRectF rect = group->boundingRect();
            sectWid = rect.width();// + sectPadding;
            sectHei = rect.height();// + sectPadding;
        } else {
            QPoint pos;
            if (i == 1 || i == 3) {
                pos.setX(sectWid);
            }
            if (i == 2 || i == 3) {
                pos.setY(sectHei);
            }
            group->setPos(pos);
        }
    }

    GraphicsViewCanvas *view = new GraphicsViewCanvas(scene, wind);
    view->show();
    view->ensureVisible(QRectF(0, 0, 50, 50), 0, 0);
    wind->wids.push_back(Widget{view, QPoint(13, 0), QSize(67, 100)});

    playerViewWidget *playerView = new playerViewWidget(wind);
    playerView->show();
    wind->wids.push_back(Widget{playerView, QPoint(81, 1), QSize(19, 48)});

    svgBtnWidget *btn = new svgBtnWidget(":/assets/UI/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, &Window::gameMenu);
    btn->show();
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});
}
