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

#include "needthings.h"

needThings::needThings(proxControl *parent)
{
    pc = parent;
    needed = THING_nothing;
    needed2 = THING_nothing;
    ver_bootrom = "";
    ver_os = "";
}

things needThings::getNeeded()
{
    return needed;
}

bool needThings::setNeeded(things th)
{
    if (needed != THING_nothing)
        return false;

    needed = th;
    emit isReady(th == THING_nothing ? true : false);
    return true;
}

QString needThings::filter(QString str)
{
    switch (needed)
    {
    case THING_version:
        return filterVersion(str);
    case THING_proxmark:
        return filterProxmark3(str);
    case THING_proxFirstTime:
        return filterProxmark3First(str);
    case THING_nothing:
        return str;
    default:
        return str;
    }

}

QString needThings::filterVersion(QString str)
{
    if (str.contains("#db#")){

        str.replace("\nproxmark3>", "");
        str.replace("proxmark3>", "");
        str.replace("\r#", "\n#");
        str.replace("\r", "");

        if (str.contains("os"))
            ver_os = str.mid(str.lastIndexOf("os") + 4, 7);
        if (str.contains("bootrom"))
            ver_bootrom = str.mid(str.lastIndexOf("bootrom") + 9, 7);

        if (ver_os != "" && ver_bootrom != "")
        {
            QString toList = QString("os: %1 | bootrom: %2").arg(ver_os, ver_bootrom);
            //list->append(toList);
            pc->finishedCheckingForThings(THING_version, toList);
        }

        return str;
    }
    return "";
}

QString needThings::filterProxmark3(QString str)
{
    if (str.contains("proxmark3>"))
    {
        str.replace("proxmark3>", "#### FINISHED ####");
        pc->finishedCheckingForThings(needed, "");
    }
    return str;
}

QString needThings::filterProxmark3First(QString str)
{
    if (str.contains("ERROR"))
    {
        pc->finishedCheckingForThings(needed, nt_error);
    }
    else if (str.contains("proxmark3>"))
    {
        str.replace("proxmark3>", "#### FINISHED ####");
        pc->finishedCheckingForThings(needed, "");
    }
    return str;
}

void needThings::setToNothing()
{
    needed = THING_nothing;
    emit isReady(true);
}
