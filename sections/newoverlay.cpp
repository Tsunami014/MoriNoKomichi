#include "widgets/svgbtnwidget.h"
#include "sections.h"
#include "../window.h"

#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QApplication>
#include <QTextCursor>

class OverlayWid : public QWidget {
public:
    OverlayWid(std::function<void()> clickFun, QWidget* parent = nullptr) : QWidget(parent) {
        clickFunc = clickFun;
    }

protected:
    void mouseReleaseEvent(QMouseEvent *event) override {
        clickFunc();
        event->accept();
    }
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setBrush(QBrush(QColor(50, 10, 10, 125)));
        painter.drawRect(QRect(QPoint(), size()));
        painter.end();
    }

private:
    std::function<void()> clickFunc;
};

void newOverlay(Window* wind) {
    std::vector<QWidget*>* rmWids = new std::vector<QWidget*>;
    auto backFun = [wind, rmWids](){removeOverlay(wind, rmWids);};

    OverlayWid *overlay = new OverlayWid(backFun, wind);
    overlay->show();

    svgBtnWidget *btn = new svgBtnWidget(":/assets/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, backFun);
    btn->show();

    rmWids->push_back(overlay);
    rmWids->push_back(btn);
    wind->wids.push_back(Widget{overlay, QPoint(-1, -1), QSize(102, 102)});
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});

    wind->resizeElms();
}
