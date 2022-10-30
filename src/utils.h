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

#ifndef UTILS_H
#define UTILS_H

#include <QDesktopServices>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QUrl>


class Utils
{
public:
    static bool checkPathExists(const QString& path) ;
    static QString pathAppend(const QString& path1, const QString& path2);
    static void ensurePath(const QString& path);
    static QUrl localPathToUrl(const QString& path);
    static QString getLogMessage(const QString& type,const QString& functionName,int line,const QString& msg);
    static void openLocalFolderPath(const QString& path);

private:
    Utils();
};

#endif // UTILS_H
