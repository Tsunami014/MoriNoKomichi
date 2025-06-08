#include "widgets/taskwidget.h"
#include "widgets/bigtaskwidget.h" // It is used plz no one remove this line
#include "widgets/svgbtnwidget.h"
#include "widgets/graphicsviewcanvas.h"
#include "sections.h"
#include "../window.h"

#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QApplication>
#include <QTextCursor>

// We create our own graphics view for multiple reasons:
// 1. To have a transparent background; where on click, it goes back to the previous screen
// 2. Because the space key has issues
// 3. So resizing the window affects the big task widget display
// 4. To remove horizontal scrolling and zoom
class NewGraphicsView : public GraphicsViewCanvas {
public:
    NewGraphicsView(QGraphicsScene* scene, bigTaskWidget* wid, Window* window)
    : GraphicsViewCanvas(scene, window) {
        wind = window;
        bigW = wid;
        setStyleSheet("background: transparent");
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

protected:
    void mousePressEvent(QMouseEvent* event) override { // If click on bg, run the click func (goes back to prev page)
        QPointF scenePos = mapToScene(event->pos());
        QGraphicsItem* item = scene()->itemAt(scenePos, transform());

        if (item) {
            QGraphicsView::mousePressEvent(event);
        } else {
            backFun(wind);
            event->accept();
        }
    }
    bool event(QEvent *ev) override {
        // As the space key for some reason does not like being passed to the text items,
        // We have to manually insert the space ourselves.
        if (ev->type() == QEvent::KeyPress) {
            auto *keyEv = static_cast<QKeyEvent*>(ev);
            if (keyEv->key() == Qt::Key_Space) {
                auto *item = scene()->focusItem();
                if (item && item->type() == QGraphicsTextItem::Type) {
                    auto *txtItem = static_cast<QGraphicsTextItem*>(item);
                    txtItem->textCursor().insertText(" ");
                    bigW->updateChildren(true, true); // Don't forget to update the children!
                    return true;
                }
            }
        }
        return QGraphicsView::event(ev);

    }
    void offsetPos(int x, int y) override { // Remove x scrolling
        GraphicsViewCanvas::offsetPos(0, y);
    }
    void resizeEvent(QResizeEvent *event) override { // Update bg sizing on resize
        bigW->updateWidth(event->size().width());
        GraphicsViewCanvas::resizeEvent(event);
    }
    void zoom(int delta) {} // Remove zoom
private:
    Window* wind;
    bigTaskWidget* bigW = nullptr;
};

void taskOverlay(Window* wind, TaskWidget* task) {
    // Make the overlay
    OverlayWid *overlay = new OverlayWid(wind);
    overlay->show();

    // Make the graphics scene with the big task widget
    QGraphicsScene* scene = new QGraphicsScene();
    bigTaskWidget* bigW = task->toBigWidget();
    scene->addItem(bigW);
    NewGraphicsView *view = new NewGraphicsView(scene, bigW, wind);
    view->show();

    // Make the back btn
    svgBtnWidget *btn = new svgBtnWidget(":/assets/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, [wind](){backFun(wind);});
    btn->show();

    // Add all the widgets to the rmWids and wind->wids lists
    rmWids->push_back(overlay);
    rmWids->push_back(view);
    rmWids->push_back(btn);
    wind->wids.push_back(Widget{overlay, QPoint(-1, -1), QSize(102, 102)});
    wind->wids.push_back(Widget{view, QPoint(25, 0), QSize(50, 100)});
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});

    wind->resizeElms(); // Update all the positionings!
}
