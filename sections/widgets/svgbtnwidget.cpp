#include "svgbtnwidget.h"

#include <QPainter>

SvgBtnWidget::SvgBtnWidget(QString path, QWidget *parent, int expandAmount)
    : QPushButton(parent)
{
    expandAmnt = expandAmount;
    svg = new QSvgRenderer(QString(path));
    setMouseTracking(true);
    setCursor(QCursor(Qt::PointingHandCursor));
}

bool SvgBtnWidget::hitButton(const QPoint &pos) const {
    // Make a new QImage
    QImage img(size(), QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);

    // Paint the svg onto the image
    QPainter p(&img);
    svg->render(&p, rect());
    p.end();

    // Find the colour at the pixel of the mouse
    QColor color = img.pixelColor(pos);
    return color.alpha() != 0; // Find if it's transparent
}

void SvgBtnWidget::mouseMoveEvent(QMouseEvent *event) {
    // Only show that you're touching it when your mouse is over the *svg*, not just the button itself
    if (hitButton(event->pos())) {
        setCursor(Qt::PointingHandCursor);
        touching = true;
    } else {
        touching = false;
        unsetCursor();
    }
    QPushButton::mouseMoveEvent(event);
}
void SvgBtnWidget::leaveEvent(QEvent *event) {
    unsetCursor();
    touching = false;
    QPushButton::leaveEvent(event);
}

void SvgBtnWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (touching && expandAmnt > 0) {
        // Transform the svg to expand
        QTransform transform;
        QPointF center(width()/2, height()/2);

        // Transform so the centre is in the middle, scale out and transform back. So it should be scaling where the centre of scaling is the centre.
        transform.translate(center.x(), center.y());
        transform.scale(static_cast<qreal>(expandAmnt*2) / width() + 1, static_cast<qreal>(expandAmnt*2) / height() + 1);
        transform.translate(-center.x(), -center.y());

        painter.setTransform(transform);
    }

    // Paint the svg!
    svg->render(&painter, QRectF(expandAmnt, expandAmnt, width()-(expandAmnt*2), height()-(expandAmnt*2)));

    painter.end(); // Clean up
}
