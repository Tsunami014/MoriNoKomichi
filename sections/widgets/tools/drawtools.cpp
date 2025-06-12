#include "drawtools.h"

QRandomGenerator getGen(QString seed) {
    std::hash<QString> h; // Hash the string to get a unique value for that string seed
    return QRandomGenerator(static_cast<uint32_t>(h(seed)));
}

QPoint distort(QPoint p, QPoint offset, QRandomGenerator gen, int distortPad, int padding) {
    return QPoint(
        p.x() + (gen.generate()%(distortPad*2) - distortPad) + padding + offset.x(),
        p.y() + (gen.generate()%(distortPad*2) - distortPad) + padding + offset.y()
    );
}


QPainterPath* roughRect(QRandomGenerator gen, QSize thisSze, int padding, int corner_radius) {
    struct corner1 {
        QPoint p;
        int8_t move_x;
        int8_t move_y;
        int8_t multip;
    };
    corner1 ps1[4] = {
        {{padding, padding}, 0, 1, 1},
        {{padding, thisSze.height()}, 1, 0, -1},
        {{thisSze.width(), thisSze.height()}, 0, -1, 1},
        {{thisSze.width(), padding}, -1, 0, -1}
    };

    struct corner2 {
        QPoint p;
        int8_t move_x;
        int8_t move_y;
    };
    QPoint ps2[8];
    for (int idx = 0; idx < 4; idx++) {
        ps2[idx*2] = distort(ps1[idx].p, {ps1[idx].move_y*corner_radius*ps1[idx].multip, ps1[idx].move_x*corner_radius*ps1[idx].multip}, gen);
        ps2[idx*2+1] = distort(ps1[idx].p, {ps1[idx].move_x*corner_radius, ps1[idx].move_y*corner_radius}, gen);
    }

    QPainterPath* pth = new QPainterPath(ps2[0]);
    bool state = true; // Corner (true) or edge (false)
    uint8_t edge = 0;
    for (int idx = 0; idx < 8; idx++) {
        QPoint next = ps2[(idx+1)%8];
        if (state) {
            QPoint control = distort(ps1[edge++].p, {0, 0}, gen);
            pth->quadTo(control, next);
        } else {
            continuePath(pth, std::vector<QPoint>{next}, gen);
        }
        state = !state;
    }
    return pth;
}

void continuePath(QPainterPath* pth, std::vector<QPoint> ps, QRandomGenerator gen, int mayhem) {
    QPointF curPos = pth->currentPosition();

    // Run through each point, creating a line that's slightly wonky
    for (QPoint& p : ps) {
        switch (gen.generate()%4) {
        case 0: // Line
            pth->lineTo(p);
            break;
        case 1: { // Cubic curve; 2 handles at third intervals slightly offset
            int diffX = (p.x() - curPos.x()) / 3;
            int diffY = (p.y() - curPos.y()) / 3;
            QPoint thirdP1 = QPoint(
                curPos.x() + diffX + gen.generate()%mayhem,
                curPos.y() + diffY + gen.generate()%mayhem
            );
            QPoint thirdP2 = QPoint(
                curPos.x() + diffX*2 + gen.generate()%mayhem,
                curPos.y() + diffY*2 + gen.generate()%mayhem
            );
            pth->cubicTo(thirdP1, thirdP2, p);
            break;
        }
        case 2: { // Cubic curve; 2 handles at the half mark, slightly offset
            QPoint midP = QPoint(
                (p.x()+curPos.x())/2,
                (p.y()+curPos.y())/2
            );
            pth->cubicTo(midP.x()+gen.generate()%mayhem,
                         midP.y()+gen.generate()%mayhem,
                         midP.x()+gen.generate()%mayhem,
                         midP.y()+gen.generate()%mayhem,
                         p.x(), p.y()
            );
            break;
        }
        case 3: { // Quad curve; 1 handle at the half mark slightly offser
            QPoint midP = QPoint(
                (p.x()+curPos.x())/2 + gen.generate()%mayhem,
                (p.y()+curPos.y())/2 + gen.generate()%mayhem
            );
            pth->quadTo(midP, p);
            break;
        }
        }
        curPos = p;
    }
}
QPainterPath genPath(std::vector<QPoint> ps, QRandomGenerator gen, bool closed, int mayhem) {
    // Just 'continue' a new path.
    QPainterPath pth(ps.back());
    if (!closed) {
        ps.pop_back();
    }
    continuePath(&pth, ps, gen, mayhem);
    return pth;
}
