#include "graphicsviewcanvas.h"

#include <QApplication>
#include <QScrollBar>
#include <qmath.h>

GraphicsViewCanvas::GraphicsViewCanvas(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) {
    setMouseTracking(true);
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    connect(pollTimer, &QTimer::timeout, this, &GraphicsViewCanvas::mousePoll);
}

void GraphicsViewCanvas::offsetPos(int x, int y) { // Uses this instead of regular methods so subclasses can override it
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + x);
    verticalScrollBar()->setValue(verticalScrollBar()->value() + y);
}

void GraphicsViewCanvas::gotoTopLeft() {
    horizontalScrollBar()->setValue(0);
    verticalScrollBar()->setValue(0);
}

void GraphicsViewCanvas::startMousePoll() {
    // If it's the first requirement (the poll wasn't running before), start the poll
    if (pollRequires++ == 0) {
        lastMousePos = mapFromGlobal(QCursor::pos());
        pollTimer->start(16); // ~60 FPS
    }
}
void GraphicsViewCanvas::endMousePoll() {
    // If it's the last requirement, stop the poll
    if (--pollRequires == 0) {
        pollTimer->stop();
        unsetCursor();
    }
}
void GraphicsViewCanvas::mousePoll() {
    // Even though the poll is running, that doesn't mean the canvas should move with the mouse (e.g. with just pressing space)
    // So it has to re-check the buttons pressed
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
    if (delta == 0) {
        return;
    }
    double factor = qPow(1.2, delta/qAbs(delta));
    scale(factor, factor);
    checkZoom();
}
void GraphicsViewCanvas::checkZoom() {
    qreal currentScale = transform().m11();

    QRectF sceneRect = scene()->sceneRect();
    QSizeF viewportSize = viewport()->size();

    if (sceneRect.isEmpty()) {
        if (currentScale != 1) { // Scale back to regular size
            scale(1/currentScale, 1/currentScale);
        }
        return; // Do not scale
    }

    // Find scale ratios
    qreal scaleX = viewportSize.width() / sceneRect.width();
    qreal scaleY = viewportSize.height() / sceneRect.height();

    // Choose the larger one to ensure the entire scene stays within the view (remember these are in 0.xxx)
    qreal min = std::max(scaleX, scaleY);

    // Get what the scale *should* be
    qreal shouldBe = std::min(std::max(currentScale, min), 100.0);
    if (currentScale != shouldBe) { // Only change if there's a difference
        scale(shouldBe/currentScale, shouldBe/currentScale);
    }
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
        // Don't use original method; call offsetPos so *it* handles everything
        QPoint delta = event->angleDelta();
        offsetPos(-delta.x()/2, -delta.y()/2);
    }
}
