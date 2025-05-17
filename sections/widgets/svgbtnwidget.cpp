#include "svgbtnwidget.h"

#include <QPainter>

svgBtnWidget::svgBtnWidget(QString path, QWidget *parent, int expandAmount)
    : QPushButton(parent)
{
    expandAmnt = expandAmount;
    svg = new QSvgRenderer(QString(path));
    setMouseTracking(true);
    setCursor(QCursor(Qt::PointingHandCursor));
}

bool svgBtnWidget::hitButton(const QPoint &pos) const {
    QImage img(size(), QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);

    QPainter p(&img);
    svg->render(&p, rect());
    p.end();

    QColor color = img.pixelColor(pos);
    return color.alpha() != 0;
}

void svgBtnWidget::mouseMoveEvent(QMouseEvent *event) {
    if (hitButton(event->pos())) {
        setCursor(Qt::PointingHandCursor);
        touching = true;
    } else {
        touching = false;
        unsetCursor();
    }
    QPushButton::mouseMoveEvent(event);
}
void svgBtnWidget::leaveEvent(QEvent *event) {
    unsetCursor();
    touching = false;
    QPushButton::leaveEvent(event);
}

void svgBtnWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (touching) {
        QTransform transform;
        QPointF center(width()/2, height()/2);

        transform.translate(center.x(), center.y());
        transform.scale(static_cast<qreal>(expandAmnt*2) / width() + 1, static_cast<qreal>(expandAmnt*2) / height() + 1);
        transform.translate(-center.x(), -center.y());

        painter.setTransform(transform);
    }

    svg->render(&painter, QRectF(expandAmnt, expandAmnt, width()-(expandAmnt*2), height()-(expandAmnt*2)));

    painter.end();
}
