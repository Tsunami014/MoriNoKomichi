#ifndef BIGTASKWIDGET_H
#define BIGTASKWIDGET_H

#include <vector>
#include "taskwidget.h"

class bigTaskWidget : public TaskWidget {
public:
    bigTaskWidget(QString nme, Window* window, QGraphicsItem* parent = nullptr);
    void makePath() override;
    void updatePath(bool prepare = true);
    void updatePath(int newWid);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QSizeF pureSze;
    QPainterPath purePath;
    std::vector<QGraphicsItem*> extras;
};

#endif // BIGTASKWIDGET_H
