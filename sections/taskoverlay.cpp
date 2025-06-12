#include "widgets/taskwidget.h"
#include "widgets/bigtaskwidget.h"
#include "widgets/svgbtnwidget.h"
#include "widgets/graphicsviewcanvas.h"
#include "widgets/inputwidget.h"
#include "sections.h"
#include "../window.h"

#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QApplication>
#include <QTextCursor>

/*
We create our own graphics view for multiple reasons:
    1. To have a transparent background; where on click, it goes back to the previous screen
    2. Because the space key has issues
    3. So resizing the window updates the big task widget display
    4. So resizing of the big task widget updates the display
    5. To remove horizontal scrolling and zoom
*/
class NewGraphicsView : public GraphicsViewCanvas {
public:
    NewGraphicsView(QGraphicsScene* scene, BigTaskWidget* wid, Window* window)
    : GraphicsViewCanvas(scene, window) {
        // Some vars to set that make this class special
        wind = window;
        bigW = wid;
        setStyleSheet("background: transparent");
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QObject::connect(bigW, &TaskWidget::updating, this, &NewGraphicsView::updateRect); // Ensure on big task widget update it updates the bbx of this view
    }

    /*! \brief Updates the scene rect based off of the childrens' bounding boxes */
    void updateRect() {
        scene()->setSceneRect(scene()->itemsBoundingRect());
    }

protected:
    /*! \brief If click on bg, run the click func (goes back to prev page) */
    void mousePressEvent(QMouseEvent* event) override {
        // Ensure that if you click on not the bg it doesn't go back and handles the click appropriately
        QPointF scenePos = mapToScene(event->pos());
        QGraphicsItem* item = scene()->itemAt(scenePos, transform());

        if (item) {
            QGraphicsView::mousePressEvent(event);
        } else {
            backFun(wind);
            event->accept();
        }
    }
    bool event(QEvent *ev) override {
        // As the space key for some reason does not like being passed to the text items,
        // We have to manually insert the space ourselves.

        // Ensure the event is the specific type we want to catch, for the specific type of parent
        if (ev->type() == QEvent::KeyPress) {
            auto *keyEv = static_cast<QKeyEvent*>(ev);
            if (keyEv->key() == Qt::Key_Space) {
                auto *item = scene()->focusItem();
                if (item && item->type() == QGraphicsTextItem::Type) {
                    // The title text boxes
                    auto *txtItem = static_cast<QGraphicsTextItem*>(item);
                    txtItem->textCursor().insertText(" ");
                    bigW->updateChildren(true, true); // Don't forget to update the children!
                    return true;
                } else if (item && item->type() == QGraphicsProxyWidget::Type) {
                    // The todo items - update their label!
                    auto *proxyItem = static_cast<TodoGraphicObject*>(item);
                    proxyItem->addText(" ");
                    return true;
                }
            }
        }
        return QGraphicsView::event(ev);

    }
    /*! \brief Remove x scrolling */
    void offsetPos(int x, int y) override {
        GraphicsViewCanvas::offsetPos(0, y);
    }
    /*! \brief Update bg sizing on resize */
    void resizeEvent(QResizeEvent *event) override {
        bigW->updateWidth(event->size().width());
        GraphicsViewCanvas::resizeEvent(event);
        updateRect();
    }
    /*! \brief Remove zoom */
    void zoom(int delta) {}
private:
    /*! \brief The parent Window */
    Window* wind;
    /*! \brief The big task widget to feed the spaces into */
    BigTaskWidget* bigW = nullptr;
};

/*! \brief A function to add new subtasks, used by the buttons */
void addNewSubtask(Window* wind, BigTaskWidget* bigW, QString txt) {
    bigW->todos.push_back(new TodoGraphicObject(txt, true, bigW));
    bigW->updateChildren(true, true);
}

void taskOverlay(Window* wind, TaskWidget* task) {
    // Make the overlay
    OverlayWid *overlay = new OverlayWid(wind);
    overlay->show();

    // Make the graphics scene with a generated big task widget
    QGraphicsScene* scene = new QGraphicsScene();
    BigTaskWidget* bigW = task->toBigWidget();
    scene->addItem(bigW);
    NewGraphicsView *view = new NewGraphicsView(scene, bigW, wind);
    view->show();

    // Make the back btn
    SvgBtnWidget *btn = new SvgBtnWidget(":/assets/backBtn.svg", wind);
    wind->connect(btn, &QPushButton::released, wind, [wind](){backFun(wind);});
    btn->show();

    // Make the delete btn
    SvgBtnWidget *btn2 = new SvgBtnWidget(":/assets/binBtn.svg", wind);
    // On click delete the task and go back
    wind->connect(btn2, &QPushButton::released, wind, [wind, task](){
        // Delete task from sections
        for (auto& s : wind->sections) {
            auto result = std::find(s.begin(), s.end(), task);
            if (result != std::end(s)) {
                s.erase(result);
                break;
            }
        }

        backFun(wind); // Remove overlay
        delete task; // Delete
        updateTaskPoss(wind); // And update!
    });
    btn2->show();

    // Make the add sub-task input box
    InputWidget* newSubtask = new InputWidget(wind);
    newSubtask->setPlaceholderText("New subtask");
    // Run the new subtask func on enter pressed
    QObject::connect(newSubtask, &QLineEdit::returnPressed, [wind, bigW, newSubtask](){
        addNewSubtask(wind, bigW, newSubtask->text());
        newSubtask->setText("");
    });
    newSubtask->show();

    // Add all the widgets to the rmWids and wind->wids lists
    rmWids->push_back(overlay);
    rmWids->push_back(view);
    rmWids->push_back(btn);
    rmWids->push_back(btn2);
    rmWids->push_back(newSubtask);
    wind->wids.push_back(Widget{overlay, QPoint(0, 0), QSize(100, 100)});
    wind->wids.push_back(Widget{view, QPoint(50, 0), QSize(50, 100)});
    wind->wids.push_back(Widget{btn, QPoint(1, 1), QSize(8, 8), HEIGHT});
    wind->wids.push_back(Widget{btn2, QPoint(99, 1), QSize(8, 8), HEIGHT});
    wind->wids.push_back(Widget{newSubtask, QPoint(95, 50), QSize(18, 6)});

    wind->resizeElms(); // Update all the positionings!
}
