#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QUrl>

class Utils
{
public:
    static bool checkPathExists(const QString& path) ;
    static QString pathAppend(const QString& path1, const QString& path2);
    static void ensurePath(const QString& path);
    static QUrl localPathToUrl(const QString& path);
    static QString getLogMessage(const QString& type,const QString& functionName,int line,const QString& msg);

private:
    Utils();
};

#endif // UTILS_H
