#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QLineEdit>

class InputWidget : public QLineEdit
{
public:
    InputWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

    QPainterPath* pth = nullptr;
    QSize lastSze = {0, 0};
};

#endif // INPUTWIDGET_H
