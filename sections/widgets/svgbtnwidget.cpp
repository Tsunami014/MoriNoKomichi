#include "svgbtnwidget.h"

#include <QPainter>

svgBtnWidget::svgBtnWidget(QString path, QWidget *parent)
    : QPushButton(parent)
{
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
    } else {
        unsetCursor();
    }
    QPushButton::mouseMoveEvent(event);
}

void svgBtnWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    svg->render(&painter, QRectF(QPoint(0, 0), size()));

    painter.end();
}
