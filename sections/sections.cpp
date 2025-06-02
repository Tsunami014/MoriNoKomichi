#include "sections.h"

void removeOverlay(Window* wind, std::vector<QWidget*>* wids) {
    if (wids->empty()) {
        delete wids;
        return;
    }
    for (int i = wind->wids.size()-1; i >= 0; i--) {
        auto wid = wind->wids[i].wid;
        int start = wids->size()-1;
        for (int j = start; j >= 0; j--) {
            if (wids->at(j) == wid) {
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
    delete wids;
    wind->resizeElms();
}
