#include "sectionbgwidget.h"
#include "tools/drawtools.h"

#include <QPainter>

/*! \brief The 4 colours for the 4 sections */
static QColor COLS[4] = {
    {240, 210, 200},
    {215, 245, 190},
    {220, 200, 245},
    {245, 230, 170},
};

SectionBgWidget::SectionBgWidget(uint8_t sect, std::array<QGraphicsItemGroup*, 4>* pgroups, QGraphicsItem* parent) : QGraphicsObject(parent) {
    section = sect;
    groups = pgroups;
}

QRectF SectionBgWidget::boundingRect() const {
    auto group = (*groups)[section];
    return QRectF(group->pos(), group->childrenBoundingRect().size());
}

void SectionBgWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF nsze = boundingRect();
    if (nsze != lastSze) {
        lastSze = nsze;
        makePath();
    }

    painter->setPen(QPen(QColor(200, 180, 150), 4));
    painter->setBrush(QBrush(COLS[section]));
    painter->drawPath(pth);
}

void SectionBgWidget::makePath() {
    if (lastSze.width() == 0 || lastSze.height() == 0) { // Just in case of an empty section
        pth.clear();
        return;
    }
    // Initialisation
    QRandomGenerator gen = QRandomGenerator();
    std::vector<QPoint> edgesps[4];

    uint8_t wh = 1; //< Width or Height - 1 for going horizontal (width), 0 for going vertical (height)

    // Set some vars
    int maxJump = 10; //< The maximum distance a new point can be generated
    int padding = 5+gen.generate()%10; //< The padding between the path and the edge of the bounding box
    int cornerCut = 20; //< How much random offset to start with; can make corners more smooth

    // For each edge
    for (uint8_t i = 0; i < 4; i++) {
        float left;
        // Here we can use lastSze as it's always set before this function is ran
        if (wh) {
            left = lastSze.width()-padding;
        } else {
            left = lastSze.height()-padding;
        }
        left -= gen.generate()%cornerCut;
        // Find offset for the edge
        QPoint off = {static_cast<int>(lastSze.x()), static_cast<int>(lastSze.y())};
        if (i == 1) {
            off.setX(off.x() + lastSze.width() - padding*2);
        }
        if (i == 2) {
            off.setY(off.y() + lastSze.height() - padding*2);
        }
        while (left > padding) {
            left -= (gen.generate() / 5)%maxJump;
            //edgesps[i].push_back({static_cast<int>(left*wh)+off.x(), static_cast<int>(left*(1-wh))+off.y()});
            edgesps[i].push_back(distort({static_cast<int>(left*wh), static_cast<int>(left*(1-wh))}, off, gen, maxJump/4, padding));
        }
        wh = 1-wh;
    }

    std::vector<QPoint> ps;
    // The ordering is a little stuffed up, but this way works;

    // Insert first edge (top horizontal)
    ps.reserve(ps.size() + distance(edgesps[0].begin(), edgesps[0].end()));
    ps.insert(ps.end(), edgesps[0].begin(), edgesps[0].end());
    // Insert fourth edge (left vertical) *in reverse* (as that's how I create it to be easier)
    ps.reserve(ps.size() + distance(edgesps[3].begin(), edgesps[3].end()));
    ps.insert(ps.end(), edgesps[3].rbegin(), edgesps[3].rend());
    // Insert third edge (bottom horizontal) *in reverse* (same reason)
    ps.reserve(ps.size() + distance(edgesps[2].begin(), edgesps[2].end()));
    ps.insert(ps.end(), edgesps[2].rbegin(), edgesps[2].rend());
    // Insert second edge (right vertical)
    ps.reserve(ps.size() + distance(edgesps[1].begin(), edgesps[1].end()));
    ps.insert(ps.end(), edgesps[1].begin(), edgesps[1].end());

    // Lastly, generate the path!
    pth = genPath(ps, gen, true, maxJump/2);
}
