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

#include "settings.h"
#include "QCoreApplication"

settings::settings()
{
    QCoreApplication::setOrganizationName("KTU");
    QCoreApplication::setOrganizationDomain("ktu.lt");
    QCoreApplication::setApplicationName("Proxmark3GUI");

    mySettings = new QSettings();
}

QString settings::getValue(QString str)
{
    return mySettings->value(str).toString();
}

int settings::getValueInt(QString str)
{
    return mySettings->value(str).toInt();
}

void settings::saveValue(QString str, QString value)
{
    mySettings->setValue(str, value);
}

void settings::saveValue(QString str, int value)
{
    mySettings->setValue(str, value);
}

void settings::save()
{
    mySettings->sync();
}
