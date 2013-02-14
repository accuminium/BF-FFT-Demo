/**************************************************************************
**
**  spectrumanalyser.cpp
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

#include "spectrumanalyser.h"

SpectrumAnalyser::SpectrumAnalyser(int numPoints, double minFreq, double maxFreq, int scaleFactor) :
    QObject()
{
    resetAnalyser(numPoints, minFreq, maxFreq);
    setScaleFactor(scaleFactor);
}

void SpectrumAnalyser::resetAnalyser(int numPoints, double minFreq, double maxFreq)
{
    spectrum = FrequencySpectrum(numPoints, minFreq, maxFreq);
    spectrumPos = spectrum.begin();
}

void SpectrumAnalyser::setScaleFactor(int scaleFactor)
{
    this->scaleFactor = scaleFactor;
}

void SpectrumAnalyser::receiveValue(int32_t val)
{

    if ((unsigned)val == FFTSYNCCODE)
    {
        spectrumPos = spectrum.begin();
        emit spectrumChanged(spectrum);
        return;
    }

    if (spectrumPos == spectrum.end())
    {
        spectrumPos = spectrum.begin();
        std::cerr << "warning: spectrum overflow reset" << std::endl;
    }

    // amplitudes are formatted as bf hex and need to be converted to double first
    double d = (double)val / INT32_MAX;
    // amplitudes are transmitted as square values
    d = sqrt(d);
    // scale amplitudes
    d *= scaleFactor;

    (*spectrumPos).amplitude = d;
    spectrumPos++;
}
