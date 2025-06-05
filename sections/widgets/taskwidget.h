#ifndef TASKSVIEW_H
#define TASKSVIEW_H

#include "../../window.h"

#include <QFont>
#include <QGraphicsObject>
#include <QMouseEvent>
#include <QGraphicsTextItem>
#include <qgraphicsproxywidget.h>
#include <qlineedit.h>

class bigTaskWidget; // Forward reference
class TaskWidget; // Forward reference

/*!
    \brief This is the editable label for todo objects (below)
*/
class MyLabel : public QLineEdit {
    Q_OBJECT
public:
    MyLabel(const QString& text, bool enabled = true, QWidget* parent = nullptr);
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
private:
    bool enabled;
};

/*!
    \brief This is the todo object, containing the name and QWidgets that compose of a todo

    It turns the QWidgets into QGraphicsWidgets through its parent QGraphicsProxyWidget.
*/
class TodoGraphicObject : public QGraphicsProxyWidget {
    Q_OBJECT
public:
    TodoGraphicObject(QString nme, bool editable, TaskWidget* parent);

    QString name;
};

/*!
    A QGraphicsObject which is a task; a title with a bunch of todos and a fancy border, that has a fancy hover animation and on click launches a taskOverlay to display the task in detail.
*/
class TaskWidget : public QGraphicsObject {
    Q_OBJECT
public:
    TaskWidget(QString nme, Window* window, std::vector<QString> todos, QGraphicsItem* parent = nullptr, bool editable = false);
    virtual void makePath();

    int width = 600;
    const static inline int padding = 20;

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /*!
        \brief Convert this widget to a bigTaskWidget

        This is for displaying in the taskOverlay
    */
    bigTaskWidget* toBigWidget();

    void updateChildren(bool prepare = true);

protected:
    QPainterPath shape() const override; //< For click detection

    QTransform paintOutline(QPainter *painter);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    /*!
        \brief Get the transform applied when hovering.

        This is one that expands everything outwards from the centre.
    */
    QTransform getExpansionTransform();

    QPainterPath path;
    std::vector<QGraphicsItem*> extras; //< [0]: title
    std::vector<TodoGraphicObject*> todos;

private:
    QString name;
    Window* wind = nullptr;
    bool isHover = false;
    unsigned int lastHei = 0;
};

/*!
    \brief Helper func for making taskWidget objects
*/
TaskWidget* MakeTaskWidget(QString nme, Window* window, std::vector<QString> todos, QGraphicsItem* parent = nullptr);

#endif // TASKSVIEW_H
