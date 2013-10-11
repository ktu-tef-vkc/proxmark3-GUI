//-----------------------------------------------------------------------------
// Copyright (C) 2013
//
// Zygimantas Butkus buzzy.ltu@gmail.com
// Tautvydas Zelvys gtautvis@yandex.ru
//
// Kaunas University of Technology
// Telecommunications and electronics department
// Virtual Center of excellence vkc@ktu.lt
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
