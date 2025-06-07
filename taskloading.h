#ifndef TASKLOADING_H
#define TASKLOADING_H

#include <array>
#include <vector>

#include "sections/widgets/taskwidget.h"
#include "window.h"

std::array<std::vector<TaskWidget*>, 4> getSections(Window* wind);

#endif // TASKLOADING_H
