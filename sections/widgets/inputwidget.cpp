#include "inputwidget.h"
#include "tools/drawtools.h"

#include <QPainter>

InputWidget::InputWidget(QWidget *parent) : QLineEdit(parent) {
    // Set background to transparent and add padding for the text
    setStyleSheet("QLineEdit { background-color: rgba(0, 0, 0, 0); padding-left: 10px; padding-right: 10px; }");
}

void InputWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush({QColor(255, 235, 210)});
    painter.setPen({Qt::black, 5});
    QSize newSze = {width(), height()};
    if (newSze != lastSze) {
        lastSze = newSze;
        pth = roughRect(QRandomGenerator(), newSze, 5, 5);
    }
    painter.drawPath(*pth);
    painter.end();

    QLineEdit::paintEvent(event);
}
