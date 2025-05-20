#include "playerviewwidget.h"
#include "drawtools.h"

#include <QPainter>

playerViewWidget::playerViewWidget(QWidget *parent)
    : QWidget(parent)
{
    bg = new QSvgRenderer(QString(":/assets/player/bg.svg"));
    rend = new QSvgRenderer(QString(path));
}

void playerViewWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 5));
    QRandomGenerator gen = getGen(path);

    int padding = 10;
    QSize thisSze(width()-(padding*2), height()-(padding*2));
    int corner_radius = 10;

    struct corner1 {
        QPoint p;
        int8_t move_x;
        int8_t move_y;
    };
    corner1 ps1[4] = {
        {{padding, padding}, 0, 1},
        {{padding, thisSze.height()}, 1, 0},
        {{thisSze.width(), thisSze.height()}, 0, -1},
        {{thisSze.width(), padding}, -1, 0}
    };

    struct corner2 {
        QPoint p;
        int8_t move_x;
        int8_t move_y;
    };
    QPoint ps2[8];
    for (int idx = 0; idx < 4; idx++) {
        ps2[idx*2] = distort(ps1[idx].p, {ps1[idx].move_y*corner_radius*-1, ps1[idx].move_x*corner_radius*-1}, gen);
        ps2[idx*2+1] = distort(ps1[idx].p, {ps1[idx].move_x*corner_radius, ps1[idx].move_y*corner_radius}, gen);
    }

    QPainterPath pth(ps2[0]);
    bool state = true; // Corner (true) or edge (false)
    uint8_t edge = 0;
    for (int idx = 0; idx < 8; idx++) {
        QPoint next = ps2[(idx+1)%8];
        if (state) {
            QPoint control = distort(ps1[edge++].p, {0, 0}, gen);
            pth.quadTo(control, next);
        } else {
            continuePath(&pth, std::vector<QPoint>{next}, gen);
        }
        state = !state;
    }

    painter.setClipPath(pth);
    bg->render(&painter, QRectF(QPointF(0, 0), size()));

    QRectF paintR(QPointF(padding, padding), thisSze);
    rend->render(&painter, paintR);

    painter.setClipRect(QRect(), Qt::NoClip); // Remove clipping
    painter.drawPath(pth);
    painter.end();
}
