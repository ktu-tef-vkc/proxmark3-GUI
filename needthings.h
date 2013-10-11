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

#ifndef NEEDTHINGS_H
#define NEEDTHINGS_H

enum things
{
    THING_nothing,
    THING_version,
    THING_proxmark,
    THING_proxFirstTime
};

#include <QObject>
#include "proxcontrol.h"

#define nt_error "Error"

class MainWindow;
class needThings;
class proxControl;

class needThings : public QObject
{
    Q_OBJECT
public:
    needThings(proxControl *parent);
    things getNeeded();
    bool setNeeded(things th);
    QString filter(QString str);
    void setToNothing();

signals:
    void isReady(bool ready);

private:
    proxControl *pc;
    things needed, needed2;

    QString filterVersion(QString str);
    QString filterProxmark3(QString str);
    QString filterProxmark3First(QString str);

    QString ver_os, ver_bootrom;
};

#endif // NEEDTHINGS_H
