#include "taskloading.h"

#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

std::array<std::vector<TaskWidget*>, 4> readFrom(QTextStream& in, Window* wind) {
    uint8_t sect = 0; // So if something bad happens it won't crash
    QString tskName = "";
    std::vector<QString> todos;

    std::array<std::vector<TaskWidget*>, 4> out;

    auto tryAddTask = [sect, &tskName, &todos, wind, &out]() {
        if (tskName == "") { return; }
        out[sect].push_back(MakeTaskWidget(tskName, wind, todos));
        tskName = "";
        todos.clear();
    };

    QString line = in.readLine();
    while (!line.isNull()) {
        QChar typ = line[0];
        QString rest = line.mid(1).replace("\x01", "\n").replace("\\x01", "\x01");

        switch (typ.unicode()) {
            case 't':
                tryAddTask();
                tskName = rest;
                break;
            case 's':
                todos.push_back(rest);
                break;
            default:
                tryAddTask();
                sect = typ.digitValue();
                break;
        }

        line = in.readLine();
    }

    return out;
}

std::array<std::vector<TaskWidget*>, 4> getSections(Window* wind) {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    QString fullpth = path + "/savedata";
    QFile file(fullpth);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // If the user is new, give them some example tasks to get used to the program first
        std::array<std::vector<TaskWidget*>, 4> sections = {
            std::vector{ // This is needed for no aparent reason
                MakeTaskWidget("Get used to the main view", wind, {"Scroll or shift-scroll", "(Space/middle click)+mouse drag to pan", "Ctrl+scroll to zoom"})
            },
            {
                MakeTaskWidget("Click on tasks to edit them", wind, {"Click on this task!", "Click the black overlay or back button to go back"}),
                MakeTaskWidget("Add a new task", wind, {"Press the plus button to add a new task", "Give it a good title and select a section!"}),
                MakeTaskWidget("Rename a task", wind, {"Press on a task again", "Click on the title", "Change it to whatever you want!", "Click out to exit"}),
                MakeTaskWidget("Delete a task", wind, {"Click on a task", "Click on the title again", "Delete the entire title (backspace)"})
            },
            {
                MakeTaskWidget("Add new subtasks", wind, {"v a task", "Write text in the box to the right", "Press enter to add the subtask!"}),
                MakeTaskWidget("Rename subtasks", wind, {"Click on a task", "Double click on a task name", "Edit it!", "Click out of it again"}),
                MakeTaskWidget("Delete subtasks", wind, {"Click on a task", "Double click the name again", "Delete the entire name (backspace)"})
            },
            {
                MakeTaskWidget("The app autosaves whenever you make a change", wind, {"Close the app", "Come back again to see all your changes in place"}),
                MakeTaskWidget("Have fun!", wind, {"Delete all the premade tasks", "Populate with your own projects", "Celebrate!"})
            }
        };

        return sections;
    }

    QTextStream in(&file);
    return readFrom(in, wind);
}

QString fixStr(QString str) { //< Ensure no task name can conflict with the file data
    return str.replace("\x01", "\\x01").replace("\n", "\x01");
}

void writeTo(QTextStream& out, std::array<std::vector<TaskWidget*>, 4> sects) {
    // Loop through each section and put it on the file
    for (uint8_t idx = 0; idx < 4; idx++) {
        out << idx << "\n";
        // Loop through each task in the section and put it on the file
        for (auto task : sects[idx]) {
            out << "t" << fixStr(task->name) << "\n";
            // Loop through each tub-task in the task and put it on the file
            for (auto sub_t : task->todos) {
                out << "s" << fixStr(sub_t->getname()) << "\n";
            }
        }
    }
}

void saveSections(Window* wind) {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    QString fullpth = path + "/savedata";
    QFile file(fullpth);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed writing to file at " << fullpth << "!";
        return;
    }

    QTextStream out(&file);
    writeTo(out, wind->sections);
    qDebug() << "Wrote to file at " << fullpth << "!";
}
