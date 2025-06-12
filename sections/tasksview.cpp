#include "widgets/taskwidget.h"
#include "widgets/svgbtnwidget.h"
#include "widgets/graphicsviewcanvas.h"
#include "widgets/sectionbgwidget.h"
#include "sections.h"
#include "../window.h"
#include "../taskloading.h"

#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QDebug>
#include <QtMath>
#include <array>

/*! \brief A QGraphicsItemGroup but with a bounding box that actually works! */
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

// Some static vars for this file
/*! \brief The graphical groups that the task widgets go in */
static std::array<QGraphicsItemGroup*, 4> groups;
/*! \brief The graphics scene the groups go in */
static QGraphicsScene* scene = nullptr;
/*! \brief The graphics view for the scene */
static GraphicsViewCanvas* view = nullptr;

/*! \brief Update the positioning of all the tasks and groups */
void updatePoss(std::array<std::vector<TaskWidget*>, 4> sections, std::array<QGraphicsItemGroup*, 4> groups) {
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

void updateTaskGroup(TaskWidget* tsk, uint8_t newGroupNum, Window* wind) {
    // Find existing group num of task
    for (uint8_t idx = 0; idx < 4; idx++) {
        auto& s = wind->sections[idx];
        auto result = std::find(s.begin(), s.end(), tsk);
        if (result != s.end()) {
            // Erase from existing groups and add to new ones
            // Window groups
            s.erase(result);
            wind->sections[newGroupNum].push_back(tsk);
            // Scene groups
            groups[idx]->removeFromGroup(tsk);
            groups[newGroupNum]->addToGroup(tsk);
            // Update and return!
            updateTaskPoss(wind);
            return;
        }
    }

    qWarning("Could not find group of specified task!");
}

void updateTaskPoss(Window* wind) {
    updatePoss(wind->sections, groups); // Update tasks

    // Update the entire scene so nothing is looking smudged
    scene->invalidate(scene->sceneRect(), QGraphicsScene::ForegroundLayer);
    scene->setSceneRect(scene->itemsBoundingRect());
    scene->update();

    // Check canvas scroll based on the new size
    view->checkZoom();

    saveSections(wind);
}

void addItem(TaskWidget* it, uint8_t groupNum, Window* wind) {
    scene->addItem(it);
    groups[groupNum]->addToGroup(it);
    wind->sections[groupNum].push_back(it);

    saveSections(wind);
    // No need to call updateTaskPoss here, is called after this in cases where this is used
}
void removeItem(TaskWidget* tsk, Window* wind) {
    // Find existing group num of task
    for (uint8_t idx = 0; idx < 4; idx++) {
        auto& s = wind->sections[idx];
        auto result = std::find(s.begin(), s.end(), tsk);
        if (result != s.end()) {
            // Erase from groups
            s.erase(result);
            groups[idx]->removeFromGroup(tsk);
            // Update and return!
            updateTaskPoss(wind);
            delete tsk;
            return;
        }
    }

    qWarning("Could not find group of specified task!");
    delete tsk;
}

void taskView(Window* wind) {
    scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(QColor(250, 230, 200)));

    // Add items to groups, and everything to the screen
    for (int i = 0;i < 4;i++) {
        BetterGroup* group = new BetterGroup();
        // Don't intercept requests to children
        group->setHandlesChildEvents(false);
        group->setAcceptHoverEvents(false);
        group->setAcceptedMouseButtons(Qt::NoButton);

        // Add section BG widget attached to group
        SectionBgWidget* sectBg = new SectionBgWidget(i, &groups);
        scene->addItem(sectBg);
        sectBg->show();

        // Add tasks to group
        for (TaskWidget* tsk : wind->sections[i]) {
            scene->addItem(tsk);
            group->addToGroup(tsk);
        }

        // Add group to list and scene
        groups[i] = group;

        scene->addItem(group);
        group->show();
    }

    // Update the positions!
    updatePoss(wind->sections, groups);

    // Add the widgets to the screen

    // Canvas for displaying tasks
    view = new GraphicsViewCanvas(scene, wind);
    view->show();
    view->gotoTopLeft(); // Initialise at top left instead of centre
    wind->wids.push_back(Widget{view, QPoint(0, 0), QSize(100, 100)});

    // New task button
    SvgBtnWidget *btn = new SvgBtnWidget(":/assets/newBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, [wind](){newOverlay(wind);});
    btn->show();
    wind->wids.push_back(Widget{btn, QPoint(99, 99), QSize(8, 8), HEIGHT});

    // Help button
    SvgBtnWidget *btn2 = new SvgBtnWidget(":/assets/helpBtn.svg", wind);
    wind->connect(btn2, &QPushButton::released, wind, [wind](){helpOverlay(wind);});
    btn2->show();
    wind->wids.push_back(Widget{btn2, QPoint(99, 1), QSize(8, 8), HEIGHT});
}
