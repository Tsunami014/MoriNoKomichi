#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QLineEdit>

class InputWidget : public QLineEdit
{
public:
    InputWidget(QWidget *parent = nullptr);

protected:
    /*! \brief Draw the box around the lineedit differently */
    void paintEvent(QPaintEvent *event) override;

    /*! \brief The path cache */
    QPainterPath* pth = nullptr;
    /*! \brief The size the current path was created for */
    QSize lastSze = {0, 0};
};

#endif // INPUTWIDGET_H
