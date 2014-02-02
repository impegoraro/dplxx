/* mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "directorywatcher.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& dir, QWidget *parent = 0);
    ~MainWindow();
protected slots:
    void listIn(int inode, const QString& str);
    void listOut(int inode, const QString& str);
    void btnPressed();
    void txtFilter(const QString&);

private:
    Ui::MainWindow *ui;
    DirectoryWatcher dirwatch;
};

#endif // MAINWINDOW_H
