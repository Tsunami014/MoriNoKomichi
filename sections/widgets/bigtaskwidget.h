#ifndef BIGTASKWIDGET_H
#define BIGTASKWIDGET_H

#include "taskwidget.h"

/*!
    \brief A TaskWidget where the elements are editable and it's updated constantly to the size of the screen
*/
class bigTaskWidget : public TaskWidget {
public:
    bigTaskWidget(QString nme, Window* window, std::vector<QString> todos, QGraphicsItem* parent = nullptr);
    void makePath() override;
    void updatePath(int newWid);

private:
    QSizeF pureSze;
    QPainterPath purePath;
};

#endif // BIGTASKWIDGET_H
