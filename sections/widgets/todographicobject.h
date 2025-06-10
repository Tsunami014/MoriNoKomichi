#ifndef TODOGRAPHICOBJECT_H
#define TODOGRAPHICOBJECT_H

#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QCheckBox>

class TaskWidget; // Forward reference

/*!
    \brief This is the editable label for todo objects
*/
class TodoLabel : public QLineEdit {
    Q_OBJECT
public:
    TodoLabel(const QString& text, bool enabled = true, QWidget* parent = nullptr);
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
    TodoLabel* label;
    QCheckBox* checkbox;
};

#endif // TODOGRAPHICOBJECT_H
