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
#include "ui_mainwindow.h"
#include <QProcess>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <sys/types.h>
#include <QTextCodec>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
//#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    prepareUI();

    mySettings = new settings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::prepareUI()
{
    ui->comboBox->addItem(DEF_WRITE_BLK);
    ui->comboBox->addItem(DEF_READ_BLK);
    ui->comboBox->addItem(DEF_READ_SCT);

    ui->comboBoxKeyAB->addItem("A");
    ui->comboBoxKeyAB->addItem("B");

    ui->comboBoxMfCkKType->addItem("A");
    ui->comboBoxMfCkKType->addItem("B");
    ui->comboBoxMfCkKType->addItem("?");

    ui->comboBoxMFDumpR->addItem(DEF_MF_DUMP);
    ui->comboBoxMFDumpR->addItem(DEF_MF_RESTORE);

    prepareNestedTab();
    prepareStatusBar();

    ui->tabWidget_2->setEnabled(false);

    ui->tabWidget->removeTab(0);
}

void MainWindow::on_pushButton_clicked()
{
    proxCtrl->getVersion();
}

void MainWindow::appendToLog(QString str)
{
    ui->textBrowser->append(str);
}

void MainWindow::appendToLog(QList<QString> *list)
{
    foreach (QString l, *list)
        ui->textBrowser->append(l);
}

void MainWindow::setVersion(QString str)
{
    this->ui->labelStatusVer->setText(str);
}

void MainWindow::prepareStatusBar()
{
    this->statusBar()->addWidget(this->ui->labelStatusConnection);
    this->statusBar()->addWidget(this->ui->labelStatusDevice);
    this->statusBar()->addWidget(this->ui->buttonConnect);
    this->statusBar()->addWidget(this->ui->labelStatusVer);
    this->statusBar()->addWidget(this->ui->labelStatus);
    setStatusBarText("Not Connected", DEF_STATUS_BUT_CON, "", "", "");
}

void MainWindow::setStatusBarText(QString con, QString button, QString stat, QString dev, QString ver)
{
    ui->labelStatusConnection->setText(con);
    ui->buttonConnect->setText(button);
    ui->labelStatus->setText(stat);
    ui->labelStatusDevice->setText(dev);
    ui->labelStatusVer->setText(ver);
}

void MainWindow::on_buttonConnect_clicked()
{
    if (ui->buttonConnect->text() == DEF_STATUS_BUT_CON)
    {
        proxCtrl = new proxControl(this);
        search_usb su;
        QList<QString> list = su.enumerate();
        if (list.size() > 0)
            if (proxCtrl->initialize(mySettings->getValue(SETT_PROGRAM), list.at(0)) == 0)
            {
                setStatusBarText("Connected To:", DEF_STATUS_BUT_DISCON, "", list.at(0));
            }
    }
    else
    {
        delete proxCtrl;
        setStatusBarText("Not Connected", DEF_STATUS_BUT_CON, "");
        appendToLog("##### DISCONNECTED #####\n\n");
    }
}

void MainWindow::clickButtonConnect()
{
    on_buttonConnect_clicked();
}


void MainWindow::isReadyForCommand(bool ready)
{
    ui->tabWidget_2->setEnabled(ready);
}


///
/// \brief CHECK TAB FUNCTIONS
///

void MainWindow::on_pushButtonMfChk_clicked()
{
    if (ui->lineEditMfChkBlNr->text().size() != 2 &&
            ui->lineEditMfChkBlNr->text().size() != 12)
    {
        appendToLog("!!! Bad block number !!!");
        return;
    }
    else if (ui->checkBoxMfChk12->isChecked() &&
            ui->lineEditMfChk12->text().size() != 12)
    {
        appendToLog("!!! Key must be 12 hex symbols long !!!");
        return;
    }
    else if (ui->checkBoxMfChkDic->isChecked() &&
             !ui->lineEditMfChkDic->text().contains(".dic"))
    {
        appendToLog("!!! File name needs .dic extension !!!");
        return;
    }


    QString args = QString("%1%2%3%4%5")
            .arg(ui->lineEditMfChkBlNr->text(),
                 " " + ui->comboBoxMfCkKType->currentText(),
                 ui->checkBoxMfChkT->isChecked() ? " t" : "",
                 ui->checkBoxMfChk12->isChecked() ?
                     " " + ui->lineEditMfChk12->text() : "",
                 ui->checkBoxMfChkDic->isChecked() ?
                     " " + ui->lineEditMfChkDic->text() : "");

    appendToLog("\n##### Attempting to check card #####\nArgs: " + args);
    proxCtrl->mifare(mf_check, args);
}

