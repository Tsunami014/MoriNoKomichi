#include "taskwidget.h"
#include "drawtools.h"

#include <QPainter>
#include <QCursor>

TaskWidget::TaskWidget(QString nme) {
    setAcceptHoverEvents(true);
    name = nme;
    makePath();
}
QRectF TaskWidget::boundingRect() const {
    return QRectF(0, 0, paddedWid, 400 + (padding*2));
}
QPainterPath TaskWidget::shape() const {
    return path;
}

void TaskWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event)
    isHover = true;
    setCursor(Qt::PointingHandCursor);
    update(); // force repaint
}
void TaskWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event)
    isHover = false;
    unsetCursor();
    update(); // force repaint
}

void TaskWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);

    // Apply a scaling to everything beyond this point if hovering
    QTransform prevTrans = painter->transform();
    if (isHover) {
        QTransform transform;
        int hoverAmnt = 5;
        QRectF pthBBx = path.boundingRect();
        QPointF center = pthBBx.center();

        transform.translate(center.x(), center.y());
        transform.scale(static_cast<qreal>(hoverAmnt*2) / pthBBx.width() + 1, static_cast<qreal>(hoverAmnt*2) / pthBBx.height() + 1);
        transform.translate(-center.x(), -center.y());

        painter->setTransform(transform * prevTrans);
    }

    // Display polygon
    painter->save();
    painter->setBrush(QBrush(QColor(255, 235, 210)));
    painter->setPen(QPen(Qt::black, 5));
    painter->drawPath(path);
    painter->restore();

    // Display text
    painter->setFont(textFont);
    int fontWid = measure.horizontalAdvance(name);
    painter->drawText(QPoint((width-fontWid)/2 + padding, 40 + padding), name);

    // Reset transform
    painter->setTransform(prevTrans);
}

void TaskWidget::makePath() {
    QRandomGenerator gen = getGen(reinterpret_cast<intptr_t>(this));
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
        QPoint p(x, y);

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
            QPoint first = distort(p, QPoint(xOff*randDec(gen), 0), gen, distortPad, padding);
            QPoint mid = distort(p, QPoint(xOff*randDec(gen), yOff*randDec(gen)), gen, distortPad, padding);
            QPoint last = distort(p, QPoint(0, yOff*randDec(gen)), gen, distortPad, padding);
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
            ps.push_back(distort(p, {0, 0}, gen, distortPad, padding));
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
                ps.push_back(distort(p, {X*pos, Y*pos}, gen, distortPad, padding));
                pos += minSpacing;
            } else {
                int sze = gen.generate()%10+15;
                int halfSze = sze/2;
                ps.push_back(distort(p, {X*pos, Y*pos}, gen, distortPad, padding));
                ps.push_back(distort(p, {X*(pos+halfSze) + XY*opp*sze, Y*(pos+halfSze) + (!XY)*opp*sze}, gen, distortPad, padding));
                ps.push_back(distort(p, {X*(pos+sze), Y*(pos+sze)}, gen, distortPad, padding));
                pos += sze+minSpacing;
            }
        }
    }

    // Make path
    path = genPath(ps, gen, true, mayhem);
}
