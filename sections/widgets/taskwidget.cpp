#include "taskwidget.h"
#include "tools/drawtools.h"
#include "tools/texttools.h"
#include "bigtaskwidget.h"
#include "../sections.h"

#include <QPainter>
#include <QCursor>
#include <QFontMetrics>
#include <QLineEdit>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QGraphicsProxyWidget>

// See .h file for more docstrings

MyLabel::MyLabel(const QString& text, bool en, QWidget* parent)
    : QLineEdit(text, parent)
{
    setReadOnly(true);
    enabled = en;
    setStyleSheet("QLineEdit:read-only {"
                  "    border: none;"
                  "    background: rgb(255, 235, 210); }"
                  "QLineEdit {"
                  "    background: white; }");

    if (en) {
        setCursor(Qt::IBeamCursor);
    }
}
// When finished editing, become read-only again
void MyLabel::focusOutEvent(QFocusEvent *event) {
    if (!isReadOnly()) {
        this->unsetCursor();
        this->setSelection(0,0);
        this->setReadOnly(true);
    }
    QLineEdit::focusOutEvent(event);
}
void MyLabel::mouseDoubleClickEvent(QMouseEvent *) {
    // Become editable if double click
    if (enabled) {
        this->setReadOnly(false);
        this->selectAll();
    }
}

TodoGraphicObject::TodoGraphicObject(QString nme, bool iseditable, TaskWidget* taskparent) : QGraphicsProxyWidget(taskparent) {
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout();
    widget->setLayout(layout);

    editable = iseditable;
    parent = taskparent;

    // Add a checkbox and label
    checkbox = new QCheckBox();
    checkbox->setStyleSheet("QWidget { background: rgb(255, 235, 210); }");
    checkbox->setEnabled(iseditable);
    if (iseditable) {
        checkbox->setCursor(Qt::PointingHandCursor);
    }
    label = new MyLabel(nme, iseditable);

    if (iseditable) {
        // Ensure that when they get updated, so to does the parent
        QObject::connect(label, &QLineEdit::textChanged,
                         this,  &TodoGraphicObject::updateParentlab);
        QObject::connect(checkbox, &QCheckBox::clicked,
                         this,     &TodoGraphicObject::updateParentchk);
    }

    // Add the things to the layout and widget
    layout->addWidget(checkbox);
    layout->addWidget(label);
    widget->setStyleSheet("QWidget { background: rgb(255, 235, 210); }");
    widget->show();
    if (!iseditable) {
        widget->setCursor(Qt::PointingHandCursor);
        label->setCursor(Qt::PointingHandCursor);
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        checkbox->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    }
    setWidget(widget);
}

void TodoGraphicObject::updateParentlab(QString str) {
    Q_UNUSED(str);
    parent->updateChildren(true, true);
}
void TodoGraphicObject::updateParentchk(int state) {
    Q_UNUSED(state);
    parent->updateChildren();
}

QString TodoGraphicObject::getname() {
    return label->text();
}
void TodoGraphicObject::setName(QString newName) {
    label->setText(newName);
}

/*!
    \brief The title for taskwidgets
*/
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
                widparent->updateChildren(true, true);
            });
        }
        return ret;
    }
private:
    TaskWidget* widparent;
};


TaskWidget::TaskWidget(QString nme, Window* window, std::vector<QString> inptodos, QGraphicsItem* parent, bool editable)
    : QGraphicsObject(parent)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    name = nme;
    wind = window;

    // Add a title
    myText *it = new myText(nme, this);
    // Try a bunch of good fonts for the title
    it->setFont(getAFont({"Kalam", "Comic Neue", "Segoe Print", "Amatic SC", "DejaVu Sans Mono"}, 18));
    it->show();
    extras.push_back(it);

    // Start us off with the specified todos
    for (auto str : inptodos) {
        todos.push_back(new TodoGraphicObject(str, editable, this));
    }
}
TaskWidget* MakeTaskWidget(QString nme, Window* window, std::vector<QString> todos, QGraphicsItem* parent) {
    TaskWidget* tw = new TaskWidget(nme, window, todos, parent, false);
    tw->updateChildren();
    tw->makePath();
    return tw;
}

