#include "taskloading.h"

std::array<std::vector<TaskWidget*>, 4> getSections(Window* wind) {
    // Temporary example sections
    std::array<std::vector<TaskWidget*>, 4> sections = {
        std::vector{ // This is needed for no aparent reason
            MakeTaskWidget("Hello group 1", wind, {"Task 1", "Task 2"}),
            MakeTaskWidget("Reallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallylongname", wind, {"Task 1"}),
            MakeTaskWidget("This name here is quite an extremely super long name for a task, wouldn't you agree?", wind, {"Task 1", "Task 2", "Task 3", "Task 4"}),
            MakeTaskWidget("Goodbye again, group 1", wind, {})
        },
        {MakeTaskWidget("Hello group 2", wind, {}), MakeTaskWidget("Goodbye group 2", wind, {}), MakeTaskWidget("Hello again, group 2", wind, {}), MakeTaskWidget("Goodbye again, group 2", wind, {})},
        {MakeTaskWidget("Hello group 3", wind, {}), MakeTaskWidget("Goodbye group 3", wind, {}), MakeTaskWidget("Hello again, group 3", wind, {}), MakeTaskWidget("Goodbye again, group 3", wind, {})},
        {MakeTaskWidget("Hello group 4", wind, {}), MakeTaskWidget("Goodbye group 4", wind, {}), MakeTaskWidget("Hello again, group 4", wind, {}), MakeTaskWidget("Goodbye again, group 4", wind, {})}
    };

    return sections;
}
