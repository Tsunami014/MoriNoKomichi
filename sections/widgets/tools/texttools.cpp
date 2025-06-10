#include "texttools.h"
#include <QFontDatabase>

QFont getAFont(std::vector<QString> fontOpts, int size) {
    QFont font; // It will use the default font if one isn't found
    // Keep going until found one that exists or run out of options
    for (const QString &fontName : fontOpts) {
        if (QFontDatabase().hasFamily(fontName)) {
            font.setFamily(fontName);
            break;
        }
    }
    font.setPointSize(size);
    return font;
}
