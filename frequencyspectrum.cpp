/**************************************************************************
**
**  frequencyspectrum.cpp
**
**  Represent a frequency spectrum as a vector of elements, each consisting
**  of a frequency, an amplitude and a phase.
**
**  Created by Andreas Kurth on 19.12.2012.
**
**  This work is closely based on a Qt example file distributed as
**  demos/spectrum/app/frequencyspectrum.cpp. It falls under the original
**  license as stated below.
**
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
**
**************************************************************************/

#include "frequencyspectrum.h"

FrequencySpectrum::FrequencySpectrum(int numPoints, double minFreq, double maxFreq)
{
    elements = QVector<Element>(numPoints);

    double freqRange = maxFreq - minFreq;
    double deltaFreq = freqRange / numPoints;

    double freq = minFreq;
    for (iterator i = begin(); i != end(); ++i)
    {
        (*i).frequency = freq;
        freq += deltaFreq;
    }

    reset();
}

void FrequencySpectrum::reset()
{
    for (iterator i = begin(); i != end(); ++i)
    {
        (*i).amplitude = 0.0;
        (*i).phase = 0.0;
    }
}

int FrequencySpectrum::count() const
{
    return elements.count();
}

FrequencySpectrum::Element& FrequencySpectrum::operator [](int index)
{
    return elements[index];
}

const FrequencySpectrum::Element& FrequencySpectrum::operator[](int index) const
{
    return elements[index];
}

FrequencySpectrum::iterator FrequencySpectrum::begin()
{
    return elements.begin();
}

FrequencySpectrum::iterator FrequencySpectrum::end()
{
    return elements.end();
}

FrequencySpectrum::const_iterator FrequencySpectrum::begin() const
{
    return elements.begin();
}

FrequencySpectrum::const_iterator FrequencySpectrum::end() const
{
    return elements.end();
}

double FrequencySpectrum::calcFrequencyPrecision(int sampleRate, int numPoints)
{
    return (double)sampleRate / numPoints;
}

int FrequencySpectrum::calcNumFrequencies(double lowDisplayFreq, double highDisplayFreq, double precision)
{
    return (highDisplayFreq - lowDisplayFreq)/precision;
}
