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
    const static inline int padding = 20;
    const static inline int paddedWid = width + (padding*2);

    QRectF boundingRect() const override {
        return QRectF(0, 0, paddedWid, 400 + (padding*2));
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        QRandomGenerator gen = QRandomGenerator(reinterpret_cast<intptr_t>(this));

        int height = boundingRect().height() - (padding*2);

        // Make polygon
        int distortPad = 2;
        int waveHei = 50;
        int waveDiffAmnt = 5;
        int waveMidAmnt = 12;

        QVector<QPoint> ps = {
            QPoint(distortPad, distortPad)
        };
        qint16 rndAmnt = gen.generate();
        rndAmnt = rndAmnt % waveDiffAmnt + waveMidAmnt;
        rndAmnt = rndAmnt + (rndAmnt % 2);
        uint8_t pos = 0;
        float diff = (width - distortPad * 2) / rndAmnt;
        for (int i = 1; i < rndAmnt; i++) {
            pos = waveHei - pos;
            ps.push_back(QPoint(diff * i, pos+distortPad));
        }
        ps.push_back(QPoint(width-distortPad, distortPad));
        ps.push_back(QPoint(width-distortPad, height-distortPad));

        rndAmnt = gen.generate();
        rndAmnt = rndAmnt % waveDiffAmnt + waveMidAmnt;
        rndAmnt = rndAmnt + (rndAmnt % 2);
        pos = 0;
        diff = (width - distortPad * 2) / rndAmnt;
        for (int i = 1; i < rndAmnt; i++) {
            pos = waveHei - pos;
            ps.push_back(QPoint(width - (diff * i), height-(pos+distortPad)));
        }
        ps.push_back(QPoint(distortPad, height-distortPad));

        for (QPoint& p : ps) {
            qint16 rndX = gen.generate();
            qint16 rndY = gen.generate();
            p.setX(p.x()+(rndX%(distortPad*2))-distortPad + padding);
            p.setY(p.y()+(rndY%(distortPad*2))-distortPad + padding);
        }

        // Display polygon
        painter->save();
        QPolygon poly = QPolygon(ps);
        painter->setBrush(QBrush(QColor(245, 240, 250)));
        painter->setPen(QPen(Qt::black, 5));
        painter->drawPolygon(poly);
        painter->restore();

        // Display text
        painter->setFont(textFont);
        int fontWid = measure.horizontalAdvance(name);
        painter->drawText(QPoint((width-fontWid)/2 + padding, 40 + padding), name);
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
            rotMat.translate(TaskWidget::paddedWid*idx, curMin);
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
