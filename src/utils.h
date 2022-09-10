#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDir>
#include <QFileInfo>


class Utils
{
public:
    static bool checkPathExists(const QString& path) ;
    static QString pathAppend(const QString& path1, const QString& path2);

private:
    Utils();
};

#endif // UTILS_H
