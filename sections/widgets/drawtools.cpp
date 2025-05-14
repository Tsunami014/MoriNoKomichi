#include "drawtools.h"

QRandomGenerator getGen(intptr_t seed) {
    return QRandomGenerator(seed);
}

QPoint distort(QPoint p, QPoint offset, QRandomGenerator gen, int distortPad, int padding) {
    return QPoint(
        p.x() + (gen.generate()%(distortPad*2) - distortPad) + padding + offset.x(),
        p.y() + (gen.generate()%(distortPad*2) - distortPad) + padding + offset.y()
    );
}

float randDec(QRandomGenerator gen) {
    return 1+(gen.generate()%5)/10;
}

void continuePath(QPainterPath* pth, std::vector<QPoint> ps, QRandomGenerator gen, int mayhem) {
    QPointF curPos = pth->currentPosition();

    for (QPoint& p : ps) {
        switch (gen.generate()%4) {
        case 0:
            pth->lineTo(p);
            break;
        case 1: {
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
        case 2: {
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
        case 3: {
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
    QPainterPath pth(ps.back());
    if (!closed) {
        ps.pop_back();
    }
    continuePath(&pth, ps, gen, mayhem);
    return pth;
}
