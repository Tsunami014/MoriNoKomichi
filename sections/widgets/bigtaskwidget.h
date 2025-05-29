#ifndef BIGTASKWIDGET_H
#define BIGTASKWIDGET_H

#include "taskwidget.h"

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
