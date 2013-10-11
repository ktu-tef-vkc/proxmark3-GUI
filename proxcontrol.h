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

#ifndef PROXCONTROL_H
#define PROXCONTROL_H

enum mifareType {
    mf_check, mf_nested, mf_dump, mf_restore,
    mf_wrbl, mf_rdbl, mf_rdsc
};

#include <QObject>
#include <QProcess>
#include "mainwindow.h"
#include "needthings.h"

class MainWindow;
class needThings;

class proxControl : public QObject
{
    Q_OBJECT
public:
    proxControl(MainWindow *mw);
    ~proxControl();

    int initialize(QString clientLocation, QString port);
    bool getVersion();
    void finishedCheckingForThings(things th, QString list);
    bool isReady();
    bool mifare(mifareType mf, QString args);

public slots:
    void readOut();
    void isReadyForCommand(bool ready);

signals:
    void ready(bool isready);  

private:
    QProcess *myProcess;
    MainWindow *mw;
    QList<QString> *list;
    needThings *nt;

    void clean();

};

#endif // PROXCONTROL_H
