#include "taskwidget.h"
#include "tools/drawtools.h"
#include "tools/texttools.h"
#include "bigtaskwidget.h"
#include "../sections.h"

#include <QPainter>
#include <QCursor>
#include <QFontMetrics>
#include <QLineEdit>
#include <QCheckBox>
#include <QGraphicsObject>
#include <QGraphicsProxyWidget>

MyLabel::MyLabel(const QString& text, QWidget* parent)
    : QLineEdit(text, parent)
{
    setReadOnly(true);
    setStyleSheet("QLineEdit:read-only {"
                  "    border: none;"
                  "    background: rgb(255, 235, 210); }"
                  "QLineEdit {"
                  "    background: white; }");
    connect(this, &QLineEdit::editingFinished, [this]{
        this->unsetCursor();
        this->setSelection(0,0);
        this->setReadOnly(true);});
}
void MyLabel::mousePressEvent(QMouseEvent *) { emit clicked(); }
void MyLabel::mouseDoubleClickEvent(QMouseEvent *) {
    this->setReadOnly(false);
    this->selectAll();
}

TodoGraphicObject::TodoGraphicObject(QString nme, TaskWidget* parent) : QGraphicsProxyWidget(parent) {
    name = nme;
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout();
    widget->setLayout(layout);
    QCheckBox* checkbox = new QCheckBox();
    checkbox->setStyleSheet("QWidget { background: rgb(255, 235, 210); }");
    MyLabel* label = new MyLabel(nme);
    connect(label, &MyLabel::clicked, checkbox, &QCheckBox::toggle);
    layout->addWidget(checkbox);
    layout->addWidget(label);
    widget->setStyleSheet("QWidget { background: rgb(255, 235, 210); }");
    widget->show();
    setWidget(widget);
}

class myText : public QGraphicsTextItem {
public:
    myText(const QString &text, TaskWidget* parent) : QGraphicsTextItem(text, parent) {
        widparent = parent;
    };
protected:
    bool event(QEvent *ev) override {
        // If you pressed a key, update the sizing and positioning of all the elements
        bool ret = QGraphicsTextItem::event(ev);
        if (ev->type() == QEvent::KeyPress) {
            QTimer::singleShot(0, this, [this](){
                widparent->updateChildren();
            });
        }
        return ret;
    }
private:
    TaskWidget* widparent;
};


TaskWidget::TaskWidget(QString nme, Window* window, std::vector<QString> inptodos, QGraphicsItem* parent)
    : QGraphicsObject(parent)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    name = nme;
    wind = window;

    myText *it = new myText(nme, this);
    it->setFont(getAFont({"Kalam", "Comic Neue", "Segoe Print", "Amatic SC", "DejaVu Sans Mono"}, 18));
    it->show();
    extras.push_back(it);

    for (auto str : inptodos) {
        todos.push_back(new TodoGraphicObject(str, this));
    }
}
TaskWidget* MakeTaskWidget(QString nme, Window* window, std::vector<QString> todos, QGraphicsItem* parent) {
    TaskWidget* tw = new TaskWidget(nme, window, todos, parent);
    tw->updateChildren();
    tw->makePath();
    return tw;
}

bigTaskWidget* TaskWidget::toBigWidget() {
    std::vector<QString> strtodos;
    for (auto t : todos) {
        strtodos.push_back(t->name);
    }
    bigTaskWidget* newWid = new bigTaskWidget(name, wind, strtodos, nullptr);
    newWid->updateChildren();
    newWid->makePath();
    newWid->show();
    newWid->updateChildren();
    return newWid;
}

QRectF TaskWidget::boundingRect() const {
    unsigned int hei = lastHei;
    unsigned int minHei = 170;
    if (hei <= minHei) {
        hei = minHei + 1;
    }
    return QRectF(0, 0, width + (padding*2), hei);
}
QPainterPath TaskWidget::shape() const {
    return path;
}

void TaskWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event)
    if (!isHover) {
        // Set all transforms on children here
        QTransform transform = getExpansionTransform();
        for (auto ex : extras) {
            ex->setTransform(transform, true);
        }
        for (auto t : todos) {
            t->setTransform(transform, true);
        }
    }
    isHover = true;
    setCursor(Qt::PointingHandCursor);

    update(); // force repaint
}
void TaskWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event)
    if (isHover) {
        // Unset all transforms on children here by transforming them the opposite of what they were
        QTransform transform = getExpansionTransform().inverted();
        for (auto ex : extras) {
            ex->setTransform(transform, true);
        }
        for (auto t : todos) {
            t->setTransform(transform, true);
        }
    }
    isHover = false;
    unsetCursor();

    update(); // force repaint
}

void TaskWidget::mousePressEvent(QGraphicsSceneMouseEvent* event) {} // Needs to not be the default or else release won't be called
void TaskWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    isHover = false;
    unsetCursor();
    taskOverlay(wind, this);
}