void MainWindow::on_checkBoxMfChk12_stateChanged(int arg1)
{
    bool enable = false;
    if (arg1 == 2)
        enable = true;
    ui->lineEditMfChk12->setEnabled(enable);
}

void MainWindow::on_checkBoxMfChkDic_stateChanged(int arg1)
{
    bool enable = false;
    if (arg1 == 2)
        enable = true;
    ui->lineEditMfChkDic->setEnabled(enable);
}

///
/// \brief NESTED TAB FUNCTIONS
///


void MainWindow::on_radioButtonMfNestedCrdMem_clicked()
{
    switchNested(false);
}

void MainWindow::on_radioButtonMfNestedOneSector_clicked()
{
    switchNested(true);
}

void MainWindow::on_pushButtonMfNestedExecuter_clicked()
{
    QString args = QString("%1%2%3%4%5%6%7%8")
            .arg(ui->radioButtonMfNestedCrdMem->isChecked() ? ui->spinBoxMfNestedCrdMem->text() : "o")
            .arg(" " + ui->spinBoxMfNestedBlkNr->text())
            .arg(" " + ui->comboBoxMfNestedKey->currentText())
            .arg(" " + ui->lineEditMfNested12->text())
            .arg(ui->radioButtonMfNestedOneSector->isChecked() ? ui->spinBoxMfNestedTrgtBlNr->text().prepend(" ") : "")
            .arg(ui->radioButtonMfNestedOneSector->isChecked() ? ui->comboBoxMfNestedTrgtKey->currentText().prepend(" ") : "")
            .arg(ui->checkBoxMfNestedT->isChecked() ? " t" : "")
            .arg(ui->checkBoxMfNestedD->isChecked() ? " d" : "");
    //line_needed(true, NS_long);
    appendToLog(QString("###### SENDING NESTED COMMAND WITH ARGS: %1").arg(args));
    proxCtrl->mifare(mf_nested, args);
    //pctrl->proxmarkLongCmd(PM_NESTED, args);
}


void MainWindow::prepareNestedTab()
{
    ui->comboBoxMfNestedKey->addItem("A");
    ui->comboBoxMfNestedKey->addItem("B");
    ui->comboBoxMfNestedTrgtKey->addItem("A");
    ui->comboBoxMfNestedTrgtKey->addItem("B");
}

void MainWindow::switchNested(bool oneSector)
{
    if (!ui->labelMfNestedKey12->isEnabled())
    {
        ui->labelMfNestedBlkNr->setEnabled(true);
        ui->spinBoxMfNestedBlkNr->setEnabled(true);
        ui->labelMfNestedKey->setEnabled(true);
        ui->comboBoxMfNestedKey->setEnabled(true);
        ui->labelMfNestedKey12->setEnabled(true);
        ui->lineEditMfNested12->setEnabled(true);
        ui->checkBoxMfNestedT->setEnabled(true);
        ui->pushButtonMfNestedExecuter->setEnabled(true);
    }
    bool enable = true;
    if (!oneSector)
        enable = false;

    ui->checkBoxMfNestedD->setEnabled(!enable);
    ui->labelMfNestedTrgtBlNr->setEnabled(enable);
    ui->spinBoxMfNestedTrgtBlNr->setEnabled(enable);
    ui->labelMfNestedTrgtKey->setEnabled(enable);
    ui->comboBoxMfNestedTrgtKey->setEnabled(enable);
    ui->spinBoxMfNestedCrdMem->setEnabled(!enable);
}

///
/// \brief DUMP/RESTORE
///

void MainWindow::on_pushButtonMfDumpR_clicked()
{
    if (ui->comboBoxMFDumpR->currentText() == DEF_MF_DUMP)
    {
        proxCtrl->mifare(mf_dump, NULL);
    }
    else
    {
        proxCtrl->mifare(mf_restore, NULL);
    }
}

///
/// \brief READ/WRITE
///

