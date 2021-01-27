#ifndef DAQDEVICEFUNCTIONEXTRACTIONRESOURCES_H
#define DAQDEVICEFUNCTIONEXTRACTIONRESOURCES_H

#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldaapi.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oltypes.h"

#ifndef RC_INVOKED

#ifdef _WIN32
#define EXPORT WINAPI
#else
#define EXPORT __export FAR PASCAL    // used for callback funtions
#endif

#ifdef _WIN32
#define _huge
#endif

#define MAXOUTPUTSIZE 10000
#define MAXPLOTSIZE 10000
typedef struct config {
    HWND    hwnd;         // handle to top level window
    char    BoardName[MAX_BOARD_NAME_LENGTH];
    BOOL    bUseWinMsgs;  // flag to determine WinMsgs or Callbacks
    HDEV    hDev;         // board handle
    HDASS   hAdc;         // ADC subsystem handle
    HDASS   hDac;         // DAC subsystem handle
    HDASS   hCT0;         // Counter Timer 0 subsystem handle
    HDASS	hDigIn0;
    HDASS	hDigIn1;
    HDASS	hDigIn2;
    HDASS	hDigIn3;
    HDASS	hDigOut0;
    HDASS	hDigOut1;
    HDASS	hDigOut2;
    HDASS	hDigOut3;
    HBUF    hFlushBuf;
    BOOL    fCTRunning;   // TRUE if CT is currently running
    UINT    uiCTModeItem; // item number of CT mode combo box
    BOOL    fScanEnable;  // TRUE if triggered scan is used for input
    DBL     dScanFreq;    // Scan trigger frequency
    BOOL    fDisableIn;   // disable input if TRUE
    BOOL    fDisplay;     // display input data if TRUE
    BOOL    fFileWrite;   // write input data to file if TRUE
    UINT    uiDataFlow;   // one of: IDD_BURST or IDD_CONTINUOUS
    DBL     dInFreq;      // input sample freq
    UINT    uiNumBuf;     // number of input buffers
    ULNG    ulBufSize;    // size of each input buffer
    UINT    uiInDma;      // number of input dma channels
    BOOL    fDisableOut;  // disable output if true
    UINT    uiOutDma;     // output dma channels (0 or 1)
    UINT    uiWaveType;   // output wave type, one of: IDD_SINE, IDD_SQUARE, or IDD_TRIANGLE
    DBL     dPeakVolts;   // peak AC amp of output
    DBL     dWaveFreq;    // frequency of output signal
    DBL     dOutFreq;     // output sample rate
    long    lOutBufSize;  // output buffer size
    HBUF    hAdcBuf;      // handle to current input buffer
    HBUF    hDacBuf;      // handle to current output buffer
    BOOL    fViewDac;     // paint DAC trace if TRUE, otherwise paint ADC
    long    lAdcBufsDone; // number of input buffers transferred (this run)
    long    lDacBufsDone; // number of output buffers transferred (this run)
    UINT    uiOldWaveSize;// size of previous display trace (in samples)
    RECT    GraphRect;    // area inwhich to draw graph
    RECT    TextRect;     // area  inwhich to paint text
    UINT    uiCTStartEdge;  // Start Edge for Measure mode
    UINT    uiCTStopEdge;   // Stop Edge for Measure mode
    DBL     dCtrClockFreq;	// Counter's internal clock frequency
    BOOL    fFileWriteText;
} Config;

typedef Config       CONFIG;
typedef CONFIG FAR * LPCONFIG;

#endif  // RC_INVOKED

typedef struct tag_board {
   HDEV  hdrvr;        /* driver handle            */
   HDASS hdass;        /* sub system handle        */
   ECODE status;       /* board error status       */
   HBUF  hbuf;         /* sub system buffer handle */
   PWORD lpbuf;        /* buffer pointer           */
   char name[MAX_BOARD_NAME_LENGTH];  /* string for board name    */
   char entry[MAX_BOARD_NAME_LENGTH]; /* string for board name    */
} Board;

typedef Board* LPBOARD;

typedef HBUF Hbuf;

struct Range {
    double min;
    double max;
};

int DisplayErrorMessage(const char*);

BOOL CALLBACK AddBoardList(PTSTR lpszName, PTSTR lpszEntry, LPARAM lParam);
BOOL CALLBACK AddRangeList(UINT cap, DBL max, DBL min, LPARAM lParam);
BOOL CALLBACK AddGainList(UINT cap, DBL gain, DBL unused, LPARAM lParam);
BOOL CALLBACK AddFilterList(UINT cap, DBL filter, DBL unused, LPARAM lParam);

BOOL InitConfig(LPCONFIG lpconfig);
BOOL CALLBACK InitBoard(LPSTR lpszName, LPSTR lpszEntry, LPARAM lpconfig);
ECODE UpdateConfig(LPCONFIG lpconfig,LPSTR lpszBoardName);
VOID CloseBoard(LPCONFIG lpconfig);

// resource defines

