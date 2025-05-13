#ifndef TASKSVIEW_H
#define TASKSVIEW_H

#include "../window.h"

#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QRandomGenerator>

QPoint distort(int x, int y, QRandomGenerator gen, int distortPad, int padding) {
    return QPoint(
        x + (gen.generate()%(distortPad*2) - distortPad) + padding,
        y + (gen.generate()%(distortPad*2) - distortPad) + padding
    );
}

float randDec(QRandomGenerator gen) {
    return 1+(gen.generate()%5)/10;
}

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

        painter->setRenderHint(QPainter::Antialiasing);
        QRandomGenerator gen = QRandomGenerator(reinterpret_cast<intptr_t>(this));
        int height = boundingRect().height() - (padding*2);

        // Make polygon
        int8_t distortPad = 10;
        int8_t mayhem = 5;
        int8_t cornerOffRnd = 30;

        struct Corner {
            bool x;
            bool y;
        };

        Corner ps1[4] = {
            Corner{false, false},
            Corner{true, false},
            Corner{true, true},
            Corner{false, true},
        };

        std::vector<QPoint> ps;
        for (int8_t idx = 0; idx < 4; idx++) {
            // Torn/regular corner
            int x;
            if (ps1[idx].x) {
                x = width-distortPad;
            } else {
                x = distortPad;
            }
            int y;
            if (ps1[idx].y) {
                y = height-distortPad;
            } else {
                y = distortPad;
            }

            if (gen.generate()%7 == 0) {
                int8_t xOff;
                int8_t xOffRnd = gen.generate()%cornerOffRnd;
                if (ps1[idx].x) {
                    xOff = -xOffRnd;
                } else {
                    xOff = xOffRnd;
                }
                int8_t yOff;
                int8_t yOffRnd = gen.generate()%cornerOffRnd;
                if (ps1[idx].y) {
                    yOff = -yOffRnd;
                } else {
                    yOff = yOffRnd;
                }
                QPoint first = distort(x+xOff*randDec(gen), y, gen, distortPad, padding);
                QPoint mid = distort(x+xOff*randDec(gen), y+yOff*randDec(gen), gen, distortPad, padding);
                QPoint last = distort(x, y+yOff*randDec(gen), gen, distortPad, padding);
                if (ps1[idx].x ^ ps1[idx].y) {
                    ps.push_back(first);
                    ps.push_back(mid);
                    ps.push_back(last);
                } else {
                    ps.push_back(last);
                    ps.push_back(mid);
                    ps.push_back(first);
                }
            } else {
                ps.push_back(distort(x, y, gen, distortPad, padding));
            }

            // Rips
            int8_t XY = ps1[idx].x ^ ps1[idx].y; // 0 for x, 1 for y
            int8_t dir = 1;
            if (ps1[idx].y) {
                dir = -1;
            }
            int8_t opp = -1;
            if (XY^(dir==1)) {
                opp = 1;
            }
            int8_t X = (!XY)*dir;
            int8_t Y = XY*dir;

            int pos = cornerOffRnd*2 + distortPad;
            int max;
            if (XY) {
                max = height - pos;
            } else {
                max = width - pos;
            }
            max -= cornerOffRnd*2;
            int minSpacing = max/10;
            while (true) {
                pos += gen.generate()%max;
                if (pos >= max) {
                    break;
                }
                if (idx == 0) {
                    if (pos > max/4 && pos < (max/4)*3) {
                        continue;
                    }
                }
                if (gen.generate()%2 == 0) {
                    ps.push_back(distort(x + X*pos, y + Y*pos, gen, distortPad, padding));
                    pos += minSpacing;
                } else {
                    int sze = gen.generate()%10+15;
                    int halfSze = sze/2;
                    ps.push_back(distort(x + X*pos, y + Y*pos, gen, distortPad, padding));
                    ps.push_back(distort(x + X*(pos+halfSze) + XY*opp*sze, y + Y*(pos+halfSze) + (!XY)*opp*sze, gen, distortPad, padding));
                    ps.push_back(distort(x + X*(pos+sze), y + Y*(pos+sze), gen, distortPad, padding));
                    pos += sze+minSpacing;
                }
            }
        }

        QPoint curPos = ps.back();
        QPainterPath pth(curPos);
        for (QPoint& p : ps) {
            switch (gen.generate()%4) {
                case 0:
                    pth.lineTo(p);
                    break;
                case 1: {
                    int diffX = (p.x() - curPos.x()) / 3;
                    int diffY = (p.y() - curPos.y()) / 3;
                    QPoint thirdP1 = QPoint(
                        curPos.x() + diffX + gen.generate()%mayhem,
                        curPos.y() + diffY + gen.generate()%mayhem
                    );
                    QPoint thirdP2 = QPoint(
                        curPos.x() + diffX*2 + gen.generate()%mayhem,
                        curPos.y() + diffY*2 + gen.generate()%mayhem
                    );
                    pth.cubicTo(thirdP1, thirdP2, p);
                    break;
                }
                case 2: {
                    QPoint midP = QPoint(
                        (p.x()+curPos.x())/2,
                        (p.y()+curPos.y())/2
                    );
                    pth.cubicTo(midP.x()+gen.generate()%mayhem,
                                midP.y()+gen.generate()%mayhem,
                                midP.x()+gen.generate()%mayhem,
                                midP.y()+gen.generate()%mayhem,
                                p.x(), p.y()
                    );
                    break;
                }
                case 3: {
                    QPoint midP = QPoint(
                        (p.x()+curPos.x())/2 + gen.generate()%mayhem,
                        (p.y()+curPos.y())/2 + gen.generate()%mayhem
                    );
                    pth.quadTo(midP, p);
                    break;
                }
            }
            curPos = p;
        }

        // Display polygon
        painter->save();
        painter->setBrush(QBrush(QColor(255, 235, 210)));
        painter->setPen(QPen(Qt::black, 5));
        painter->drawPath(pth);
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
    scene->setBackgroundBrush(QBrush(QColor(250, 230, 200)));

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
