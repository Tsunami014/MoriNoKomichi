#ifndef GRAPHICSVIEWCANVAS_H
#define GRAPHICSVIEWCANVAS_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QTimer>

/*!
    \brief A more interactive GraphicsView

    Includes:
    - Shift+scroll = inverse (vertical->horizontal)
    - Ctrl+scroll = zoom
    - Middle mouse click+drag = move canvas with mouse
    - Space+move mouse = move canvas with mouse
*/
class GraphicsViewCanvas : public QGraphicsView {
    Q_OBJECT

public:
    explicit GraphicsViewCanvas(QGraphicsScene *scene, QWidget *parent = nullptr);
    /*! \brief Move the view to the top left */
    void gotoTopLeft();

protected:
    /*!
        \brief Offset the position of the canvas display

        Handles all the scrolling
    */
    virtual void offsetPos(int x, int y);

    // Mouse polling; checks for mouse movement only when needed
    void startMousePoll();
    void endMousePoll();
    void mousePoll();

    // Check if the right keys/mouse buttons are being held - and start/stop the polling if so
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    /*! \brief Zooms the canvas in/out */
    void zoom(int delta);

    /*! \brief Handles mouse wheel events */
    void wheelEvent(QWheelEvent *event) override;

private:
    /*!
        \brief Tracks the number of reasons for the mouse polling to be running

        Since there are multiple reasons to poll the mouse (space+click+drag and middleclick+drag), sometimes multiple of these will be active at the same time.
        This keeps track of the amount of reasons the polling has for running; and if it is 0, the mouse polling stops.
    */
    uint8_t pollRequires = 0;
    /*! \brief Keeps track of the last mouse position */
    QPoint lastMousePos;
    /*! \brief A timer for the mouse polling */
    QTimer *pollTimer = new QTimer(this);
};

#endif // GRAPHICSVIEWCANVAS_H
