/**************************************************************************
**
**  main.cpp
**
**  This project visualizes the fast Fourier transform (FFT) performed on
**  BFKit by displaying the obtained frequency amplitudes. It was part of
**  a student project at ISI/ETH Zurich on digital signal processing.
**  The BFKit was created at ISI and consists (amongst others) of an
**  ADSP-BF532 (processor), a FT232R (USB UART IC) and a TLV320AIC23BPW
**  (audio codec).
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
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    
    return a.exec();
}
