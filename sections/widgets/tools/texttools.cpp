#include "texttools.h"
#include <QFontDatabase>

QFont getAFont(std::vector<QString> fontOpts, int size) {
    QFont font;
    for (const QString &fontName : fontOpts) {
        if (QFontDatabase().hasFamily(fontName)) {
            font.setFamily(fontName);
            break;
        }
    }
    font.setPointSize(size);
    return font;
}
