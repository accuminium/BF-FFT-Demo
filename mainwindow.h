/**************************************************************************
**
**  mainwindow.h
**
**  Main UI window for BFKit FFT Demo.
**
**  Created by Andreas Kurth on 19.12.2012.
**
**  This work is licensed under the Creative Commons
**  Attribution-ShareAlike 3.0 Unported License (CC BY-SA)
**  and distributed with absolutely no warranty.
**  To view a copy of this license, please visit
**  http://creativecommons.org/licenses/by-sa/3.0/.
**
**************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include "bfconnectionthread.h"
#include "spectrumanalyser.h"
#include "settingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void handleConnectionEstablished();
    void handleConnectionError(const char* errMsg);

private slots:
    void on_actionPreferences_triggered();
    void settingsChanged();


private:
    Ui::MainWindow *ui;

    BFConnectionThread* BFConThread;
    SpectrumAnalyser*   SpecAnalyser;
    SettingsDialog*     SetDialog;
    int                 numFftPoints;
    double              fftPrecision;
    double              lowDisplayFrequency;
    double              highDisplayFrequency;
    int                 sampleRate;
    int                 scaleFactor;
    QString             usbDeviceName;
    QString             hexFileName;

    void    connectUi();
    void    applySettings();
    void    retrieveSettings();
};

#endif // MAINWINDOW_H
