/**************************************************************************
**
**  spectrographwidget.cpp
**
**  Represent a frequency spectrum in frequency and amplitude as a series
**  of graphical bars.
**
**  Created by Andreas Kurth on 19.12.2012.
**
**  This work is closely based on a Qt example file distributed as
**  demos/spectrum/app/spectrograph.cpp. It falls under the original
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

#include "spectrographwidget.h"

SpectrographWidget::SpectrographWidget(QWidget *parent) :
    QWidget(parent)
{

}

void SpectrographWidget::setParams(double lowFreq, double highFreq, double freqPrecision)
{
    this->lowFreq = lowFreq;
    this->highFreq = highFreq;
    this->freqPrecision = freqPrecision;
    this->numBars = FrequencySpectrum::calcNumFrequencies(lowFreq, highFreq, freqPrecision);
    bars.resize(numBars);       // resize vector

    calcGeometry();
    update();
}

void SpectrographWidget::reset()
{
    spectrum.reset();
    updateBars();
}

void SpectrographWidget::spectrumChanged(const FrequencySpectrum &spectrum)
{
    this->spectrum = spectrum;
    updateBars();
}

void SpectrographWidget::calcGeometry()
{
    maxBarHeight = 0.875 * height();
    minBarHeight = 0.01 * height();

    barWidth = width() / (1.2*numBars + 0.2);
    gapWidth = 0.2 * barWidth;
}

void SpectrographWidget::updateBars()
{
    bars.fill(Bar());

    FrequencySpectrum::const_iterator i = spectrum.begin();
    const FrequencySpectrum::const_iterator end = spectrum.end();

    for ( ; i != end; ++i)
    {
        const FrequencySpectrum::Element e = *i;
        if (e.frequency >= lowFreq && e.frequency < highFreq)
        {
            Bar &bar = bars[getBarIndex(e.frequency)];
            bar.value = e.amplitude;
        }
    }
    update();
}

int SpectrographWidget::getBarIndex(double frequency) const
{
    Q_ASSERT(frequency >= lowFreq && frequency < highFreq);
    const int index = (frequency - lowFreq) / freqPrecision;
    Q_ASSERT(index >= 0 && index < bars.count());
    return index;
}

void SpectrographWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    // draw background
    painter.fillRect(rect(), Qt::black);

    // write texts
    QFont freqFont = QFont("Helvetica", 10);
    painter.setFont(freqFont);
    painter.setPen(Qt::white);
    QPoint lowFreqPos = QPoint(rect().left() + gapWidth + 8, 50);
    QPoint highFreqPos = QPoint(rect().right() - gapWidth, 50);
    QString lowFreqText = lowFreq >= 1000 ? QString::number(lowFreq/1000, 'f', 2) + QString(" kHz") : QString::number(lowFreq, 'f', 2) + QString(" Hz");
    QString highFreqText = highFreq >= 1000 ? QString::number(highFreq/1000, 'f', 2) + QString(" kHz") : QString::number(highFreq, 'f', 2) + QString(" Hz");
    painter.save();
    painter.translate(lowFreqPos);
    painter.rotate(270);
    painter.drawText(0, 0, lowFreqText);
    painter.restore();
    painter.save();
    painter.translate(highFreqPos);
    painter.rotate(270);
    painter.drawText(0, 0, highFreqText);
    painter.restore();

    // draw the bars
    QColor normalColor(255, 255, 255);
    QColor clippedColor(255, 0, 0);
    if (numBars)
    {
        double leftPos = rect().left() + gapWidth;
        for (int i=0; i<numBars; ++i)
        {
            QColor barColor = normalColor;
            double value = bars[i].value;
            if (value > 1.0)
            {
                value = 1.0;
                barColor = clippedColor;
            }
            else if (value < 0)
            {
                std::cerr << "warning: reset negative value" << std::endl;
                value = 0;
            }

            QRectF barRect = QRectF();
            double rightPos = leftPos + barWidth;
            double bottomPos = rect().height();
            double barHeight = maxBarHeight * value;
            barHeight = barHeight < minBarHeight ? minBarHeight : barHeight;
            double topPos = rect().height() - barHeight;

            barRect.setLeft(leftPos);
            barRect.setRight(rightPos);
            barRect.setBottom(bottomPos);
            barRect.setTop(topPos);

            painter.fillRect(barRect, barColor);
            leftPos += barWidth + gapWidth;
        }

    }
}

void SpectrographWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    calcGeometry();
}
