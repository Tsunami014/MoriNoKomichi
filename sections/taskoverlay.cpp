#include "widgets/taskwidget.h"
#include "widgets/bigtaskwidget.h" // It is used plz no one remove this line
#include "widgets/svgbtnwidget.h"
#include "widgets/graphicsviewcanvas.h"
#include "../window.h"

#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QApplication>

class OverlayWid : public QWidget {
public:
    OverlayWid(std::function<void()> clickFun, QWidget* parent = nullptr) : QWidget(parent) {clickFunc = clickFun;}

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

class NewGraphicsView : public GraphicsViewCanvas {
public:
    NewGraphicsView(QGraphicsScene* scene, std::function<void()> clickFun, Window* wind) : GraphicsViewCanvas(scene, wind) {
        clickFunc = clickFun;
        setStyleSheet("background: transparent");
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

protected:
    void mousePressEvent(QMouseEvent* event) override {
        QPointF scenePos = mapToScene(event->pos());
        QGraphicsItem* item = scene()->itemAt(scenePos, transform());

        if (item) {
            QGraphicsView::mousePressEvent(event);
        } else {
            clickFunc();
            event->accept();
        }
    }
    void offsetPos(int x, int y) override { // Remove x scrolling
        GraphicsViewCanvas::offsetPos(0, y);
    }
    void resizeEvent(QResizeEvent *event) override {
        dynamic_cast<bigTaskWidget*>(scene()->items()[0])->updatePath(event->size().width());
        GraphicsViewCanvas::resizeEvent(event);
    }
    void zoom(int delta) {} // Remove zoom
private:
    std::function<void()> clickFunc;
};

void taskOverlay(Window* wind, TaskWidget* task) {
    auto backFun = [wind](){removeOverlay(wind);};
    OverlayWid *overlay = new OverlayWid(backFun, wind);
    overlay->show();
    wind->wids.push_back(Widget{overlay, QPoint(-1, -1), QSize(102, 102)});

    QGraphicsScene* scene = new QGraphicsScene();
    scene->addItem(task->toBigWidget());
    NewGraphicsView *view = new NewGraphicsView(scene, backFun, wind);
    view->show();
    wind->wids.push_back(Widget{view, QPoint(25, 0), QSize(50, 100)});

    svgBtnWidget *btn = new svgBtnWidget(":/assets/UI/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, backFun);
    btn->show();
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});

    wind->resizeElms();
}
