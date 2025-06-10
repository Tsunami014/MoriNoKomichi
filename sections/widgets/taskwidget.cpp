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
#include <QGraphicsLayout>


TitleText::TitleText(const QString &text, TaskWidget* parent) : QGraphicsTextItem(text, parent) {
    widparent = parent;
};
bool TitleText::event(QEvent *ev) {
    // Check if it's the right type of event
    bool ret = QGraphicsTextItem::event(ev);
    if (ev->type() == QEvent::KeyPress) {
        // Update next frame
        QTimer::singleShot(0, this, [this](){
            widparent->updateChildren(true, true);
        });
    }
    return ret;
}


TaskWidget::TaskWidget(QString nme, Window* window, std::vector<QString> inptodos, QGraphicsItem* parent, bool editable)
    : QGraphicsObject(parent)
{
    // Set some flags
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    // Set some of the things from the params
    name = nme;
    wind = window;

    // Add a title
    title = new TitleText(nme, this);
    // Try a bunch of good fonts for the title
    title->setFont(getAFont({"Kalam", "Comic Neue", "Segoe Print", "Amatic SC", "DejaVu Sans Mono"}, 18));
    title->show();

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

BigTaskWidget* TaskWidget::toBigWidget() {
    // Create the new widget, ensuring all the appropriate construction functions are called
    BigTaskWidget* newWid = new BigTaskWidget(name, wind, {}, nullptr);
    // Copy all the todos over
    for (auto t : todos) {
        newWid->todos.push_back(new TodoGraphicObject(t->getname(), true, newWid));
        newWid->todos.back()->setChecked(t->getChecked());
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
    // Ensure there's a minimum height to not get everything to turn to mush
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
        title->setTransform(transform, true);
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
        title->setTransform(transform, true);
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

    if (parent != nullptr) { // This would be any BigTaskWidgets
        // Update parent's children so text gets updated!
        QString txt = title->toPlainText();
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
        title->setPlainText(txt);
        parent->name = txt;

        // Create a list of what the todos should be
        std::vector<TODOstate> outtodos;
        for (unsigned int idx = 0; idx < todos.size(); idx++) {
            QString nme = todos[idx]->getname();
            if (nme != "") {
                outtodos.push_back({nme, todos[idx]->getChecked()});
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
            todos[idx]->setChecked(outT.checked);

            if (parent->todos.size() > idx) {
                parent->todos[idx]->setName(outT.nme);
            } else {
                parent->todos.push_back(new TodoGraphicObject(outT.nme, false, parent));
            }
            parent->todos[idx]->setChecked(outT.checked);
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
    title->setTextWidth(-1);
    qreal max = static_cast<qreal>(width - (padding)*2 - 90);
    qreal min = qMin(max, title->boundingRect().width());
    if (min < max) { // If text is smaller than width - padding*2, let it auto-resize - else, stick to the max defined
        title->setTextWidth(-1);
    } else {
        title->setTextWidth(max);
    }

    // Just add the rest of the todos one at a time underneath
    QRectF bbx = title->boundingRect();
    unsigned int y = padding*2;
    title->setPos(QPoint((width-bbx.width())/2 + 10, y));

    y += bbx.height() + padding;
    for (auto t : todos) {
        t->setWidth(width*0.8);
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
    // Make some variables
    QTransform transform;
    int hoverAmnt = 5;
    QRectF BBx = boundingRect();
    QPointF center = BBx.center();

    // Transform so the centre is in the middle, scale out and transform back. So the centre of scaling should be the centre of the graphic.
    transform.translate(center.x(), center.y());
    transform.scale(static_cast<qreal>(hoverAmnt*2) / BBx.width() + 1, static_cast<qreal>(hoverAmnt*2) / BBx.height() + 1);
    transform.translate(-center.x(), -center.y());

    return transform;
}

void TaskWidget::updatePath() {
    QSizeF nsze = boundingRect().size();
    // The x difference; a ratio
    float diffX = nsze.width() / pureSze.width();
    // The y difference; find which end it's closest to and keep it the same distance away
    // This ensures the top and bottom won't become stretched (e.g. if the top was at y=3, then the window was stretched heaps, it would be at y=34, and if the text starts at y=5 it would be bad)
    float origHei = pureSze.height();
    float newHei = nsze.height();

    // Loop over all the points
    for (int idx = 0; idx < path.elementCount(); idx++) {
        QPointF point = purePath.elementAt(idx);
        // Find the correct y
        float fromTop = origHei - point.y();
        int y;
        if (abs(point.y()) < abs(fromTop)) {
            y = point.y();
        } else {
            y = newHei - fromTop;
        }
        // Set the point on the path
        path.setElementPositionAt(idx, point.x()*diffX, y);
    }
}

void TaskWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing); // Make it smooth

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
    // Make some commonly used variables
    QRandomGenerator gen = getGen(name);
    int height = boundingRect().height() - (padding * 2);

    // Make polygon
    int8_t distortPad = 10;
    int8_t mayhem = 5;
    int8_t cornerOffRnd = 30;

    // First, start with taking each corner and randomly applying a 'tear' to it

    // Corners specified like this to make the code into a loop instead of repetition
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
        // Get position of the corner based off of setup above
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
