#include "widgets/svgbtnwidget.h"
#include "../window.h"

#include <QWidget>
#include <QPainter>

class OverlayWid : public QWidget {
public:
    OverlayWid(QWidget *parent = nullptr) : QWidget(parent) {}

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setBrush(QBrush(QColor(50, 10, 10, 125)));
        painter.drawRect(QRect(QPoint(), size()));
        painter.end();
    }
};

void removeOverlay(Window* wind) {
    bool wasOverlay = false;
    while (!wind->wids.empty() && !wasOverlay) {
        QWidget* back = wind->wids.back().wid;
        wasOverlay = dynamic_cast<const OverlayWid*>(back) != nullptr;
        delete back;
        wind->wids.pop_back();
    }
    wind->resizeElms();
}

void taskOverlay(Window* wind) {
    OverlayWid *view = new OverlayWid(wind);
    view->show();
    wind->wids.push_back(Widget{view, QPoint(-1, -1), QSize(102, 102)});

    svgBtnWidget *btn = new svgBtnWidget(":/assets/UI/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, [wind](){removeOverlay(wind);});
    btn->show();
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});

    wind->resizeElms();
}
