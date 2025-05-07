#ifndef WINDOW_H
#define WINDOW_H

#include "graphics.h"

#include <QWidget>

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private:
    Graphic graphics;
};

#endif
