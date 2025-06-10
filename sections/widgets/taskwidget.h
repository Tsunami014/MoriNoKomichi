#ifndef TASKSVIEW_H
#define TASKSVIEW_H

#include "../../window.h"

#include <QFont>
#include <QGraphicsObject>
#include <QMouseEvent>
#include <QGraphicsTextItem>
#include <qgraphicsproxywidget.h>
#include <qlineedit.h>
#include <QCheckBox>

class BigTaskWidget; // Forward reference
class TaskWidget; // Forward reference


/*!
    \brief This is the editable label for todo objects
*/
class MyLabel : public QLineEdit {
    Q_OBJECT
public:
    MyLabel(const QString& text, bool enabled = true, QWidget* parent = nullptr);
protected:
    /*! \brief When finished editing, become read-only again */
    void focusOutEvent(QFocusEvent *event);
    /*! \brief Starts editing the text (used when clicked) */
    void mouseDoubleClickEvent(QMouseEvent *);
private:
    /*! \brief Whether the label is able to be edited */
    bool enabled;
};


/*!
    \brief This is the todo object, containing the name and QWidgets that compose a todo

    It turns the QWidgets into QGraphicsWidgets through its parent QGraphicsProxyWidget.
*/
class TodoGraphicObject : public QGraphicsProxyWidget {
    Q_OBJECT
public:
    TodoGraphicObject(QString nme, bool editable, TaskWidget* parent);

    // Getters and setters for the name as it is in the label, not a variable
    QString getname();
    void setName(QString newName);
    // Same with for the checkbox state
    bool getChecked();
    void setChecked(bool checked);

    /*! \brief Set the width of this todo properly */
    void setWidth(unsigned int width);

public slots:
    /*!
        \brief Get the parent to update children
        Parameters do nothing, but are this way to handle the LABel's signal params.
    */
    void updateParentlab(QString str);
    /*!
        \brief Get the parent to update children
        Parameters do nothing, but are this way to handle the CHecKbox's signal params.
    */
    void updateParentchk(int state);

private:
    bool editable;
    TaskWidget* parent;

    // The internal QWidgets making up this object
    MyLabel* label;
    QCheckBox* checkbox;
};


/*!
    \brief The title for taskwidgets
*/
class MyText : public QGraphicsTextItem {
public:
    MyText(const QString &text, TaskWidget* parent);
protected:
    /*! \brief If you pressed a key, update the sizing and positioning of all the elements */
    bool event(QEvent *ev) override;
private:
    TaskWidget* widparent;
};


/*!
    \brief A QGraphicsObject which is a task; a title with a bunch of todos and a fancy border, that has a fancy hover animation and on click launches a taskOverlay to display the task in detail.
*/
class TaskWidget : public QGraphicsObject {
    Q_OBJECT
public:
    TaskWidget(QString nme, Window* window, std::vector<QString> todos, QGraphicsItem* parent = nullptr, bool editable = false);
    /*! \brief Makes the path of the border */
    void makePath();

    /*! \brief The width of the element (only changes if it's a BigTaskWidget) */
    int width = 600;
    /*! \brief The maximum the border will protrude inward (excluding tears & such) */
    const static inline int padding = 20;

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /*!
        \brief Convert this widget to a BigTaskWidget

        This is for displaying in the taskOverlay
    */
    BigTaskWidget* toBigWidget();

    std::vector<TodoGraphicObject*> todos;
    QString name;

    void updateChildren(bool prepare = true, bool updateAll = false);

protected:
    /*! \brief Only handle mouse events if collides with path */
    QPainterPath shape() const override;

    // Handle hover effects and clicks
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    /*!
        \brief Get the transform applied when hovering.

        This is one that expands everything outwards from the centre.
    */
    QTransform getExpansionTransform();

    /*! \brief The path of the border */
    QPainterPath path;
    /*!
        \brief Set the new path to the old path but with the position differing

        DOES NOT COPY THE PATH; just tweaks the points' positions.
    */
    void updatePath();

    /*! \brief The title of the task */
    MyText* title = nullptr;

    TaskWidget* parent = nullptr;

    // Keeps track of the path as it was when first created to handle updates to the size
    QSizeF pureSze;
    QPainterPath purePath;

private:
    Window* wind = nullptr;
    /*! \brief Whether the element is being hovered over or not */
    bool isHover = false;
    /*!
        \brief The last height of the object

        This is the culmnation of all the heights of the elements that make up the todo; meaning it's perfect for the real height of the graphic
    */
    unsigned int lastHei = 0;
};

/*!
    \brief Helper func for making taskWidget objects

    Runs functions that need to be ran *after* initialisation, but are crucial for the class to work
*/
TaskWidget* MakeTaskWidget(QString nme, Window* window, std::vector<QString> todos, QGraphicsItem* parent = nullptr);


#endif // TASKSVIEW_H
