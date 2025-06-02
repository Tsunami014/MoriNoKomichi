#ifndef SECTIONS_H
#define SECTIONS_H

#include "widgets/taskwidget.h"
#include "../window.h"

void removeOverlay(Window* wind, std::vector<QWidget*>* wids);

void taskView(Window* wind);
void newOverlay(Window* wind);
void taskOverlay(Window* wind, TaskWidget* task);

#endif // SECTIONS_H
