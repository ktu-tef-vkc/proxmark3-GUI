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

#ifndef SETTINGS_H
#define SETTINGS_H

#define SETT_PROGRAM "program"

#include "QSettings"

class settings
{
public:
    settings();

    QString getValue(QString str);
    int getValueInt(QString str);

    void saveValue(QString str, QString value);
    void saveValue(QString str, int value);

    void save();

private:
    QSettings *mySettings;

};

#endif // SETTINGS_H
