#ifndef BIGTASKWIDGET_H
#define BIGTASKWIDGET_H

#include "taskwidget.h"

/*!
    \brief A TaskWidget where the elements are editable and it's updated constantly to the size of the screen
*/
class BigTaskWidget : public TaskWidget {
public:
    BigTaskWidget(QString nme, Window* window, std::vector<QString> todos, QGraphicsItem* parent = nullptr);
    /*! \brief Update the width of the task widget (from a window resize); in turn updating all the widgets and parent too */
    void updateWidth(int newWid);
};

#endif // BIGTASKWIDGET_H
