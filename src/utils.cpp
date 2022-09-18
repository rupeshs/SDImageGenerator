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
