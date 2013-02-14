/**************************************************************************
**
**  bfconnectionthread.h
**
**  Threaded serial connection to BFKit over USB port.
**
**  Created by Andreas Kurth on 19.12.2012.
**
**  This work is based on bflod.c, a hex file uploader for bfkit created
**  and copyright (C) 2008, ISI/ETH Zurich, strebel@isi.(...).
**
**  This is free software in terms of the GNU General Public Lisense as
**  published by the Free Software Foundation. Absolutely no warranty.
**
**************************************************************************/

#ifndef BFCONNECTION_H
#define BFCONNECTION_H

#include <QObject>
#include <QThread>
#include <termios.h>
#include <fcntl.h>              // O_RDWR etc.
#include <errno.h>
#include <iostream>             // std::cerr etc.

#define B1843200    1843200
#define SYNCBYTE    0xAF

typedef unsigned char   byte;

class BFConnectionThread : public QThread
{
    Q_OBJECT
public:
    explicit BFConnectionThread(std::string usbDeviceName, std::string hexFileName);
    ~BFConnectionThread();

    void    run();
    
signals:
    void    byteReceived(byte bt);
    void    intReceived(int32_t i);
    void    errorOccured(const char* errMsg);
    void    connectionEstablished();
    
public slots:

private slots:
    void    combineBytesToInt(byte bt);

private:
    struct  termios oldtio, newtio; /* old and new tty settings */
    struct  termios oldsio, newsio; /* old and new stdio settings */
    char*   devnam;                 /* name of serial device */
    char*   filenam;                /* file name of hex file */
    int     ifil;                   /* input file descriptor */
    int     devd;                   /* output file descriptor */
    int     tmod;                   /* true if tty settings modified */
    int     smod;                   /* true if stdio settings modified */
    int     baud;                   /* bits per second (constant from termios.h) */
    int     bnum;                   /* bits per second (integer) */
    int     xram;                   /* initialize external SDRAM */
    int     initBaud;               /* initial baud rate of bfkit */

    int32_t intBuf;
    short   intBufState;

    int     initTTY(int baud);
    void    die(const char* causingFunction="", int errorNumber=0);
    int     changeBaud(int bdiv);

    int     writeAvailable(int fd, int millis);
    int     readAvailable(int fd, int millis);
    byte    rxByte(void);
    void    txByte(byte bt);
    void    txHex(byte bt);
    
};

#endif // BFCONNECTION_H