bigTaskWidget* TaskWidget::toBigWidget() {
    // Create the new widget, ensuring all the appropriate construction functions are called
    bigTaskWidget* newWid = new bigTaskWidget(name, wind, {}, nullptr);
    // Copy all the todos over
    for (auto t : todos) {
        newWid->todos.push_back(new TodoGraphicObject(t->getname(), true, newWid));
        newWid->todos.back()->checkbox->setChecked(t->checkbox->isChecked());
    }
    // Run the rest of the setup
    newWid->parent = this;
    newWid->updateChildren();
    newWid->makePath();
    newWid->show();
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

struct TODOstate {
    QString nme;
    bool checked;
};

void TaskWidget::updateChildren(bool prepare, bool updateAll) {
    if (prepare) { prepareGeometryChange(); }

    if (parent != nullptr) { // This would be any bigTaskWidgets
        // Update parent's children so text gets updated!
        QString txt = static_cast<myText*>(extras[0])->toPlainText();
        if (txt == "") {
            // Delete parent from sections
            for (auto& s : wind->sections) {
                auto result = std::find(s.begin(), s.end(), parent);
                if (result != std::end(s)) {
                    s.erase(result);
                    break;
                }
            }
            // Remove overlay
            backFun(wind);
            // Delete, update and return
            delete parent;
            updateTaskPoss(wind);
            delete this;
            return;
        }
        static_cast<myText*>(parent->extras[0])->setPlainText(txt);
        parent->name = txt;

        // Create a list of what the todos should be
        std::vector<TODOstate> outtodos;
        for (unsigned int idx = 0; idx < todos.size(); idx++) {
            QString nme = todos[idx]->getname();
            if (nme != "") {
                outtodos.push_back({nme, todos[idx]->checkbox->isChecked()});
            }
        }
        // Update existing todos or add new ones if not enough to match the outtodos list
        unsigned int todoslen = outtodos.size();
        for (unsigned int idx = 0; idx < todoslen; idx++) {
            auto outT = outtodos[idx];
            if (todos.size() > idx) {
                todos[idx]->setName(outT.nme);
            } else {
                todos.push_back(new TodoGraphicObject(outT.nme, false, parent));
            }
            todos[idx]->checkbox->setChecked(outT.checked);

            if (parent->todos.size() > idx) {
                parent->todos[idx]->setName(outT.nme);
            } else {
                parent->todos.push_back(new TodoGraphicObject(outT.nme, false, parent));
            }
            parent->todos[idx]->checkbox->setChecked(outT.checked);
        }
        // Remove extra todos
        while (todos.size() > todoslen) {
            todos.back()->setParent(nullptr);
            todos.back()->deleteLater();
            todos.pop_back();
        }
        while (parent->todos.size() > todoslen) {
            parent->todos.back()->setParent(nullptr);
            parent->todos.back()->deleteLater();
            parent->todos.pop_back();
        }

        parent->updateChildren();
        parent->updatePath(); // Fix height
    }

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

    // Just add the rest of the todos one at a time underneath
    QRectF bbx = txt->boundingRect();
    unsigned int y = padding*2;
    txt->setPos(QPoint((width-bbx.width())/2 + 10, y));

    y += bbx.height() + padding;
    for (auto t : todos) {
        QRectF tBbx = t->boundingRect();
        t->setPos(QPoint((width-tBbx.width())/2, y));
        y += tBbx.height() + padding;
    }
    lastHei = y + padding * 3;
    updatePath(); // Fix height

    if (prepare) { update(); }

    if (updateAll) { updateTaskPoss(wind); }
}

QTransform TaskWidget::getExpansionTransform() {
    QTransform transform;
    int hoverAmnt = 5;
    QRectF BBx = boundingRect();
    QPointF center = BBx.center();

    // Transform so the centre is in the middle, scale out and transform back. So it should be scaling where the centre of scaling is the centre.
    transform.translate(center.x(), center.y());
    transform.scale(static_cast<qreal>(hoverAmnt*2) / BBx.width() + 1, static_cast<qreal>(hoverAmnt*2) / BBx.height() + 1);
    transform.translate(-center.x(), -center.y());

    return transform;
}

void TaskWidget::updatePath() {
    // Set the new path to the old path but with the position differing by a good ratio
    QSizeF nsze = boundingRect().size();
    float diffX = nsze.width() / pureSze.width();
    float origHei = pureSze.height();
    float newHei = nsze.height();
    for (int idx = 0; idx < path.elementCount(); idx++) {
        QPointF point = purePath.elementAt(idx);
        float fromTop = origHei - point.y();
        int y;
        if (abs(point.y()) < abs(fromTop)) {
            y = point.y();
        } else {
            y = newHei - fromTop;
        }
        path.setElementPositionAt(idx, point.x()*diffX, y);
    }
}

void TaskWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    // Apply a scaling to everything beyond this point if hovering
    QTransform prevTrans = painter->transform();
    if (isHover) {
        painter->setTransform(getExpansionTransform() * prevTrans);
    }

    // Display fancy border
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

    // First, start with taking each corner and randomly applying a 'tear' to it
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
        // Get position of the corner
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
            // Torn corner; find 3 points; one on current line, one poking inwards, and one on the next line
            /*
               +--
              ++
              |
            */
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
            // Ensure the points go the right way around depending on which corner this is
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
            // Regular corner; just add the point
            ps.push_back(distort(p, {0, 0}, gen, distortPad, padding));
        }

        // Edges; add tears and kinks
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
        // Continuously increase by a large amount, and while still within the bounds add a tear or bend
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
            if (gen.generate()%2 == 0) { // Bend; only distort the position and keep going (kink in the edge)
                ps.push_back(distort(p, {X*pos, Y*pos}, gen, distortPad, padding));
                pos += minSpacing;
            } else { // Add a rip; a v-shape dent facing inwards
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

    // Save the pure path
    purePath = path;
    pureSze = boundingRect().size();
}
