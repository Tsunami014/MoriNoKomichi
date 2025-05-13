#ifndef TASKSVIEW_H
#define TASKSVIEW_H

#include <QFont>
#include <QFontMetrics>
#include <QGraphicsItem>

class TaskWidget : public QGraphicsItem {
public:
    TaskWidget(QString nme);

    const static inline int width = 600;
    const static inline int padding = 20;
    const static inline int paddedWid = width + (padding*2);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QString name;
    static inline QFont textFont = QFont("Segoe Script", 14);
    static inline QFontMetrics measure = QFontMetrics(textFont);
};

#endif // TASKSVIEW_H
