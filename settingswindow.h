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

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "settings.h"

namespace Ui {
class settingsWindow;
}

class settingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit settingsWindow(QWidget *parent = 0, settings *set = NULL);
    ~settingsWindow();
    
private slots:
    void on_buttonBox_accepted();

    void on_pushButtonBinBrowse_clicked();

private:
    Ui::settingsWindow *ui;
    settings *setts;

    void loadSettings();
};

#endif // SETTINGSWINDOW_H
