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

#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "QFileDialog"

settingsWindow::settingsWindow(QWidget *parent, settings *set) :
    QDialog(parent),
    ui(new Ui::settingsWindow)
{
    ui->setupUi(this);
    setts = set;
    loadSettings();
}

settingsWindow::~settingsWindow()
{
    delete ui;
}

void settingsWindow::on_buttonBox_accepted()
{
    setts->saveValue(SETT_PROGRAM, ui->lineEditClientLoc->text());
    setts->save();
    accept();
}

void settingsWindow::loadSettings()
{
    ui->lineEditClientLoc->setText(setts->getValue(SETT_PROGRAM));
}

void settingsWindow::on_pushButtonBinBrowse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
             tr("Proxmark3 bin location"), "",
             tr("All Files (*)"));
    if (fileName.isEmpty())
        return;
    ui->lineEditClientLoc->setText(fileName);
}
