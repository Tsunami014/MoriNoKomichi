#include "taskwidget.h"

#include <QPainter>
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

TaskWidget::TaskWidget(QString nme) {
    name = nme;
}
QRectF TaskWidget::boundingRect() const {
    return QRectF(0, 0, paddedWid, 400 + (padding*2));
}

void TaskWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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
