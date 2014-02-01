/* directorywatcher.cpp
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

#include <QDebug>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <unistd.h>

#include "directorywatcher.h"

constexpr int EVENT_SIZE = (sizeof(struct inotify_event));
constexpr int BUF_LEN = (1024 * (EVENT_SIZE + 16));

DirectoryWatcher::DirectoryWatcher(const QString& dirStr)
    : mWatchFd(-1), mDir(dirStr), mSockNot(nullptr)
{
    mWatchFd = inotify_init();
    if(mWatchFd == -1) {
        perror("inotify_init");
        qDebug() << "Error: unable to initialize inotify to obtain file/directory notifications.";
        exit(1);
    }

    inotify_add_watch(mWatchFd, dirStr.toAscii(), IN_CREATE | IN_DELETE | IN_DELETE_SELF);

    mSockNot = new QSocketNotifier(mWatchFd, QSocketNotifier::Read);
    connect(mSockNot, SIGNAL(activated(int)), this, SLOT(readSocket(int)));
    mSockNot->setEnabled(true);
}

DirectoryWatcher::DirectoryWatcher(const DirectoryWatcher& dw)
    : mWatchFd(dw.mWatchFd), mDir(dw.mDir), mFileList(dw.mFileList), mSockNot(dw.mSockNot), subDirs(dw.subDirs)
{

}

DirectoryWatcher::~DirectoryWatcher()
{
    close(mWatchFd);
    delete mSockNot;
}

void DirectoryWatcher::scan()
{
    readDir(mDir.absolutePath());
}

void DirectoryWatcher::readDir(const QString& dirStr)
{
    QDir dir(dirStr);

    for(QFileInfo file : dir.entryInfoList()) {
        if(file == dir.canonicalPath() || file == dir.absoluteFilePath(".."))
            continue;

        if(file.isFile()) {
            mFileList<< file.absoluteFilePath();
            emit fileAdded(1, file.absoluteFilePath());
        } else if(file.isDir()) {
            DirectoryWatcher *subDir = new DirectoryWatcher(file.absoluteFilePath());

            connect(subDir, SIGNAL(fileAdded(int, const QString&)), this, SLOT(handlerfileAdded(int, const QString&)));
            connect(subDir, SIGNAL(fileRemoved(int, const QString&)), this, SLOT(handlerfileRemoved(int, const QString&)));
            subDir->scan();
            subDirs<< subDir;
        }
    }
}

void DirectoryWatcher::readSocket(int fd)
{
    int len, i = 0, err;
    char buf[BUF_LEN];
    struct inotify_event *event;
    bool reissuedCall;

    qDebug()<< "file/directory modification detected...";

    do {
        reissuedCall = false;
        len = read(fd, buf, BUF_LEN);
        if (len < 0) {
            if (errno == EINTR) {
                reissuedCall = true;
            } else {
                perror ("inotify - read");
                err = 1;
            }
        } else if (!len) {
            qDebug()<< ":( Well this is embarrasing"<<endl
                    << "Can't read the event because buffer is not large enough";
            abort();
        }
    } while(reissuedCall);

    if(err)
        qDebug()<< "Warning: something could be wrong with inotify...";

    while(i < len) {
        event = (struct inotify_event *) &buf[i];

        switch(event->mask) {
            case IN_CREATE:
                if (event->len)
                    emit fileAdded(1, mDir.absoluteFilePath(event->name));
            break;
            case IN_DELETE:
                if (event->len)
                    emit fileRemoved(1, mDir.absoluteFilePath(event->name));
            break;
        }

        i += EVENT_SIZE + event->len;
    }
}


void DirectoryWatcher::handlerfileAdded(int inode, const QString& file)
{
    emit fileAdded(inode, file);
}

void DirectoryWatcher::handlerfileRemoved(int inode, const QString& file)
{
    emit fileRemoved(inode, file);
}
