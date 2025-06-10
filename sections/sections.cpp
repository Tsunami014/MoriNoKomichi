#include "sections.h"
#include <QPainter>

OverlayWid::OverlayWid(Window* window) : QWidget(window) {
    wind = window;
}
void OverlayWid::mouseReleaseEvent(QMouseEvent *event) {
    backFun(wind);
    event->accept();
}
void OverlayWid::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(50, 10, 10, 125)));
    painter.drawRect(QRect(QPoint(), size()));
    painter.end();
}

/*!
    \brief Remove specified widgets from a window

    (made for overlay views; to just remove tasks in an overlay and leave the main tasks view untouched)
*/
void removeOverlay(Window* wind, std::vector<QWidget*>* wids) {
    if (wids->empty()) {
        return;
    }
    // Go through each widget in the window
    for (int i = wind->wids.size()-1; i >= 0; i--) {
        auto wid = wind->wids[i].wid;
        int start = wids->size()-1;
        // Go through each widget in the list provided
        for (int j = start; j >= 0; j--) {
            if (wids->at(j) == wid) { // If the widget is the same:
                // Ensure that if the widget is at the back, use the much faster pop_back operation; else use the .erase
                if (j == start) {
                    delete wids->back();
                    wids->pop_back();
                } else {
                    delete wids->at(j);
                    wids->erase(wids->begin() + j);
                }
                if (i == wind->wids.size()-1) {
                    wind->wids.pop_back();
                } else {
                    wind->wids.erase(wind->wids.begin() + j);
                }
                break;
            }
        }
        if (wids->empty()) {
            break;
        }
    }
    wids->clear();
    wind->resizeElms();
}

std::vector<QWidget*>* rmWids = new std::vector<QWidget*>; // Starts as an empty list
std::function<void(Window*)> backFun = [](Window* wind){ removeOverlay(wind, rmWids); }; // Run the removeOverlay func (specified in this file only) with the rmWids
