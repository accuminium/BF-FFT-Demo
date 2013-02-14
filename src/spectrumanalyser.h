/**************************************************************************
**
**  spectrumanalyser.h
**
**  Create a frequency spectrum from received amplitudes. Values are
**  processed in order and spectrum is emitted and (internally) reset upon
**  receival of a sync code.
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

#ifndef SPECTRUMANALYSER_H
#define SPECTRUMANALYSER_H

#include <QObject>
#include <iostream>
#include <cmath>
#include "frequencyspectrum.h"

#define FFTSYNCCODE 0x8FFFFFFF

class SpectrumAnalyser : public QObject
{
    Q_OBJECT

public:
    explicit SpectrumAnalyser(int numPoints, double minFreq, double maxFreq, int scaleFactor);

    void        resetAnalyser(int numPoints, double minFreq, double maxFreq);
    void        setScaleFactor(int scaleFactor);
    
signals:
    void spectrumChanged(const FrequencySpectrum &spectrum);
    
public slots:
    void receiveValue(int32_t val);
    
private:
    FrequencySpectrum           spectrum;
    FrequencySpectrum::iterator spectrumPos;
    int                         scaleFactor;


};

#endif // SPECTRUMANALYSER_H
