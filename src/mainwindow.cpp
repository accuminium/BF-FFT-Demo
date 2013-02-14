/**************************************************************************
**
**  mainwindow.cpp
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // register meta types to allow connect() to work with "custom" types
    qRegisterMetaType<int32_t>("int32_t");
    qRegisterMetaType<FrequencySpectrum>("FrequencySpectrum");

    SetDialog = new SettingsDialog();
    retrieveSettings();

    BFConThread = new BFConnectionThread(usbDeviceName.toStdString(), hexFileName.toStdString());

    SpecAnalyser = new SpectrumAnalyser(numFftPoints, 0, sampleRate, scaleFactor);
    ui->spectroWidget->setParams(lowDisplayFrequency, highDisplayFrequency, fftPrecision);

    connectUi();
    BFConThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete SpecAnalyser;
    BFConThread->terminate();
    BFConThread->wait();
    delete BFConThread;
    delete SetDialog;
}

void MainWindow::handleConnectionEstablished()
{
    ui->statusBar->showMessage("Connection established.");
}

void MainWindow::handleConnectionError(const char *errMsg)
{
    QString msg = QString("Error: ") + QString(errMsg);
    ui->statusBar->showMessage(msg);
}

void MainWindow::on_actionPreferences_triggered()
{
    SetDialog->show();
}

void MainWindow::settingsChanged()
{
    applySettings();
}

void MainWindow::connectUi()
{
    connect(BFConThread, SIGNAL(connectionEstablished()), this, SLOT(handleConnectionEstablished()));
    connect(BFConThread, SIGNAL(errorOccured(const char*)), this, SLOT(handleConnectionError(const char*)));
    connect(SetDialog, SIGNAL(accepted()), this, SLOT(settingsChanged()));

    connect(BFConThread, SIGNAL(intReceived(int32_t)), SpecAnalyser, SLOT(receiveValue(int32_t)));
    connect(SpecAnalyser, SIGNAL(spectrumChanged(FrequencySpectrum)), ui->spectroWidget, SLOT(spectrumChanged(FrequencySpectrum)));
}

void MainWindow::retrieveSettings()
{
    numFftPoints = SetDialog->getNumFftPoints();
    sampleRate = SetDialog->getSampleRate();
    // baudRate not allowed to change
    fftPrecision = FrequencySpectrum::calcFrequencyPrecision(sampleRate, numFftPoints);
    lowDisplayFrequency = SetDialog->getLowDisplayFrequency();
    highDisplayFrequency = SetDialog->getHighDisplayFrequency();
    scaleFactor = SetDialog->getScaleFactor();
    usbDeviceName = SetDialog->getUSBDeviceName();
    hexFileName = SetDialog->getHexFileName();
}

void MainWindow::applySettings()
{
    retrieveSettings();

    SpecAnalyser->resetAnalyser(numFftPoints, 0, sampleRate);
    SpecAnalyser->setScaleFactor(scaleFactor);
    ui->spectroWidget->setParams(lowDisplayFrequency, highDisplayFrequency, fftPrecision);
}
