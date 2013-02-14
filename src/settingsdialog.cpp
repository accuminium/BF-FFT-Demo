/**************************************************************************
**
**  settingsdialog.cpp
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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog() :
    QDialog(),
    ui(new Ui::SettingsDialog)
{
    settingsFileName = QApplication::applicationDirPath() + "/settings.cfg";
    ui->setupUi(this);
    readSettings();
    updateSettingsDisplay();
    customizeUi();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

int SettingsDialog::getNumFftPoints()
{
    return this->numFftPoints;
}

double SettingsDialog::getLowDisplayFrequency()
{
    return this->lowDisplayFrequency;
}

double SettingsDialog::getHighDisplayFrequency()
{
    return this->highDisplayFrequency;
}

int SettingsDialog::getScaleFactor()
{
    return this->scaleFactor;
}

QString SettingsDialog::getUSBDeviceName()
{
    return this->usbDeviceName;
}

QString SettingsDialog::getHexFileName()
{
    return this->hexFileName;
}

void SettingsDialog::on_doubleSpinBoxLowDisplayFreq_editingFinished()
{
    lowDisplayFrequency = ui->doubleSpinBoxLowDisplayFreq->value();
    updateSettingsDisplay();
}

void SettingsDialog::on_doubleSpinBoxHighDisplayFreq_editingFinished()
{
    highDisplayFrequency = ui->doubleSpinBoxHighDisplayFreq->value();
    updateSettingsDisplay();
}

int SettingsDialog::fftPointsFromIndex(int index)
{
    return 1 << (3 + index);
}

int SettingsDialog::fftIndexFromPoints(int points)
{
    return log2(points)-3;  // TODO: remove magic numbers
}

int SettingsDialog::getSampleRate()
{
    return sampleRate;
}

void SettingsDialog::on_comboBoxNumFFTPoints_currentIndexChanged(int index)
{
    numFftPoints = fftPointsFromIndex(index);
    updateSettingsDisplay();
}

void SettingsDialog::on_spinBoxScaleFactor_editingFinished()
{
    scaleFactor = ui->spinBoxScaleFactor->value();
    updateSettingsDisplay();
}

void SettingsDialog::on_lineEditUSBDeviceName_editingFinished()
{
    usbDeviceName = ui->lineEditUSBDeviceName->text();
    updateSettingsDisplay();
}

void SettingsDialog::on_lineEditHexFileName_editingFinished()
{
    hexFileName = ui->lineEditHexFileName->text();
    updateSettingsDisplay();
}

void SettingsDialog::customizeUi()
{
    ui->doubleSpinBoxLowDisplayFreq->setMinimum(0);
    ui->doubleSpinBoxLowDisplayFreq->setMaximum(sampleRate);
    ui->doubleSpinBoxHighDisplayFreq->setMinimum(0);
    ui->doubleSpinBoxHighDisplayFreq->setMaximum(sampleRate);
}

void SettingsDialog::updateSettingsDisplay()
{
    fftPrecision = FrequencySpectrum::calcFrequencyPrecision(sampleRate, numFftPoints);
    validateSettingsInput();
    numDisplayBars = FrequencySpectrum::calcNumFrequencies(lowDisplayFrequency, highDisplayFrequency, fftPrecision);
    displayValues();
    writeSettings();
}

void SettingsDialog::validateSettingsInput()
{
    double lowDisplayFrequencyRemainder = fmod(lowDisplayFrequency, fftPrecision);
    if (lowDisplayFrequencyRemainder != 0)
        lowDisplayFrequency -= lowDisplayFrequencyRemainder;

    double highDisplayFrequencyRemainder = fmod(highDisplayFrequency, fftPrecision);
    if (highDisplayFrequencyRemainder != 0)
        highDisplayFrequency -= highDisplayFrequencyRemainder;

    if (highDisplayFrequency <= lowDisplayFrequency)
        highDisplayFrequency = lowDisplayFrequency + fftPrecision;
}

void SettingsDialog::displayValues()
{
    ui->comboBoxNumFFTPoints->setCurrentIndex(fftIndexFromPoints(numFftPoints));

    ui->spinBoxSampleRate->setValue(sampleRate);

    ui->doubleSpinBoxFFTPrecision->setValue(fftPrecision);

    ui->doubleSpinBoxLowDisplayFreq->setSingleStep(fftPrecision);
    ui->doubleSpinBoxLowDisplayFreq->setValue(lowDisplayFrequency);

    ui->doubleSpinBoxHighDisplayFreq->setSingleStep(fftPrecision);
    ui->doubleSpinBoxHighDisplayFreq->setValue(highDisplayFrequency);

    ui->spinBoxNumDisplayBars->setValue(numDisplayBars);

    ui->spinBoxScaleFactor->setValue(scaleFactor);

    ui->lineEditUSBDeviceName->setText(usbDeviceName);

    ui->lineEditHexFileName->setText(hexFileName);
}

void SettingsDialog::writeSettings()
{
    QSettings settings(settingsFileName, QSettings::NativeFormat);

    settings.beginGroup("Display");
    settings.setValue("lowDisplayFrequency", getLowDisplayFrequency());
    settings.setValue("highDisplayFrequency", getHighDisplayFrequency());
    settings.setValue("scaleFactor", getScaleFactor());
    settings.endGroup();

    settings.beginGroup("FFT/Codec");
    settings.setValue("sampleRate", getSampleRate());
    settings.setValue("numFftPoints", getNumFftPoints());
    settings.endGroup();

    settings.beginGroup("Connection");
    settings.setValue("usbDeviceName", getUSBDeviceName());
    settings.setValue("hexFileName", getHexFileName());
    settings.endGroup();
}

void SettingsDialog::readSettings()
{
    QSettings settings(settingsFileName, QSettings::NativeFormat);

    settings.beginGroup("Display");
    lowDisplayFrequency = settings.value("lowDisplayFrequency", ui->doubleSpinBoxLowDisplayFreq->value()).toDouble();
    highDisplayFrequency = settings.value("highDisplayFrequency", ui->doubleSpinBoxHighDisplayFreq->value()).toDouble();
    scaleFactor = settings.value("scaleFactor", ui->spinBoxScaleFactor->value()).toInt();
    settings.endGroup();

    settings.beginGroup("FFT/Codec");
    sampleRate = settings.value("sampleRate", ui->spinBoxSampleRate->value()).toInt();
    numFftPoints = settings.value("numFftPoints", fftPointsFromIndex(ui->comboBoxNumFFTPoints->currentIndex())).toInt();
    settings.endGroup();

    settings.beginGroup("Connection");
    usbDeviceName = settings.value("usbDeviceName", ui->lineEditUSBDeviceName->text()).toString();
    hexFileName = settings.value("hexFileName", ui->lineEditHexFileName->text()).toString();
    settings.endGroup();
}