void MainWindow::on_buttonMifare_clicked()
{
    if (ui->lineEdit12hex->text().size() != 12)
    {
        appendToLog("!!! Key must be 12 hex symbols long !!!\n!!! There are "
                       + QString::number(ui->lineEdit12hex->text().size()) + " !!!\n");

        return;
    }
    if (ui->comboBox->currentText() == DEF_WRITE_BLK)
    {
        if (ui->lineEdit32hex->text().size() != 32)
        {
            appendToLog("!!! Block data must be 32 hex symbols long !!!\n!!! There are "
                           + QString::number(ui->lineEdit32hex->text().size()) + " !!!\n");
            return;
        }

        appendToLog("##### Attempting to write block " + ui->spinBoxBlockNr->text() + " | Key type "
                       + ui->comboBoxKeyAB->currentText() + " | Key "
                       + ui->lineEdit12hex->text() + " | Block data "
                       + ui->lineEdit32hex->text());
        QString args = QString(ui->spinBoxBlockNr->text() + " "
                                        + ui->comboBoxKeyAB->currentText() + " "
                                        + ui->lineEdit12hex->text() + " "
                                        + ui->lineEdit32hex->text());
        proxCtrl->mifare(mf_wrbl, args);
    }
    else if (ui->comboBox->currentText() == DEF_READ_BLK)
    {
        appendToLog("##### Attempting to read block " + ui->spinBoxBlockNr->text() + " | Key type "
                       + ui->comboBoxKeyAB->currentText() + " | Key "
                       + ui->lineEdit12hex->text());
        QString args = QString(ui->spinBoxBlockNr->text() + " "
                               + ui->comboBoxKeyAB->currentText() + " "
                               + ui->lineEdit12hex->text());
        proxCtrl->mifare(mf_rdbl, args);
    }
    else //Read Sector
    {
        appendToLog("##### Attempting to read sector " + ui->spinBoxBlockNr->text() + " | Key type "
                       + ui->comboBoxKeyAB->currentText() + " | Key "
                       + ui->lineEdit12hex->text());
        QString args = QString(ui->spinBoxBlockNr->text() + " "
                               + ui->comboBoxKeyAB->currentText() + " "
                               + ui->lineEdit12hex->text());
        proxCtrl->mifare(mf_rdsc, args);
    }
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    prepareMifareReadWriteTab();
}

void MainWindow::prepareMifareReadWriteTab()
{
    if (ui->comboBox->currentText() == DEF_WRITE_BLK)
    {
        renameMifareReadWriteTabs("Block Nr.:", 0, 255, true);
    }
    else if (ui->comboBox->currentText() == DEF_READ_BLK)
    {
        renameMifareReadWriteTabs("Block Nr.:", 0, 255);
    }
    else //Read Sector
    {
        renameMifareReadWriteTabs(QString("Sector Nr.:"));
    }
}

void MainWindow::renameMifareReadWriteTabs(QString blLabel, int blMin, int blMax, bool blData)
{
    ui->labelBlockNr->setText(blLabel);
    ui->spinBoxBlockNr->setMinimum(blMin);
    ui->spinBoxBlockNr->setMaximum(blMax);
    ui->labelBlockData->setEnabled(blData);
    ui->lineEdit32hex->setEnabled(blData);
    if (blData)
        ui->buttonMifare->setText("Write");
    else
        ui->buttonMifare->setText("Read");
}

///
/// \brief Menu bar
///

void MainWindow::on_actionSave_log_triggered()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QString fileName = QFileDialog::getSaveFileName(this,
             tr("Save Log to File..."), "",
             tr("Log Files (*.log);;All Files (*)"));
    if (fileName.isEmpty())
            return;
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        else
        {
            QDataStream out(&file);

            //out.setVersion(QDataStream::Qt_4_8);
            out << ui->textBrowser->toPlainText().toUtf8();
        }
    }
}

void MainWindow::on_actionClear_log_triggered()
{
    ui->textBrowser->clear();
}

void MainWindow::on_actionOptions_triggered()
{
    settingsWindow *sw = new settingsWindow(this, mySettings);

    if (sw->exec() != sw->Accepted)
    {
        //qDebug() << "Cancelled";
    }
    else
    {
        //qDebug() << "Accepted";
    }
}
