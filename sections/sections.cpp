#include "sections.h"
#include "widgets/graphicsviewcanvas.h"
#include "widgets/bigtaskwidget.h"

#include <QPainter>
#include <QDebug>

OverlayWid::OverlayWid(Window* window) : QWidget(window) {
    wind = window;
}
void OverlayWid::mouseReleaseEvent(QMouseEvent *event) {
    backFun(wind);
    event->accept();
}
void OverlayWid::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(50, 10, 10, 125)));
    painter.drawRect(QRect(QPoint(), size()));
    painter.end();
}

/*!
    \brief Remove specified widgets from a window

    (made for overlay views; to just remove tasks in an overlay and leave the main tasks view untouched)
*/
void removeOverlay(Window* wind, std::vector<QWidget*>* wids) {
    if (wids->empty()) {
        return;
    }

    unsigned int start = wids->size()-1;
    // Go through each widget in the list provided in reverse order (higher likelihood for user to have put them in order so we can use the fast .pop_back instead)
    for (unsigned int i = start; i <= start; i--) {
        QWidget* toFindWid = (*wids)[i];
        unsigned int start2 = wind->wids.size()-1;
        bool found = false;
        // Have to use manual loop as the wind->wids is a vector of Widgets which *contain* QWidgets
        for (unsigned int j = start2; j >= 0; j--) {
            if (wind->wids[j].wid == toFindWid) {
                if (j == start2) { // Use faster operation if possible
                    wind->wids.pop_back();
                } else {
                    wind->wids.erase(wind->wids.begin()+j);
                }
                found = true;
                break;
            }
        }
        toFindWid->deleteLater();
        if (!found) {
            qCritical() << "Couldn't find element " << i << " in window widgets to delete!";
        }
    }
    wids->clear();
    wind->resizeElms();
}

std::vector<QWidget*>* rmWids = new std::vector<QWidget*>; // Starts as an empty list
std::function<void(Window*)> backFun = [](Window* wind){ removeOverlay(wind, rmWids); }; // Run the removeOverlay func (specified in this file only) with the rmWids


NewGraphicsView::NewGraphicsView(QGraphicsScene* scene, BigTaskWidget* wid, Window* window)
    : GraphicsViewCanvas(scene, window) {
    // Some vars to set that make this class special
    wind = window;
    bigW = wid;
    setStyleSheet("background: transparent");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QObject::connect(bigW, &TaskWidget::updating, this, &NewGraphicsView::updateRect); // Ensure on big task widget update it updates the bbx of this view
}

void NewGraphicsView::updateRect() {
    scene()->setSceneRect(scene()->itemsBoundingRect());
}

void NewGraphicsView::mousePressEvent(QMouseEvent* event) {
    // Ensure that if you click on not the bg it doesn't go back and handles the click appropriately
    QPointF scenePos = mapToScene(event->pos());
    QGraphicsItem* item = scene()->itemAt(scenePos, transform());

    if (item) {
        QGraphicsView::mousePressEvent(event);
    } else {
        backFun(wind);
        event->accept();
    }
}
bool NewGraphicsView::event(QEvent *ev) {
    // As the space key for some reason does not like being passed to the text items,
    // We have to manually insert the space ourselves.

    // Ensure the event is the specific type we want to catch, for the specific type of parent
    if (ev->type() == QEvent::KeyPress) {
        auto *keyEv = static_cast<QKeyEvent*>(ev);
        if (keyEv->key() == Qt::Key_Space) {
            auto *item = scene()->focusItem();
            if (item && item->type() == QGraphicsTextItem::Type) {
                // The title text boxes
                auto *txtItem = static_cast<QGraphicsTextItem*>(item);
                txtItem->textCursor().insertText(" ");
                bigW->updateChildren(true, true); // Don't forget to update the children!
                return true;
            } else if (item && item->type() == QGraphicsProxyWidget::Type) {
                // The todo items - update their label!
                auto *proxyItem = static_cast<TodoGraphicObject*>(item);
                proxyItem->addText(" ");
                return true;
            }
        }
    }
    return QGraphicsView::event(ev);

}

void NewGraphicsView::offsetPos(int x, int y) {
    GraphicsViewCanvas::offsetPos(0, y);
}
void NewGraphicsView::resizeEvent(QResizeEvent *event) {
    bigW->updateWidth(event->size().width());
    GraphicsViewCanvas::resizeEvent(event);
    updateRect();
}
void NewGraphicsView::zoom(int delta) {}
