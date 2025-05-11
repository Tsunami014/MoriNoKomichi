#ifndef TASKSVIEW_H
#define TASKSVIEW_H

#include "../window.h"

#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QRandomGenerator>

class TaskWidget : public QGraphicsItem {
public:
    TaskWidget(QString nme) {
        name = nme;
    }

    const static inline int width = 600;

    QRectF boundingRect() const override {
        return QRectF(0, 0, width, 100);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        QRandomGenerator gen = QRandomGenerator(reinterpret_cast<intptr_t>(this));

        int height = boundingRect().height();

        painter->setFont(textFont);
        int fontWid = measure.horizontalAdvance(name);
        painter->drawText(QPoint((width-fontWid)/2, 40), name);

        int padding = 2;
        int waveHei = 50;
        int waveDiffAmnt = 5;
        int waveMidAmnt = 12;

        std::vector<QPoint> ps = {
            QPoint(padding, padding)
        };
        qint16 rndAmnt = gen.generate();
        rndAmnt = rndAmnt % waveDiffAmnt + waveMidAmnt;
        rndAmnt = rndAmnt + (rndAmnt % 2);
        uint8_t pos = 0;
        float diff = (width - padding * 2) / rndAmnt;
        for (int i = 1; i < rndAmnt; i++) {
            pos = waveHei - pos;
            ps.push_back(QPoint(diff * i, pos+padding));
        }
        ps.push_back(QPoint(width-padding, padding));
        ps.push_back(QPoint(width-padding, height-padding));

        rndAmnt = gen.generate();
        rndAmnt = rndAmnt % waveDiffAmnt + waveMidAmnt;
        rndAmnt = rndAmnt + (rndAmnt % 2);
        pos = 0;
        diff = (width - padding * 2) / rndAmnt;
        for (int i = 1; i < rndAmnt; i++) {
            pos = waveHei - pos;
            ps.push_back(QPoint(width - (diff * i), height-(pos+padding)));
        }
        ps.push_back(QPoint(padding, height-padding));

        for (QPoint& p : ps) {
            qint16 rndX = gen.generate();
            qint16 rndY = gen.generate();
            p.setX(p.x()+(rndX%(padding*2))-padding);
            p.setY(p.y()+(rndY%(padding*2))-padding);
        }
        QVector<QLine> lns;
        QPoint prevP = ps.back();
        for (QPoint p : ps) {
            lns.push_back(QLine(prevP, p));
            prevP = p;
        }
        painter->drawLines(lns);
    }

private:
    QString name;
    static inline QFont textFont = QFont("Segoe Script", 14);
    static inline QFontMetrics measure = QFontMetrics(textFont);
};

void taskView(Window* wind) {
    QGraphicsScene* scene = new QGraphicsScene();

    std::vector<TaskWidget*> sections[4] = {
        {new TaskWidget("Hello group 1"), new TaskWidget("Goodbye group 1"), new TaskWidget("Hello again, group 1"), new TaskWidget("Goodbye again, group 1")},
        {new TaskWidget("Hello group 2"), new TaskWidget("Goodbye group 2"), new TaskWidget("Hello again, group 2"), new TaskWidget("Goodbye again, group 2")},
        {new TaskWidget("Hello group 3"), new TaskWidget("Goodbye group 3"), new TaskWidget("Hello again, group 3"), new TaskWidget("Goodbye again, group 3")},
        {new TaskWidget("Hello group 4"), new TaskWidget("Goodbye group 4"), new TaskWidget("Hello again, group 4"), new TaskWidget("Goodbye again, group 4")}
    };

    int sectWid = 0;
    int sectHei = 0;

    for (int i = 0;i < 4;i++) {
        QGraphicsItemGroup* group = new QGraphicsItemGroup();
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
            QTransform rotMat;
            rotMat.translate(TaskWidget::width*idx, curMin);
            tsk->setTransform(rotMat);
            heights[idx] = curMin+tskSze.height()+2;
            scene->addItem(tsk);
            group->addToGroup(tsk);
        }
        scene->addItem(group);
        if (i == 0) {
            QRectF rect = group->boundingRect();
            sectWid = rect.width();
            sectHei = rect.height();
        } else {
            QTransform rotMat;
            if (i == 1 || i == 3) {
                rotMat.translate(sectWid, 0);
            }
            if (i == 2 || i == 3) {
                rotMat.translate(0, sectHei);
            }
            group->setTransform(rotMat);
        }
    }

    QGraphicsView *view = new QGraphicsView(scene, wind);
    view->show();
    wind->wids.push_back(Widget{view, QPoint(0, 0), QSize(100, 100)});

    QPushButton *btn = new QPushButton("<-", wind);
    wind->connect(btn, &QPushButton::released, wind, &Window::gameMenu);
    btn->show();
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(5, 5), HEIGHT});
}

#endif // TASKSVIEW_H
