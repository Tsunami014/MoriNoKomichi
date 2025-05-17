#ifndef SVGBTNWIDGET_H
#define SVGBTNWIDGET_H

#include <QSvgRenderer>
#include <QMouseEvent>
#include <QPushButton>

class svgBtnWidget: public QPushButton
{
    Q_OBJECT

public:
    svgBtnWidget(QString path, QWidget *parent = nullptr, int expandAmount = 5);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool hitButton(const QPoint &pos) const override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QSvgRenderer* svg = nullptr;
    int expandAmnt;
    bool touching = false;
};

#endif // SVGBTNWIDGET_H
