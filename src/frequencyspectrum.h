/**************************************************************************
**
**  frequencyspectrum.h
**
**  Represent a frequency spectrum as a vector of elements, each consisting
**  of a frequency, an amplitude and a phase.
**
**  Created by Andreas Kurth on 19.12.2012.
**
**  This work is closely based on a Qt example file distributed as
**  demos/spectrum/app/frequencyspectrum.h. It falls under the original
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

#ifndef FREQUENCYSPECTRUM_H
#define FREQUENCYSPECTRUM_H

#include <QtCore/QVector>

class FrequencySpectrum
{
public:
    explicit FrequencySpectrum(int numPoints = 0, double minFreq = 0.0, double maxFreq = 0.0);

    struct Element
    {
        Element() : frequency(0.0), amplitude(0.0), phase(0.0) { }

        double  frequency;      // frequency in Hertz
        double  amplitude;      // amplitude in range [0.0, 1.0]
        double  phase;          // phase in range [0.0, 2*PI)
    };

    typedef QVector<Element>::iterator iterator;
    typedef QVector<Element>::const_iterator const_iterator;

    void            reset();

    int             count() const;
    Element&        operator[](int index);
    const Element&  operator[](int index) const;
    iterator        begin();
    iterator        end();
    const_iterator  begin() const;
    const_iterator  end() const;

    static double   calcFrequencyPrecision(int sampleRate, int numPoints);
    static int      calcNumFrequencies(double lowDisplayFreq, double highDisplayFreq, double precision);

private:
    QVector<Element> elements;

};

#endif // FREQUENCYSPECTRUM_H
