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

void removeOverlay(Window* wind, std::vector<QWidget*>* wids) {
    if (wids->empty()) {
        delete wids;
        return;
    }
    for (int i = wind->wids.size()-1; i >= 0; i--) {
        auto wid = wind->wids[i].wid;
        int start = wids->size()-1;
        for (int j = start; j >= 0; j--) {
            if (wids->at(j) == wid) {
                if (j == start) {
                    delete wids->back();
                    wids->pop_back();
                } else {
                    delete wids->at(j);
                    wids->erase(wids->begin() + j);
                }
                if (i == wind->wids.size()-1) {
                    wind->wids.pop_back();
                } else {
                    wind->wids.erase(wind->wids.begin() + j);
                }
                break;
            }
        }
        if (wids->empty()) {
            break;
        }
    }
    delete wids;
    wind->resizeElms();
}

void taskOverlay(Window* wind, TaskWidget* task) {
    std::vector<QWidget*>* rmWids = new std::vector<QWidget*>;
    auto backFun = [wind, rmWids](){removeOverlay(wind, rmWids);};

    OverlayWid *overlay = new OverlayWid(backFun, wind);
    overlay->show();

    QGraphicsScene* scene = new QGraphicsScene();
    scene->addItem(task->toBigWidget());
    NewGraphicsView *view = new NewGraphicsView(scene, backFun, wind);
    view->show();

    svgBtnWidget *btn = new svgBtnWidget(":/assets/UI/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, backFun);
    btn->show();

    rmWids->push_back(overlay);
    rmWids->push_back(view);
    rmWids->push_back(btn);
    wind->wids.push_back(Widget{overlay, QPoint(-1, -1), QSize(102, 102)});
    wind->wids.push_back(Widget{view, QPoint(25, 0), QSize(50, 100)});
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});

    wind->resizeElms();
}
