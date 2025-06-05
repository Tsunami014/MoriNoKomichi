#ifndef TEXTTOOLS_H
#define TEXTTOOLS_H

#include <QFont>

/*!
    \brief Query the font database for all the font options and return a font with that family (or default if none found)
*/
QFont getAFont(std::vector<QString> fontOpts, int pointSize = -1);

#endif // TEXTTOOLS_H
