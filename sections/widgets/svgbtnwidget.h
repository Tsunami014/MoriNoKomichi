#ifndef SVGBTNWIDGET_H
#define SVGBTNWIDGET_H

#include <QSvgRenderer>
#include <QMouseEvent>
#include <QPushButton>

class svgBtnWidget: public QPushButton
{
    Q_OBJECT

public:
    svgBtnWidget(QString path, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool hitButton(const QPoint &pos) const override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QSvgRenderer* svg = nullptr;
};

#endif // SVGBTNWIDGET_H