void TaskWidget::updateChildren(bool prepare) {
    if (prepare) { prepareGeometryChange(); }

    // Position the heading text in the correct spot and with the correct width
    auto *txt = static_cast<myText*>(extras[0]);

    txt->setTextWidth(-1);
    qreal max = static_cast<qreal>(width - (padding)*2 - 90);
    qreal min = qMin(max, txt->boundingRect().width());
    if (min < max) { // If text is smaller than width - padding*2, let it auto-resize - else, stick to the max defined
        txt->setTextWidth(-1);
    } else {
        txt->setTextWidth(max);
    }

    QRectF bbx = txt->boundingRect();
    unsigned int y = padding+30;
    txt->setPos(QPoint((width-bbx.width())/2 + 10, y));

    y += bbx.height() + padding;
    for (auto t : todos) {
        QRectF tBbx = t->boundingRect();
        t->setPos(QPoint((width-tBbx.width())/2, y));
        y += tBbx.height() + padding;
    }
    lastHei = y + padding * 3;

    if (prepare) { update(); }
}

QTransform TaskWidget::getExpansionTransform() {
    QTransform transform;
    int hoverAmnt = 5;
    QRectF BBx = boundingRect();
    QPointF center = BBx.center();

    transform.translate(center.x(), center.y());
    transform.scale(static_cast<qreal>(hoverAmnt*2) / BBx.width() + 1, static_cast<qreal>(hoverAmnt*2) / BBx.height() + 1);
    transform.translate(-center.x(), -center.y());

    return transform;
}

void TaskWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (lastHei == 0) {
        updateChildren();
    }

    painter->setRenderHint(QPainter::Antialiasing);

    // Apply a scaling to everything beyond this point if hovering
    QTransform prevTrans = painter->transform();
    if (isHover) {
        painter->setTransform(getExpansionTransform() * prevTrans);
    }

    // Display polygon
    painter->save();
    painter->setBrush(QBrush(QColor(255, 235, 210)));
    painter->setPen(QPen(Qt::black, 5));
    painter->drawPath(path);
    painter->restore();

    // Reset transform
    painter->setTransform(prevTrans);
}

void TaskWidget::makePath() {
    QRandomGenerator gen = getGen(name);
    int height = boundingRect().height() - (padding * 2);

    // Make polygon
    int8_t distortPad = 10;
    int8_t mayhem = 5;
    int8_t cornerOffRnd = 30;

    struct Corner {
        bool x;
        bool y;
    };

    Corner ps1[4] = {
        Corner{false, false},
        Corner{true, false},
        Corner{true, true},
        Corner{false, true},
    };

    std::vector<QPoint> ps;
    for (int8_t idx = 0; idx < 4; idx++) {
        // Torn/regular corner
        int x;
        if (ps1[idx].x) {
            x = width-distortPad;
        } else {
            x = distortPad;
        }
        int y;
        if (ps1[idx].y) {
            y = height-distortPad;
        } else {
            y = distortPad;
        }
        QPoint p(x, y);

        if (gen.generate()%7 == 0) {
            int8_t xOff;
            int8_t xOffRnd = gen.generate()%cornerOffRnd;
            if (ps1[idx].x) {
                xOff = -xOffRnd;
            } else {
                xOff = xOffRnd;
            }
            int8_t yOff;
            int8_t yOffRnd = gen.generate()%cornerOffRnd;
            if (ps1[idx].y) {
                yOff = -yOffRnd;
            } else {
                yOff = yOffRnd;
            }
            QPoint first = distort(p, QPoint(xOff*randDec(gen), 0), gen, distortPad, padding);
            QPoint mid = distort(p, QPoint(xOff*randDec(gen), yOff*randDec(gen)), gen, distortPad, padding);
            QPoint last = distort(p, QPoint(0, yOff*randDec(gen)), gen, distortPad, padding);
            if (ps1[idx].x ^ ps1[idx].y) {
                ps.push_back(first);
                ps.push_back(mid);
                ps.push_back(last);
            } else {
                ps.push_back(last);
                ps.push_back(mid);
                ps.push_back(first);
            }
        } else {
            ps.push_back(distort(p, {0, 0}, gen, distortPad, padding));
        }

        // Rips
        int8_t XY = ps1[idx].x ^ ps1[idx].y; // 0 for x, 1 for y
        int8_t dir = 1;
        if (ps1[idx].y) {
            dir = -1;
        }
        int8_t opp = -1;
        if (XY^(dir==1)) {
            opp = 1;
        }
        int8_t X = (!XY)*dir;
        int8_t Y = XY*dir;

        int pos = cornerOffRnd*2 + distortPad;
        int max;
        if (XY) {
            max = height - pos;
        } else {
            max = width - pos;
        }
        max -= cornerOffRnd*2;
        int minSpacing = max/10;
        while (true) {
            pos += gen.generate()%max;
            if (pos >= max) {
                break;
            }
            if (idx == 0) {
                if (pos > max/4 && pos < (max/4)*3) {
                    continue;
                }
            }
            if (gen.generate()%2 == 0) {
                ps.push_back(distort(p, {X*pos, Y*pos}, gen, distortPad, padding));
                pos += minSpacing;
            } else {
                int sze = gen.generate()%10+15;
                int halfSze = sze/2;
                ps.push_back(distort(p, {X*pos, Y*pos}, gen, distortPad, padding));
                ps.push_back(distort(p, {X*(pos+halfSze) + XY*opp*sze, Y*(pos+halfSze) + (!XY)*opp*sze}, gen, distortPad, padding));
                ps.push_back(distort(p, {X*(pos+sze), Y*(pos+sze)}, gen, distortPad, padding));
                pos += sze+minSpacing;
            }
        }
    }

    // Make path
    path = genPath(ps, gen, true, mayhem);
}
