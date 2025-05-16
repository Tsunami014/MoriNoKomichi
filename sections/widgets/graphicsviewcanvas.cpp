#include "graphicsviewcanvas.h"
#include "qapplication.h"
#include <QScrollBar>
#include <QtCore/qmath.h>
GraphicsViewCanvas::GraphicsViewCanvas(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) {
    setMouseTracking(true);
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    connect(pollTimer, &QTimer::timeout, this, &GraphicsViewCanvas::mousePoll);
}

void GraphicsViewCanvas::offsetPos(int x, int y) {
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + x);
    verticalScrollBar()->setValue(verticalScrollBar()->value() + y);
}

void GraphicsViewCanvas::gotoTopLeft() {
    horizontalScrollBar()->setValue(0);
    verticalScrollBar()->setValue(0);
}

void GraphicsViewCanvas::startMousePoll() {
    if (pollRequires++ == 0) {
        lastMousePos = mapFromGlobal(QCursor::pos());
        pollTimer->start(16); // ~60 FPS
    }
}
void GraphicsViewCanvas::endMousePoll() {
    if (--pollRequires == 0) {
        pollTimer->stop();
        unsetCursor();
    }
}
void GraphicsViewCanvas::mousePoll() {
    QPoint mousePos = mapFromGlobal(QCursor::pos());
    Qt::MouseButtons btns = QApplication::mouseButtons();
    if (btns & Qt::LeftButton || btns & Qt::MiddleButton) {
        offsetPos(lastMousePos.x() - mousePos.x(), lastMousePos.y() - mousePos.y());
        setCursor(Qt::ClosedHandCursor);
    } else {
        unsetCursor();
    }
    lastMousePos = mousePos;
}

void GraphicsViewCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        startMousePoll();
        event->accept();
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}
void GraphicsViewCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        endMousePoll();
        event->accept();
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void GraphicsViewCanvas::keyPressEvent(QKeyEvent *event) {
    if ((!event->isAutoRepeat()) && event->key() == Qt::Key_Space) {
        startMousePoll();
        event->accept();
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}
void GraphicsViewCanvas::keyReleaseEvent(QKeyEvent *event) {
    if ((!event->isAutoRepeat()) && event->key() == Qt::Key_Space) {
        endMousePoll();
        event->accept();
    } else {
        QGraphicsView::keyReleaseEvent(event);
    }
}

void GraphicsViewCanvas::zoom(int delta) {
    double factor = qPow(1.2, delta/qAbs(delta));
    scale(factor, factor);
}
void GraphicsViewCanvas::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        QPoint delta = event->angleDelta();
        zoom(delta.x() + delta.y());
    }
    else if (event->modifiers() & Qt::ShiftModifier) {
        QPoint delta = event->angleDelta();
        offsetPos(-delta.y()/2, -delta.x()/2);
    }
    else {
        QGraphicsView::wheelEvent(event);
    }
}
