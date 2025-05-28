#include "bigtaskwidget.h"
#include "tools/texttools.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>

#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QtMath>
#include <QDebug>

class myText : public QGraphicsTextItem {
public:
    myText(const QString &text, bigTaskWidget* parent) : QGraphicsTextItem(text, parent) {
        widparent = parent;
    };
protected:
    bool event(QEvent *ev) override {
        // If you pressed a key, update the sizing and positioning of all the elements
        bool ret = QGraphicsTextItem::event(ev);
        if (ev->type() == QEvent::KeyPress) {
            QTimer::singleShot(0, this, [this](){
                widparent->updatePath();
            });
        }
        return ret;
    }
private:
    bigTaskWidget* widparent;
};

bigTaskWidget::bigTaskWidget(QString nme, Window* window, QGraphicsItem *parent)
    :TaskWidget(nme, window, parent)
{
    setAcceptHoverEvents(false);
    setAcceptedMouseButtons(Qt::NoButton);

    myText *it = new myText(nme, this);
    it->setFont(getAFont({"Kalam", "Comic Neue", "Segoe Print", "Amatic SC", "DejaVu Sans Mono"}, 24));
    it->setTextInteractionFlags(Qt::TextEditorInteraction);
    it->show();
    extras.push_back(it);
}

void bigTaskWidget::makePath() {
    TaskWidget::makePath();
    purePath = path;
    pureSze = boundingRect().size();
}

void bigTaskWidget::updatePath(int newWid) {
    prepareGeometryChange();
    width = newWid - (padding * 2);

    QSizeF nsze = boundingRect().size();
    float diffX = nsze.width() / pureSze.width();
    float diffY = nsze.height() / pureSze.height();
    for (int idx = 0; idx < path.elementCount(); idx++) {
        QPointF point = purePath.elementAt(idx);
        path.setElementPositionAt(idx, point.x()*diffX, point.y()*diffY);
    }

    updatePath(false);

    update();
}
void bigTaskWidget::updatePath(bool prepare) {
    if (prepare) { prepareGeometryChange(); }

    // Position the heading text in the correct spot and with the correct width
    auto *txt = static_cast<myText*>(extras[0]);

    qreal max = static_cast<qreal>(width - padding*2);
    qreal min = qMin(max, txt->boundingRect().width());
    if (min < max) { // If text is smaller than width - padding*2, let it auto-resize - else, stick to the max defined
        txt->setTextWidth(-1);
    } else {
        txt->setTextWidth(min);
    }

    QRectF bbx = txt->boundingRect();
    txt->setPos(QPoint((width-bbx.width())/2, padding+20));

    if (prepare) { update(); }
}

void bigTaskWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QTransform prevTrans = paintOutline(painter);

    // Reset transform
    painter->setTransform(prevTrans);
}
