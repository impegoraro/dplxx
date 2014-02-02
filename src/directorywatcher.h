/* directorywatcher.h
 * Copyright (C) 2014  Ilan Pegoraro
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DIRECTORYWATCHER_H
#define DIRECTORYWATCHER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QList>
#include <QStringList>
#include <QSocketNotifier>
#include <QLinkedList>
#include <sys/inotify.h>

class DirectoryWatcher : public QObject
{
    Q_OBJECT
public:
    DirectoryWatcher(const QString& file);
    //DirectoryWatcher(const DirectoryWatcher& dw);
    ~DirectoryWatcher();

    QStringList scan();

public slots:
    void readSocket(int fd);

signals:
    void fileAdded(int inode, const QString& file);
    void fileRemoved(int inode, const QString& file);


protected:
    int mWatchFd;
    QDir mDir;
    QStringList mFileList;
    QSocketNotifier *mSockNot;
    QLinkedList<DirectoryWatcher*> subDirs;

protected slots:
    void handlerfileAdded(int inode, const QString& file);
    void handlerfileRemoved(int inode, const QString& file);
};

#endif // DIRECTORYWATCHER_H
