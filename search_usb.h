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

#ifndef SEARCH_USB_H
#define SEARCH_USB_H

#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <string>
#include <sstream>

#include <QThread>

class search_usb: public QThread
{
    Q_OBJECT
signals:
    void device_state_change(QString new_state, QString address);

protected:
    virtual void run();

public:
    search_usb();
    int monitor();
    QList<QString> enumerate();
    virtual ~search_usb();


private:
    struct udev *udev;

};

#endif // SEARCH_USB_H
