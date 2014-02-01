/* mainwindow.cpp
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(const QString& dir, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), dirwatch(dir)
{
    ui->setupUi(this);
    connect(ui->btnReload, SIGNAL(clicked()), this, SLOT(btnPressed()));
    connect(&dirwatch, SIGNAL(fileAdded(int, const QString)), this, SLOT(listIn(int, const QString&)));
    connect(&dirwatch, SIGNAL(fileRemoved(int, const QString)), this, SLOT(listOut(int, const QString&)));

    dirwatch.scan();
}

void MainWindow::listIn(int inode, const QString& str)
{
    if(inode || !inode)
        ui->listFiles->addItem(str);
}

void MainWindow::listOut(int inode, const QString& str)
{
    QListWidgetItem* item;

    for(int i = 0; i < ui->listFiles->count(); ++i) {
        item = ui->listFiles->item(i);
        if(item->text() == str)
            delete item;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnPressed()
{
    dirwatch.scan();
}
