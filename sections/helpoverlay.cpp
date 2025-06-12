#include "sections.h"
#include "widgets/svgbtnwidget.h"
#include "widgets/bigtaskwidget.h"
#include "../taskloading.h"

#include <QDesktopServices>
#include <QTextDocument>
#include <QTextLayout>
#include <QTextBlock>

/*! \brief The help screen is just another task widget with predefined help text that's not editable and slightly modified. Here's the widget. */
static TaskWidget* helpTsk = nullptr;
static QString htmlHelpTxt = QString(
R"(<table width="100%" cellspacing="0"><tr><td align="center">)" // Table hack to get centring

    "<h1>Help</h1>"
    "<h2>How to use the program</h2>"
    "<h3>Tasks</h3><ul>"
        "<li>Create a task by pressing the plus button in the bottom right corner</li>"
        "<li>View a task by clicking on it (clicking the back arrow or clicking the overlay will bring you back)</li>"
        "<li>Rename a task by clicking on its title in the expanded view</li>"
        "<li>Delete a task by clicking on the delete button in the expanded view</li>"
    "</ul><h3>Subtasks (or todos)</h3><ul>"
        "<li>Create a subtask by typing in the box in the expanded task view and pressing 'enter'</li>"
        "<li>Check/uncheck the subtask by clicking on the checkbox in the expanded task view</li>"
        "<li>Rename a subtask by double-clicking on its title in the expanded task view (BE CAREFUL; SEE BELOW)</li>"
        "<li>Delete a subtask by double clicking to edit the name in the expanded task view and deleting the entire name</li>"
    "</ul><h2>Task storage location</h2>"
        "If you want to use a backup tool or import/export your tasks, all you need to look at is the task storage file.<br>"
        "(For you) it is located at <a href=\"hi\">$1</a>."

"</td></tr></table>"
).replace("$1", getPath()); // Show path

void helpOverlay(Window* wind) {
    if (helpTsk == nullptr) {
        helpTsk = new TaskWidget("", wind, {});
    }

    // Make the overlay
    OverlayWid *overlay = new OverlayWid(wind);
    overlay->show();

    // Make the graphics scene with a generated big task widget
    QGraphicsScene* scene = new QGraphicsScene();
    BigTaskWidget* bigW = helpTsk->toBigWidget();
    bigW->title->setTextInteractionFlags(Qt::TextBrowserInteraction); // Become not editable again, but still selectable
    bigW->title->setHtml(htmlHelpTxt); // Set text via html
    // Copy the title again and set the font size this way (again)
    QFont newFont = bigW->title->font();
    newFont.setPointSize(18);
    bigW->title->setFont(newFont);
    // On click open the file explorer at this location
    QObject::connect(bigW->title, &QGraphicsTextItem::linkActivated, [](){
        QDesktopServices::openUrl(QUrl("file:///" + getPath(false)));
    });
    // Add the new big widget to the scene
    scene->addItem(bigW);
    NewGraphicsView *view = new NewGraphicsView(scene, bigW, wind);
    view->show();

    // Make the back btn
    SvgBtnWidget *btn = new SvgBtnWidget(":/assets/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, [wind](){backFun(wind);});
    btn->show();

    // Add all the widgets to the rmWids and wind->wids lists
    rmWids->push_back(overlay);
    rmWids->push_back(view);
    rmWids->push_back(btn);
    wind->wids.push_back(Widget{overlay, QPoint(0, 0), QSize(100, 100)});
    wind->wids.push_back(Widget{view, QPoint(50, 0), QSize(80, 100)});
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});

    wind->resizeElms(); // Update all the positionings!
}
