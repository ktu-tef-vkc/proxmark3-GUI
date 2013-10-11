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

#include "proxcontrol.h"

proxControl::proxControl(MainWindow *mww)
{
    this->mw = mww;
    nt = new needThings(this);
    nt->setNeeded(THING_proxFirstTime);
}

proxControl::~proxControl()
{
    clean();
}

int proxControl::initialize(QString clientLocation, QString port)
{
    nt->setNeeded(THING_proxFirstTime);

    //QString program = "/home/buzzy/qtProjects/build-proxmark3-Desktop-Debug/proxmark3_gui";
    QStringList arguments;
    arguments << port << "flush";

    myProcess = new QProcess(this);
    myProcess->setProcessChannelMode(QProcess::SeparateChannels);
    myProcess->start(clientLocation, arguments);

    connect(myProcess, SIGNAL(readyRead()), this, SLOT(readOut()));
    connect(nt, SIGNAL(isReady(bool)), this, SLOT(isReadyForCommand(bool)));
    connect(this, SIGNAL(ready(bool)), mw, SLOT(isReadyForCommand(bool)));

    return 0;
}

void proxControl::readOut()
{
    QProcess *p = dynamic_cast<QProcess *>(sender());

    if (p)
    {
        QString str = p->readAllStandardOutput();
        mw->appendToLog(nt->filter(str));
    }
}

void proxControl::isReadyForCommand(bool isready)
{
    emit ready(isready);
}

void proxControl::clean()
{
    myProcess->write("exit\n");
}

bool proxControl::getVersion()
{
    if (nt->setNeeded(THING_version))
    {
        myProcess->write("hw version\n");
        return true;
    }
    return false;
}

void proxControl::finishedCheckingForThings(things th, QString str)
{
    nt->setToNothing();

    switch (th)
    {
    case THING_version:
    {
        mw->setVersion(str);
        break;
    }
    case THING_proxmark:
        break;
    case THING_proxFirstTime:
        if (str == nt_error)
            isReadyForCommand(false);
        else
            getVersion();
        break;
    default:
        break;
    }
}

bool proxControl::isReady()
{
    if (nt->getNeeded() == THING_nothing)
        return true;
    return false;
}

bool proxControl::mifare(mifareType mf, QString args)
{
    if (nt->setNeeded(THING_proxmark))
    {
        switch (mf) {
        case mf_check:
            myProcess->write(QString("hf mf chk " + args + "\n").toStdString().c_str());
            return true;
        case mf_dump:
            myProcess->write(QString("hf mf dump\n").toStdString().c_str());
            return true;
        case mf_nested:
            myProcess->write(QString("hf mf nested " + args + "\n").toStdString().c_str());
            return true;
        case mf_rdbl:
            myProcess->write(QString("hf mf rdbl " + args + "\n").toStdString().c_str());
            return true;
        case mf_rdsc:
            myProcess->write(QString("hf mf rdsc " + args + "\n").toStdString().c_str());
            return true;
        case mf_restore:
            myProcess->write(QString("hf mf restore\n").toStdString().c_str());
            return true;
        case mf_wrbl:
            myProcess->write(QString("hf mf wrbl " + args + "\n").toStdString().c_str());
            return true;
        default:
            return false;
        }
    }
    return false;
}
