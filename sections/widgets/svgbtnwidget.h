#ifndef SVGBTNWIDGET_H
#define SVGBTNWIDGET_H

#include <QSvgRenderer>
#include <QMouseEvent>
#include <QPushButton>

/*!
    \brief An svg widget that acts like a button

    You can connect to it like a button and it has a neat hover animation.
*/
class SvgBtnWidget: public QPushButton
{
    Q_OBJECT

public:
    SvgBtnWidget(QString path, QWidget *parent = nullptr, int expandAmount = 5);

protected:
    void paintEvent(QPaintEvent *event) override;
    /*! \brief Check if the pixel of the image under your cursor is transparent */
    bool hitButton(const QPoint &pos) const override;
    /*! \brief Have an expand effect on hover */
    void mouseMoveEvent(QMouseEvent *event) override;
    /*! \brief Ensure the hover effect is removed once the mouse leaves */
    void leaveEvent(QEvent *event) override;

private:
    // Some internals
    QSvgRenderer* svg = nullptr;
    int expandAmnt;
    bool touching = false;
};

#endif // SVGBTNWIDGET_H
