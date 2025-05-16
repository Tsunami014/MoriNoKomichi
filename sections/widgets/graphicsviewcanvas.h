#ifndef GRAPHICSVIEWCANVAS_H
#define GRAPHICSVIEWCANVAS_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QTimer>

class GraphicsViewCanvas : public QGraphicsView {
    Q_OBJECT

public:
    explicit GraphicsViewCanvas(QGraphicsScene *scene, QWidget *parent = nullptr);
    void gotoTopLeft();

protected:
    void offsetPos(int x, int y);
    void startMousePoll();
    void endMousePoll();
    void mousePoll();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void zoom(int delta);
    void wheelEvent(QWheelEvent *event) override;

private:
    uint8_t pollRequires = 0;
    QPoint lastMousePos;
    QTimer *pollTimer = new QTimer(this);
};

#endif // GRAPHICSVIEWCANVAS_H
