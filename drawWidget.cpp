#include "drawWidget.h"
#include "graphics.h"

#include <QPainter>
#include <QTimer>

DrawWidget::DrawWidget(Graphic *graphics, QWidget *parent)
    : QOpenGLWidget(parent), graphics(graphics)
{
    elapsed = 0;
    setFixedSize(200, 200);
    setAutoFillBackground(false);
}

void DrawWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    graphics->paint(&painter, event, elapsed);
    painter.end();
}
