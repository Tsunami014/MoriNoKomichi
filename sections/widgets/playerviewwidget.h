#ifndef PLAYERVIEWWIDGET_H
#define PLAYERVIEWWIDGET_H

#include <QWidget>
#include <QSvgRenderer>

class playerViewWidget : public QWidget
{
    Q_OBJECT

public:
    playerViewWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QSvgRenderer* rend = nullptr;
};

#endif // PLAYERVIEWWIDGET_H
