#include "daqdeviceinterface.h"

QStringList DAQDeviceInterface::getBoardsList() {
    QStringList boards;
    if (olDaEnumBoards(AddBoardList, (LPARAM)&boards) != OLNOERROR) {
        DisplayErrorMessage("Ошибка при чтении информации об устройствах!");
        return QStringList();
    }
    return boards;
}

bool DAQDeviceInterface::connect(const QString &boardName) {
    char *name = new char[80];
    QByteArray str = boardName.toUtf8();
    for (unsigned int i = 0; i < (unsigned int)str.size(); i++) {
        if (str[i] == ' ') {
            name[i] = '\0';
            break;
        }
        name[i] = str[i];
    }
    if (olDaInitialize((wchar_t*)name, &board.hdrvr) != OLNOERROR) {
        DisplayErrorMessage("Ошибка при подключении к устройству!");
        delete []name;
        return false;
    }
    delete []name;

    unsigned int numberADs, dma;
    QStringList gains, ranges, filters;

    if (olDaGetDevCaps(board.hdrvr, OLDC_ADELEMENTS, &numberADs) != OLNOERROR) {
        DisplayErrorMessage("Ошибка при запросе доступа к A/D подсистеме!");
        return false;
    }
    if (olDaGetDASS(board.hdrvr, OLSS_AD, 0, &board.hdass) != OLNOERROR) {
        DisplayErrorMessage("Ошибка при подключении к A/D подсистеме!");
        return false;
    }
    unsigned int ssdata;
    if ((olDaSetChannelType(board.hdass, OL_CHNT_DIFFERENTIAL) != OLNOERROR) || (olDaGetSSCapsEx(board.hdass, OLSSCE_MAXTHROUGHPUT, &frequency) != OLNOERROR) || (olDaGetSSCaps(board.hdass, OLSSC_NUMDMACHANS, &dma) != OLNOERROR) || (olDaGetSSCaps(board.hdass, OLSSC_SUP_PROGRAMGAIN, &gainsup) != OLNOERROR) || (olDaGetSSCaps(board.hdass, OLSSC_NUMCHANNELS, &channelsCount) != OLNOERROR) || (olDaGetEncoding(board.hdass, &encoding) != OLNOERROR) || (olDaGetSSCaps(board.hdass, OLSSC_RETURNS_FLOATS, &bReturnsFloats) != OLNOERROR) || (olDaGetTrigger(board.hdass, &ssdata) != OLNOERROR) || (olDaSetTrigger(board.hdass, ssdata) != OLNOERROR) || (olDaGetClockSource(board.hdass, &ssdata) != OLNOERROR) || (olDaSetClockSource(board.hdass, ssdata) != OLNOERROR) || (olDaGetEncoding(board.hdass, &ssdata) != OLNOERROR) || (olDaSetEncoding(board.hdass, ssdata) != OLNOERROR)) {
        DisplayErrorMessage("Ошибка при чтении параметров устройства!");
        return false;
    }

    if (dma > 1) {
        dma = 1;
    }
    if (frequency > 1000.0) {
        frequency = 1000.0;
    }

    unsigned int cap;
    if ((olDaSetDataFlow(board.hdass, OL_DF_CONTINUOUS) != OLNOERROR) || (olDaSetWrapMode(board.hdass, OL_WRP_NONE) != OLNOERROR) || ((olDaGetSSCaps( board.hdass, OLSSC_SUP_TRIGSCAN, &cap ) != OLNOERROR) || (cap && olDaSetRetriggerFrequency(board.hdass, 10.0) != OLNOERROR)) || (olDaSetClockFrequency(board.hdass, frequency) != OLNOERROR) || (olDaSetDmaUsage(board.hdass, dma) != OLNOERROR)) {
        DisplayErrorMessage("Ошибка при выставлении параметров устройства!");
        return false;
    }
    if (olDaConfig(board.hdass) != OLNOERROR) {
        DisplayErrorMessage("Ошибка при конфигурировании устройства!");
        return false;
    }
    return true;
}

unsigned int DAQDeviceInterface::getChannelsCount() {
    return channelsCount;
}

QStringList DAQDeviceInterface::getGainsList() {
    QStringList gains;
    if (olDaEnumSSCaps(board.hdass, OL_ENUM_GAINS, AddGainList, (LPARAM)&gains) != OLNOERROR) {
        return QStringList();
    }
    return gains;
}

QStringList DAQDeviceInterface::getRangesList() {
    QStringList ranges;
    if (olDaEnumSSCaps(board.hdass, OL_ENUM_RANGES, AddRangeList, (LPARAM)&ranges) != OLNOERROR) {
        return QStringList();
    }
    return ranges;
}

QStringList DAQDeviceInterface::getFiltersList() {
    QStringList filters;
    if (olDaEnumSSCaps(board.hdass, OL_ENUM_FILTERS, AddFilterList, (LPARAM)&filters) != OLNOERROR) {
        return QStringList();
    }
    return filters;
}

double DAQDeviceInterface::getFrequency() {
    return frequency;
}

Range DAQDeviceInterface::getRange() {
    return rangeBoard;
}

