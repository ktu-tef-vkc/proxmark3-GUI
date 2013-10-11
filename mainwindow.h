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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "proxcontrol.h"
#include "search_usb.h"
#include "settings.h"
#include "settingswindow.h"

#define DEF_STATUS_BUT_CON "Connect"
#define DEF_STATUS_BUT_DISCON "Disconnect"
#define DEF_WRITE_BLK "Write Block"
#define DEF_READ_BLK "Read Block"
#define DEF_READ_SCT "Read Sector"
#define DEF_MF_DUMP "Dump"
#define DEF_MF_RESTORE "Restore"

class proxControl;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void appendToLog(QString str);
    void appendToLog(QList<QString> *list);
    void setVersion(QString str);

    void clickButtonConnect();

    settings *mySettings;

    
private slots:
    void on_pushButton_clicked();

    void on_buttonConnect_clicked();

    void on_checkBoxMfChk12_stateChanged(int arg1);

    void on_checkBoxMfChkDic_stateChanged(int arg1);

    void on_pushButtonMfChk_clicked();

    void on_radioButtonMfNestedCrdMem_clicked();

    void on_radioButtonMfNestedOneSector_clicked();

    void on_pushButtonMfNestedExecuter_clicked();

    void on_pushButtonMfDumpR_clicked();

    void on_buttonMifare_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_actionSave_log_triggered();

    void on_actionClear_log_triggered();

    void on_actionOptions_triggered();

public slots:
    void isReadyForCommand(bool ready);

private:
    Ui::MainWindow *ui;
    proxControl *proxCtrl;

    void prepareUI();
    void prepareStatusBar();
    void prepareMifareReadWriteTab();
    void prepareNestedTab();
    void switchNested(bool oneSector);
    void renameMifareReadWriteTabs(QString blLabel, int blMin = 0, int blMax = 63, bool blData = false);
    void setStatusBarText(QString con, QString button, QString stat = "", QString dev = "", QString ver = "");
};

#endif // MAINWINDOW_H
