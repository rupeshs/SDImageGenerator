/*
    SDImageGenerator, Text to image generation AI app
    Copyright(C) 2022 Rupesh Sreeraman
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

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
