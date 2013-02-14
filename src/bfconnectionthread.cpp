/**************************************************************************
**
**  bfconnectionthread.cpp
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

#include "bfconnectionthread.h"

BFConnectionThread::BFConnectionThread(std::string usbDeviceName, std::string hexFileName) : QThread()
{
    xram = 1;
    bnum = 1843200;
    baud = B1843200;
    initBaud = B9600;

    devnam = new char[usbDeviceName.length()+1];
    strcpy(devnam, usbDeviceName.c_str());

    filenam = new char[hexFileName.length()+1];
    strcpy(filenam, hexFileName.c_str());

    ifil = -1;
    devd = -1;
    tmod = 0;
    smod = 0;

    intBuf = 0;
    intBufState = -1;

    connect(this, SIGNAL(byteReceived(byte)), this, SLOT(combineBytesToInt(byte)), Qt::DirectConnection);
}

BFConnectionThread::~BFConnectionThread()
{
    // clean up
    if (ifil > 0)
        close(ifil);

    if (tmod)
        tcsetattr(devd, TCSANOW, &oldtio);

    if (devd > 0)
        close(devd);

    delete devnam;
    delete filenam;

    // terminate
    this->terminate();
    this->quit();
}

void BFConnectionThread::run()
{
    byte buf;

    // open hex file
    ifil = open(filenam, O_RDONLY);
    if (ifil < 0)
    {
        char causeStr[256];
        sprintf(causeStr, "open(%s)", filenam);
        die(causeStr, errno);
        return;
    }

    // open serial connection
    devd = open(devnam, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (devd < 0)
    {
        char causeStr[256];
        sprintf(causeStr, "open(%s)", devnam);
        die(causeStr, errno);
        return;
    }

    // setup initial tty
    if (initTTY(initBaud) < 0)
    {
        char causeStr[256];
        sprintf(causeStr, "BFConnectionThread::initTTY(%d)", initBaud);
        die(causeStr);
        return;
    }

    // communicate baud rate
    int bdiv = 6000000/bnum;
    if (changeBaud(bdiv) < 0)
    {
        char causeStr[256];
        sprintf(causeStr, "BFConnectionThread::changeBaud(%d)", bdiv);
        die(causeStr);
        return;
    }
    if (bnum != 9600)
    {
        if (initTTY(baud) < 0)
        {
            char causeStr[256];
            sprintf(causeStr, "BFConnectionThread::initTTY(%d)", baud);
            die(causeStr);
            return;
        }
    }

    // initialize SDRAM
    if (xram)
    {
        int xramBaudArg = 0;
        if (changeBaud(xramBaudArg) < 0)
        {
            char causeStr[256];
            sprintf(causeStr, "BFConnectionThread::changeBaud(%d)", xramBaudArg);
            die(causeStr);
            return;
        }
    }

    // transmit hex file
    while (read(ifil, &buf, 1) == 1)
    {
        txByte(buf);
        usleep(10000000/bnum);
    }
    usleep(100000);

    emit connectionEstablished();

    // enter read loop
    while (true)
    {
        if (readAvailable(devd, 100))
        {
            buf = rxByte();
            emit byteReceived(buf);
        }
    }
}

int BFConnectionThread::initTTY(int baud)
{
    memset(&newtio, 0, sizeof newtio);
    memset(&oldtio, 0, sizeof oldtio);

    if (tcgetattr(devd, &oldtio) < 0)
    {
        die("tcgetattr", errno);
        return -1;
    }

    if (cfsetspeed(&newtio, baud) < 0)
    {
        die("cfsetspeed", errno);
        return -1;
    }

    newtio.c_cflag      = baud | CS8 | CLOCAL | CREAD;
    newtio.c_iflag      = IGNPAR;
    newtio.c_oflag      = 0;
    newtio.c_cflag      = 0;
    newtio.c_cc[VMIN]   = 0;
    newtio.c_cc[VTIME]  = 0;

    if (tcsetattr(devd, TCSANOW, &newtio) < 0)
    {
        die("tcsetattr", errno);
        return -1;
    }
    tmod = 1;

    if (tcflush(devd, TCIOFLUSH) < 0)
    {
        die("tcflush", errno);
        return -1;
    }
    return 0;
}

void BFConnectionThread::die(const char *causingFunction, int errorNumber)
{
    // handle errors
    char errMsg[512];
    sprintf(errMsg, "%s failed", causingFunction);
    if (errorNumber > 0)
    {
        char* errStr = strerror(errorNumber);
        sprintf(errMsg, "%s (errno %d - %s)", errMsg, errorNumber, errStr);
    }
    std::string msg = errMsg;
    emit errorOccured(msg.c_str());
    std::cerr << "Error: " << errMsg << std::endl;

    // clean up
    if (ifil > 0)
        close(ifil);
    if (tmod)
        tcsetattr(devd, TCSANOW, &oldtio);
    if (devd > 0)
        close(devd);

    // terminate
    this->terminate();
    this->quit();
}

int BFConnectionThread::changeBaud(int bdiv)
{
    while (readAvailable(devd, 0))
        rxByte();

    txByte(':');
    txHex(0x00);
    txHex((bdiv >> 8) & 0xFF);
    txHex(bdiv & 0xFF);
    txHex(0x0F);
    txHex((0xF1 - ((bdiv >> 8) & 0xFF) - (bdiv & 0xFF)) & 0xFF);

    if (rxByte() != '!')
    {
        char causeStr[256];
        sprintf(causeStr, "BFConnectionThread::changeBaud(%d)", bdiv);
        die(causeStr, errno);
        return -1;
    }
    return 0;
}

int BFConnectionThread::writeAvailable(int fd, int millis)
{
    fd_set fdsout;
    struct timeval tv;
    FD_ZERO(&fdsout);
    FD_SET(fd, &fdsout);
    tv.tv_sec = millis / 1000;
    tv.tv_usec = (millis % 1000) * 1000;
    if (select(fd+1, NULL, &fdsout, NULL, &tv) <= 0) return 0;
    if (!(FD_ISSET(fd, &fdsout))) return 0;
    return 1;
}

int BFConnectionThread::readAvailable(int fd, int millis)
{
    fd_set fdsin;
    struct timeval tv;
    FD_ZERO(&fdsin);
    FD_SET(fd, &fdsin);
    tv.tv_sec = millis / 1000;
    tv.tv_usec = (millis % 1000) * 1000;
    if (select(fd+1, &fdsin, NULL, NULL, &tv) <= 0) return 0;
    if (!(FD_ISSET(fd, &fdsin))) return 0;
    return 1;
}

void BFConnectionThread::combineBytesToInt(byte bt)
{
    switch(intBufState)
    {
    case -1:    // await int transmission
        if (bt == SYNCBYTE)
            intBufState = 0;
        break;
    case 0:     // int transmission ready, buffer empty, byte goes to MSByte
        intBuf = bt << 24;
        intBufState = 1;
        break;
    case 1:     // byte goes to 2nd MSByte
        intBuf |= bt << 16;
        intBufState = 2;
        break;
    case 2:     // byte goes to 3rd MSByte
        intBuf |= bt << 8;
        intBufState = 3;
        break;
    case 3:     // byte goes to LSByte
        intBuf |= bt;
        emit intReceived(intBuf); // int complete
        intBuf = 0;
        intBufState = -1;
        break;
    }
}

byte BFConnectionThread::rxByte()
{
    unsigned char bt;
    if (!readAvailable(devd, 1000))
        die("BFConnectionThread::rxByte()", ETIMEDOUT);
    if (read(devd, &bt, 1) != 1)
        die("BFConnectionThread::rxByte()", errno);
    return bt;
}

void BFConnectionThread::txByte(byte bt)
{
    if (!writeAvailable(devd, 1000))
        die("BFConnectionThread::txByte()", ETIMEDOUT);
    if (write(devd, &bt, 1) != 1)
        die("BFConnectionThread::txByte()", errno);
}


void BFConnectionThread::txHex(byte bt)
{
    unsigned char bb;

    bb = (bt >> 4) & 0x0F;
    if (bb < 0x0A)
        txByte(bb + 48);
    else
        txByte(bb + 55);

    bb = bt & 0x0F;
    if (bb < 0x0A)
        txByte(bb + 48);
    else
        txByte(bb + 55);
}
