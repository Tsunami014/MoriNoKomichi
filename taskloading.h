#ifndef TASKLOADING_H
#define TASKLOADING_H

#include <array>
#include <vector>

#include "sections/widgets/taskwidget.h"
#include "window.h"

/*! \brief Get the save file location. This is located in the app data, OS specific */
QString getPath(bool filename = true);

/*!
    \brief Get the sections and tasks

    This function reads from the file if exists; and if not, returns some default onboarding tasks.
*/
std::array<std::vector<TaskWidget*>, 4> getSections(Window* wind);
/*!
    \brief Save the tasks to the file
*/
void saveSections(Window* wind);

/*!
    \brief Delete all contents of file
*/
void writeBlank();

#endif // TASKLOADING_H