#define IDM_BOARD        101
#define IDM_INPUT        102
#define IDM_OUTPUT       103
#define IDM_ACQ          104
#define IDM_CGL          105
#define IDM_START        110
#define IDM_STOP         111
#define IDM_SV           120
#define IDM_ANA_IO       121
#define IDM_DIG_IO       122
#define IDM_CT           130
#define IDM_VIEW         140
#define IDM_ABOUT        150
#define IDM_USEWINMSGS   160
#define IDM_USECALLBACKS 170
#define IDM_FLUSHINPROCESS 180

#define BOARDBOX 200
#define IDD_BOARD 201
#define ABOUTBOX 203
#define IDD_APIVERSION 250
#define IDD_DRVVERSION 251

#define ACQBOX                      300
#define IDD_DISABLEIN               301
#define IDD_DISPLAY                 302
#define IDD_FILEWRITE               303
#define IDD_BURST                   304
#define IDD_CONTINUOUS              305
#define IDD_INFREQ                  306
#define IDD_NUMBUF                  307
#define IDD_BUFSIZE                 308
#define IDD_INDMA                   309
#define IDD_PEAK                    310
#define IDD_WAVEFREQ                311
#define IDD_OUTFREQ                 312
#define IDD_DISABLEOUT              313
#define IDD_OUTDMA                  314
#define IDD_SINE                    316
#define IDD_SQUARE                  317
#define IDD_TRIANGLE                318

#define LABEL_IN1                   350
#define LABEL_IN2                   351
#define LABEL_IN3                   352
#define LABEL_IN4                   353
#define LABEL_IN5                   354
#define LABEL_IN6                   355

#define LABEL_OUT1                  360
#define LABEL_OUT2                  361
#define LABEL_OUT3                  362
#define LABEL_OUT4                  363
#define LABEL_IO1                   370
#define LABEL_IO2                   371

#define IOBOX                       400
#define IDD_CLK_INTERNAL            403
#define IDD_SINGLEENDED             401
#define IDD_DIFFERENTIAL            402
#define IDD_BINARY                  408
#define IDD_2SCOMP                  409
#define IDD_SCANENABLE              410
#define IDD_CLK_EXTERNAL            404
#define IDD_SCANFREQ                411
#define IDD_RANGE                   407
#define IDD_TRG_INTERNAL            405
#define IDD_TRG_EXTERNAL            406
#define IDD_TRG_DIGITAL             412

#define CGLBOX                      500
#define IDD_CGLLIST                 501
#define IDD_CGLSCROLL               502
#define IDD_CGLSIZE                 504
#define IDD_CGLENTRY                503
#define IDD_CGLCHANNEL              505
#define IDD_CGLGAIN                 506
#define IDD_RESET                   507
#define IDD_ADDENTRY                509
#define IDD_CGLFILTER               510

#define SVBOX                       600
#define IDD_SVINCHANNEL             601
#define IDD_SVINGAIN                602
#define IDD_SVGET                   603
#define IDD_SVVALUE                 604
#define IDD_SVOUTCHANNEL            605
#define IDD_SVOUTGAIN               606
#define IDD_SVPUT                   607

#define CTBOX                       700
#define IDD_CTMODE                  701
#define IDD_CTFREQ                  702
#define IDD_CTSTART                 704
#define IDD_CTSTOP                  705
#define IDD_FREQSTR                 703
#define IDD_CTWIDTH                 706
#define IDD_WIDTHSTR                707
#define IDD_EVENT                   708
#define IDD_EVENTSTR                709
#define IDD_CTREAD                  710
#define IDD_START_EDGE              711
#define IDD_STOP_EDGE               712
#define IDD_START_EDGE_TITLE        713
#define IDD_STOP_EDGE_TITLE         714
#define IDD_MEASURE_FREQ			715
#define IDD_MEASURE_FREQ_STR		716
#define IDD_MEASURE_PERIOD			717
#define IDD_MEASURE_PERIOD_STR		718


#define DIGIOBOX                        800
#define IDC_ELEMENT_IN                  801
#define IDC_ELEMENT_OUT                 802
#define IDC_GET                         803
#define IDC_PUT                         804
#define IDC_RESOLUTION                  805
#define IDC_VALUE                       806

#define FREQ_FORMAT "%.1lf"     // format used of printing frequency numbers
#define RANGE_FORMAT "%.1lfV - %.1lfV" // format used for printing input/output ranges
#define RANGE2_FORMAT "%.3lfV - %.3lfV" // format used for printing input/output ranges
#define GAIN_FORMAT "%.1lf"     // format used for printing gain numbers
#define FILTER_FORMAT "%.1lfHz"     // format used for printing gain numbers
#define WIDTH_FORMAT "%.1lf"     // format used for printing pulse width numbers
#define MAXCGLSIZE 600          // max CGL size this app can handle

#define NUM_BUFFERS 4

#endif // DAQDEVICEFUNCTIONEXTRACTIONRESOURCES_H