bool DAQDeviceInterface::setParameters(unsigned int channel, double gain, Range range, unsigned int filter) {
    olDaSetEncoding( board.hdass, OL_ENC_BINARY );
    if (((gainsup) && ((gain <= 0.0) || (olDaSetGainListEntry(board.hdass, 0, gain) != OLNOERROR))) || ((olDaSetChannelListEntry(board.hdass, 0, channel) != OLNOERROR) || olDaSetChannelListSize(board.hdass, 1) != OLNOERROR) || (olDaSetChannelFilter(board.hdass, 0, filter) != OLNOERROR) || (olDaSetRange(board.hdass, range.max, range.min) != OLNOERROR)) {
        DisplayErrorMessage("Ошибка при выставлении параметров устройства!");
        return false;
    }
    if (olDaConfig(board.hdass) != OLNOERROR) {
        DisplayErrorMessage("Ошибка при конфигурировании устройства!");
        return false;
    }
    rangeBoard = range;
    gainBoard = gain;
    return true;
}

bool DAQDeviceInterface::start() {
    if (board.hbuf != NULL) {
        olDmFreeBuffer(board.hbuf);
        board.hbuf = NULL;
    }
    unsigned int bufferbytes;

    if ((olDaGetResolution(board.hdass,&bufferbytes) != OLNOERROR) || (olDaGetChannelListSize(board.hdass, &listsize) != OLNOERROR)) {
        DisplayErrorMessage("Ошибка настройки чтения данных с устройства!");
        return false;
    }
    if (bufferbytes > 16)
        resolution = 4;
    else
        resolution = 2;

    unsigned int buffer_size = frequency / 10;
    for (unsigned int i=0; i < NUM_BUFFERS; i++ ) {
        olDmCallocBuffer(0, 0, buffer_size, bufferbytes, &board.hbuf);
        olDaPutBuffer(board.hdass, board.hbuf);
    }

    if ((olDmCallocBuffer(0, 0, buffer_size / 2, bufferbytes, &board.hbuf) != OLNOERROR) || (olDaStart(board.hdass) != OLNOERROR)) {
        DisplayErrorMessage("Ошибка запуска чтения данных!");
        return false;
    }
    x = 0;
    return true;
}

QList<FunctionElement> DAQDeviceInterface::getData() {
    unsigned long bufsize;
    unsigned char *wave;
    //board.hbuf = NULL;
    if ((olDaGetBuffer(board.hdass, &board.hbuf) != OLNOERROR) || (board.hbuf == NULL) || (olDmGetValidSamples(board.hbuf, &bufsize) != OLNOERROR) || (bufsize == 0) || (olDmGetBufferPtr(board.hbuf, (void**)&wave) != OLNOERROR)) {
        //DisplayErrorMessage("Ошибки при чтении данных с устройства!");
        return QList<FunctionElement>();
    }

    QList<FunctionElement> result;
    double step = 1.0 / frequency;
    long res = 1L << resolution;
    double amp = ((rangeBoard.max / gainBoard - rangeBoard.min / gainBoard) / res);

    if (bReturnsFloats) {
        float* pFloatData;
        pFloatData = (float*)wave;
        for (unsigned int i = 0; i < bufsize; i++, x += step) {
            result.append({x, pFloatData[i]});
        }
    } else if (encoding == OL_ENC_BINARY) {
        for (unsigned int i = 0; i < bufsize * resolution; i = i + (resolution * listsize), x += step) {
            long tempdata = 0;
            for (unsigned int k = resolution - 1; k > 0; k--) {
                tempdata = tempdata + (unsigned char)wave[i + k];
                tempdata <<= 8;
            }
            tempdata = tempdata + (unsigned char)wave[i];

            if (tempdata < 0) {
                result.append({x, (tempdata + res) * amp + (rangeBoard.min / gainBoard)});
            } else {
                result.append({x, tempdata * amp + (rangeBoard.min / gainBoard)});
            }
        }
    } else {
        for (unsigned int i = 0; i < bufsize * resolution; i = i + (resolution * listsize), x += step) {
            unsigned long tempdata = 0;
            for (unsigned int k = resolution - 1; k > 0; k--) {
                tempdata = tempdata + (unsigned char)wave[i + k];
                tempdata<<=8;
            }
            tempdata = tempdata + (unsigned char)wave[i];

            tempdata ^= 1L << (resolution - 1);
            tempdata &= (1L << resolution) - 1;

            result.append({x, ((rangeBoard.max / gainBoard) - (rangeBoard.min / gainBoard)) / res * tempdata + (rangeBoard.min / gainBoard)});
        }
    }
    if (olDaPutBuffer(board.hdass, board.hbuf) != OLNOERROR) {
        DisplayErrorMessage("Ошибки при чтении данных с устройства!");
    }
    return result;
}

void DAQDeviceInterface::abort() {
    Hbuf hbuf;
    olDaAbort(board.hdass);
    olDmFreeBuffer(board.hbuf);
    olDaFlushBuffers(board.hdass);
    while ((olDaGetBuffer(board.hdass, &hbuf) == OLNOERROR) && (hbuf != NULL)) {
        olDmFreeBuffer(hbuf);
    }
}

void DAQDeviceInterface::stop() {
    Hbuf hbuf;
    olDaStop(board.hdass);
    olDaFlushBuffers(board.hdass);
    for (unsigned int i = 0; i < NUM_BUFFERS; i++) {
        olDaGetBuffer(board.hdass, &hbuf);
        olDmFreeBuffer(hbuf);
    }
}

void DAQDeviceInterface::disconnect() {
    olDaReleaseDASS(board.hdass);
    olDaTerminate(board.hdrvr);
}
