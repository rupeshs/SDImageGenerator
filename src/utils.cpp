#include "utils.h"


Utils::Utils()
{

}

bool Utils::checkPathExists(const QString& path)
{
    QFileInfo checkFile(path);
    if (checkFile.isDir())
        return QDir(path).exists();
    else if (checkFile.isFile()) {
        return QFile::exists(path);
    }
    return false;
}

QString Utils::pathAppend(const QString& path1, const QString& path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}

void Utils::ensurePath(const QString& path)
{
    if(!QDir(path).exists())
        QDir(path).mkdir(path);

}

QUrl Utils::localPathToUrl(const QString &path)
{
   QString cleanedPath = QDir::cleanPath(path);
   return QUrl::fromLocalFile(cleanedPath);
}

QString Utils::getLogMessage(const QString& type,const QString& functionName,int line,const QString& msg)
{
    QString log;
#ifdef QT_DEBUG
       log =  QString(" [%1] [%2 -> %3] %4").arg(type).arg(functionName).arg(line).arg(msg);
#else
       log = QString(" %1").arg(msg);
#endif
   return log;
}
