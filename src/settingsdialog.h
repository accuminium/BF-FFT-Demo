/**************************************************************************
**
**  settingsdialog.h
**
**  Settings dialog enabling run-time customization of certain display
**  parameters.
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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <cmath>
#include <QDialog>
#include <QSettings>
#include "frequencyspectrum.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog();
    ~SettingsDialog();

    int     getNumFftPoints();
    int     getSampleRate();
    double  getLowDisplayFrequency();
    double  getHighDisplayFrequency();
    int     getScaleFactor();
    QString getUSBDeviceName();
    QString getHexFileName();

private slots:

    void on_doubleSpinBoxLowDisplayFreq_editingFinished();
    void on_doubleSpinBoxHighDisplayFreq_editingFinished();
    void on_comboBoxNumFFTPoints_currentIndexChanged(int index);
    void on_spinBoxScaleFactor_editingFinished();
    void on_lineEditUSBDeviceName_editingFinished();
    void on_lineEditHexFileName_editingFinished();

private:
    Ui::SettingsDialog *ui;

    int     numFftPoints;
    int     sampleRate;
    double  fftPrecision;
    double  lowDisplayFrequency;
    double  highDisplayFrequency;
    int     numDisplayBars;
    int     scaleFactor;
    QString usbDeviceName;
    QString hexFileName;

    QString settingsFileName;

    void    customizeUi();
    void    updateSettingsDisplay();
    void    validateSettingsInput();
    void    displayValues();

    void    writeSettings();
    void    readSettings();

    int     fftPointsFromIndex(int index);
    int     fftIndexFromPoints(int points);

};

#endif // SETTINGSDIALOG_H
