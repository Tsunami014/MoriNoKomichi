#include "playerviewwidget.h"
#include "drawtools.h"

#include <QPainter>

playerViewWidget::playerViewWidget(QWidget *parent)
    : QWidget(parent)
{
    rend = new QSvgRenderer(QString(":/assets/Player.svg"));
}

void playerViewWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(QColor(244, 222, 200)));
    painter.setPen(QPen(Qt::black, 5));
    QRandomGenerator gen = getGen(reinterpret_cast<intptr_t>(this));

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

    painter.drawPath(pth);
    rend->render(&painter, QRectF(QPointF(padding, padding), thisSze));
    painter.end();
}
