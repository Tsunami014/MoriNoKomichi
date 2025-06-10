#include "todographicobject.h"
#include "taskwidget.h"

TodoLabel::TodoLabel(const QString& text, bool en, QWidget* parent)
    : QLineEdit(text, parent)
{
    setReadOnly(true);
    enabled = en;
    setStyleSheet("QLineEdit:read-only {"
                  "    border: none;"
                  "    background: rgb(255, 235, 210); }"
                  "QLineEdit {"
                  "    background: white; }");

    if (en) {
        setCursor(Qt::IBeamCursor);
    }
}
void TodoLabel::focusOutEvent(QFocusEvent *event) {
    if (!isReadOnly()) {
        this->unsetCursor();
        this->setSelection(0,0);
        this->setReadOnly(true);
    }
    QLineEdit::focusOutEvent(event);
}
void TodoLabel::mouseDoubleClickEvent(QMouseEvent *) {
    // Become editable if double click
    if (enabled) {
        this->setReadOnly(false);
        this->selectAll();
    }
}

TodoGraphicObject::TodoGraphicObject(QString nme, bool iseditable, TaskWidget* taskparent) : QGraphicsProxyWidget(taskparent) {
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout();
    widget->setLayout(layout);

    editable = iseditable;
    parent = taskparent;

    // Add a checkbox and label
    checkbox = new QCheckBox();
    checkbox->setStyleSheet("QWidget { background: rgb(255, 235, 210); }");
    checkbox->setEnabled(iseditable);
    if (iseditable) {
        checkbox->setCursor(Qt::PointingHandCursor);
    }
    label = new TodoLabel(nme, iseditable);

    if (iseditable) {
        // Ensure that when they get updated, so to does the parent
        QObject::connect(label, &QLineEdit::textChanged,
                         this,  &TodoGraphicObject::updateParentlab);
        QObject::connect(checkbox, &QCheckBox::clicked,
                         this,     &TodoGraphicObject::updateParentchk);
    }

    checkbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Add the things to the layout and widget
    layout->addWidget(checkbox);
    layout->addWidget(label);
    widget->setStyleSheet("QWidget { background: rgb(255, 235, 210); }");
    if (!iseditable) {
        widget->setCursor(Qt::PointingHandCursor);
        label->setCursor(Qt::PointingHandCursor);
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        checkbox->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    }
    setWidget(widget);
}

void TodoGraphicObject::setWidth(unsigned int width) {
    QSizeF current = this->size();
    this->resize(width, current.height());
}

void TodoGraphicObject::updateParentlab(QString str) {
    Q_UNUSED(str);
    parent->updateChildren(true, true);
}
void TodoGraphicObject::updateParentchk(int state) {
    Q_UNUSED(state);
    parent->updateChildren();
}

QString TodoGraphicObject::getname() {
    return label->text();
}
void TodoGraphicObject::setName(QString newName) {
    label->setText(newName);
}
bool TodoGraphicObject::getChecked() {
    return checkbox->isChecked();
}
void TodoGraphicObject::setChecked(bool checked) {
    checkbox->setChecked(checked);
}
