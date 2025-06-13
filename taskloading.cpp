#include "taskloading.h"

#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

QString getPath(bool filename) {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    if (!filename) {
        return path;
    }
    return path + "/savedata";
}

/*! \brief Read from a text stream of a file, decoding the info contained within */
std::array<std::vector<TaskWidget*>, 4> readFrom(QTextStream& in, Window* wind) {
    // Initialisation
    uint8_t sect = 0;
    QString tskName = "";
    int16_t tskPriority = -1;
    std::vector<QString> todos;
    std::array<std::vector<TaskWidget*>, 4> out;

    // Lambda to add tasks; if a task was specified, add it and clear task cache
    auto tryAddTask = [&sect, &tskName, &todos, wind, &out, &tskPriority]() {
        if (tskName == "") { return; }
        out[sect].push_back(MakeTaskWidget(tskName, wind, todos, tskPriority));
        tskName = "";
        todos.clear();
    };

    // Read lines one at a time until out of lines
    QString line = in.readLine();
    while (!line.isNull()) {
        QChar typ = line[0]; // First character in line determines it's purpose
        QString rest = line.mid(1).replace("\x01", "\n").replace("\\x01", "\x01"); // Decode line (fix newlines)

        switch (typ.unicode()) {
            case 't': // Task
                tryAddTask();
                tskName = rest;
                // Next line is priority
                line = in.readLine();
                if (line.isNull()) {
                    qCritical() << "Expected priority on line after task, found EOF! Using priority -1.";
                    tskPriority = -1;
                } else {
                    tskPriority = line.toInt();
                }
                break;
            case 's': // Sub-task
                todos.push_back(rest);
                break;
            default: // A section number
                tryAddTask();
                sect = typ.digitValue();
                if (sect > 3) { // Just in case
                    qCritical() << "Specified section " << sect << " is too big! Expected 0-3! Adding to section 0 instead.";
                    sect = 0;
                }
                break;
        }

        line = in.readLine();
    }

    tryAddTask(); // For that last task

    return out;
}

std::array<std::vector<TaskWidget*>, 4> getSections(Window* wind) {
    QString fullpth = getPath();
    QFile file(fullpth);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // If the user is new (no file exists), give them some example tasks to get used to the program first
        std::array<std::vector<TaskWidget*>, 4> sections = {
            std::vector{ // This is needed for no aparent reason
                MakeTaskWidget("Get used to the main view", wind, {"Scroll or shift-scroll", "Space+mouse click and drag to pan", "Middle click and drag to pan", "Ctrl+scroll to zoom"}),
                MakeTaskWidget("Check out the tutorial", wind, {"Do the tasks in the green section", "Do the tasks in the blue section", "Do the tasks in the yellow section"})
            },
            {
                MakeTaskWidget("Click on tasks to edit them", wind, {"Click on this task!", "Click the black overlay or back button to go back"}),
                MakeTaskWidget("Add a new task", wind, {"Press the plus button (bottom right) to add a new task", "Give it a good title and select a section!"}),
                MakeTaskWidget("Rename a task", wind, {"Click on a task", "Click on the title", "Change it to whatever you want!"}),
                MakeTaskWidget("Change task priority", wind, {"Click on a task", "Write a number in the box to the right labelled 'priority'", "Change it to different values for different tasks to see what happens"}),
                MakeTaskWidget("Move a task", wind, {"Click on a task", "Click on a different section in the bottom left corner", "Try with different tasks with different sections!"}),
                MakeTaskWidget("Delete a task", wind, {"Click on a task", "Click on the delete button"})
            },
            {
                MakeTaskWidget("Add new subtasks", wind, {"Click on a task", "Write text in the box labelled 'New subtask' (to the right)", "Press enter to add the subtask!", "Click anywhere to stop editing"}),
                MakeTaskWidget("Rename subtasks", wind, {"Click on a task", "Double click on a sub-task name", "Change it to something else!", "Click anywhere to stop editing"}),
                MakeTaskWidget("Toggle subtasks", wind, {"Click on a task", "Click on a checkbox to toggle it!", "Do this multiple times to test"}),
                MakeTaskWidget("Delete subtasks", wind, {"Click on a task", "Double click the name again", "Delete the entire name (backspace)"})
            },
            {
                MakeTaskWidget("The app autosaves whenever you make a change", wind, {"Close the app", "Come back again to see all your changes in place"}),
                MakeTaskWidget("Look at the help menu", wind, {"Press the help button (top right)"}),
                MakeTaskWidget("Have fun!", wind, {"Delete all the premade tasks (can use help menu)", "Populate with your own projects", "Have a great time! Made with <3 by Tsunami014"})
            }
        };

        return sections;
    }

    // Else read from the file as normal
    QTextStream in(&file);
    return readFrom(in, wind);
}

/*! \brief Ensure no task name can conflict with the file data */
QString fixStr(QString str) {
    return str.replace("\x01", "\\x01").replace("\n", "\x01");
}

/*! \brief Write task data to a file stream */
void writeTo(QTextStream& out, std::array<std::vector<TaskWidget*>, 4> sects) {
    // Loop through each section and put it on the file
    for (uint8_t idx = 0; idx < 4; idx++) {
        out << idx << "\n";
        // Loop through each task in the section and put it on the file
        for (auto task : sects[idx]) {
            out << "t" << fixStr(task->title->toPlainText()) << "\n";
            out << QString::number(task->priority) << "\n"; // Add priority on next line
            // Loop through each tub-task in the task and put it on the file
            for (auto subT : task->todos) {
                out << "s" << fixStr(subT->getname()) << "\n";
            }
        }
    }
}

void saveSections(Window* wind) {
    QString fullpth = getPath();
    QFile file(fullpth);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { // This should never fail (as it's writing to a new file if not exists), but we need to check every possibility
        qWarning() << "Failed writing to file at " << fullpth << "!";
        return;
    }

    // Write to file
    QTextStream out(&file);
    writeTo(out, wind->sections);
    qDebug() << "Wrote to file at " << fullpth << "!";
}

void writeBlank() {
    QString fullpth = getPath();
    QFile file(fullpth);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { // This should never fail (as it's writing to a new file if not exists), but we need to check every possibility
        qWarning() << "Failed opening file to delete contents at " << fullpth << "!";
        return;
    }
    qDebug() << "Deleted file contents at " << fullpth << "!";
}
