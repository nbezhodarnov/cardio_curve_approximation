#if !defined(_OLXDADEFS_)
#define _OLXDADEFS_

/*-----------------------------------------------------------------------

 Copyright  (C)  1997.   Data  Translation,  Inc.,  100  Locke   Drive,
 Marlboro Massachusetts 01752-1192.

 All rights reserved.  This software is furnished to purchaser under  a
 license  for  use  on a single computer system and can be copied (with
 the inclusion of DTI's copyright notice) only for use in such  system,
 except  as  may  be otherwise provided in writing by Data Translation,
 Inc., 100 Locke Drive, Marlboro, Massachusetts 01752-1192.

 The information in this document is subject to change  without  notice
 and  should not be construed as a commitment by Data Translation, Inc.
 Data Translation, Inc.  assumes no responsibility for any errors  that
 may appear in this document.

 Data Translation cannot assume any responsibility for the use  of  any
 portion  of  this  software  on  any  equipment  not  supplied by Data
 Translation, Inc.

FILE NAME: olxdadefs.h

ABSTRACT:

   This unit contains common declarations for the DT Open Layers 
   Data Acquisition OCX control.


06/25/97
---------------------------------------------------------------------------*/



typedef enum olss_tag
   {
   OLSS_AD,
   OLSS_DA,
   OLSS_DIN,
   OLSS_DOUT,
   OLSS_SRL,
   OLSS_CT
   }
OLSS;


typedef enum olssc_tag
   {
	OLSSC_MAXSECHANS,
	OLSSC_MAXDICHANS,
	OLSSC_CGLDEPTH,
	OLSSC_NUMFILTERS,
	OLSSC_NUMGAINS,
	OLSSC_NUMRANGES,
	OLSSC_NUMDMACHANS,
	OLSSC_NUMCHANNELS,
	OLSSC_NUMEXTRACLOCKS,
	OLSSC_NUMEXTRATRIGGERS,
	OLSSC_NUMRESOLUTIONS,

	OLSSC_SUP_INTERRUPT,
	OLSSC_SUP_SINGLEENDED,
	OLSSC_SUP_DIFFERENTIAL,
	OLSSC_SUP_BINARY,
	OLSSC_SUP_2SCOMP,
	OLSSC_SUP_SOFTTRIG,
	OLSSC_SUP_EXTERNTRIG,
	OLSSC_SUP_THRESHTRIGPOS,
	OLSSC_SUP_THRESHTRIGNEG,
	OLSSC_SUP_ANALOGEVENTTRIG,
	OLSSC_SUP_DIGITALEVENTTRIG,
	OLSSC_SUP_TIMEREVENTTRIG,
	OLSSC_SUP_TRIGSCAN,
	OLSSC_SUP_INTCLOCK,
	OLSSC_SUP_EXTCLOCK,
	OLSSC_SUP_SWCAL,
	OLSSC_SUP_EXP2896,
	OLSSC_SUP_EXP727,
	OLSSC_SUP_FILTERPERCHAN,
	OLSSC_SUP_DTCONNECT,
	OLSSC_SUP_FIFO,
	OLSSC_SUP_PROGRAMGAIN,
	OLSSC_SUP_PROCESSOR,
	OLSSC_SUP_SWRESOLUTION,
	OLSSC_SUP_CONTINUOUS,
	OLSSC_SUP_SINGLEVALUE,
	OLSSC_SUP_PAUSE,
	OLSSC_SUP_WRPMULTIPLE,
	OLSSC_SUP_WRPSINGLE,

	OLSSC_SUP_POSTMESSAGE,
	OLSSC_SUP_CASCADING,
	OLSSC_SUP_CTMODE_COUNT,
	OLSSC_SUP_CTMODE_RATE,
	OLSSC_SUP_CTMODE_ONESHOT,
	OLSSC_SUP_CTMODE_ONESHOT_RPT,

	OLSSC_MAX_DIGITALIOLIST_VALUE,
	OLSSC_SUP_DTCONNECT_CONTINUOUS,
	OLSSC_SUP_DTCONNECT_BURST,
	OLSSC_SUP_CHANNELLIST_INHIBIT,
	OLSSC_SUP_SYNCHRONOUS_DIGITALIO,
	OLSSC_SUP_SIMULTANEOUS_START,
	OLSSC_SUP_INPROCESSFLUSH,

	OLSSC_SUP_RANGEPERCHANNEL,
	OLSSC_SUP_SIMULTANEOUS_SH,
	OLSSC_SUP_RANDOM_CGL,
	OLSSC_SUP_SEQUENTIAL_CGL,
	OLSSC_SUP_ZEROSEQUENTIAL_CGL,

	OLSSC_SUP_GAPFREE_NODMA,
	OLSSC_SUP_GAPFREE_SINGLEDMA,
	OLSSC_SUP_GAPFREE_DUALDMA,

	OLSSCE_MAXTHROUGHPUT,
	OLSSCE_MINTHROUGHPUT,
	OLSSCE_MAXRETRIGGER,
	OLSSCE_MINRETRIGGER,
	OLSSCE_MAXCLOCKDIVIDER,
	OLSSCE_MINCLOCKDIVIDER,
	OLSSCE_BASECLOCK,

	OLSSCE_RANGELOW,
	OLSSCE_RANGEHIGH,
	OLSSCE_FILTER,
	OLSSCE_GAIN,
	OLSSCE_RESOLUTION, 

	OLSSC_SUP_PLS_HIGH2LOW,      
	OLSSC_SUP_PLS_LOW2HIGH,      

	OLSSC_SUP_GATE_NONE,         
	OLSSC_SUP_GATE_HIGH_LEVEL,
	OLSSC_SUP_GATE_LOW_LEVEL,    
	OLSSC_SUP_GATE_HIGH_EDGE,    
	OLSSC_SUP_GATE_LOW_EDGE,     
	OLSSC_SUP_GATE_LEVEL,     
	OLSSC_SUP_GATE_HIGH_LEVEL_DEBOUNCE,
	OLSSC_SUP_GATE_LOW_LEVEL_DEBOUNCE,    
	OLSSC_SUP_GATE_HIGH_EDGE_DEBOUNCE,    
	OLSSC_SUP_GATE_LOW_EDGE_DEBOUNCE,     
	OLSSC_SUP_GATE_LEVEL_DEBOUNCE,

	OLSS_SUP_RETRIGGER_INTERNAL,
	OLSS_SUP_RETRIGGER_SCAN_PER_TRIGGER,

	OLSSC_MAXMULTISCAN,	
	OLSSC_SUP_CONTINUOUS_PRETRIG,
	OLSSC_SUP_CONTINUOUS_ABOUTTRIG, 
	OLSSC_SUP_BUFFERING,
	OLSSC_SUP_RETRIGGER_EXTRA,

	OLSSC_NONCONTIGUOUS_CHANNELNUM,
	OLSSC_SUP_SINGLEVALUE_AUTORANGE,
	OLSSC_SUP_CTMODE_UP_DOWN,
	OLSSC_SUP_CTMODE_MEASURE,
	OLSSC_SUP_WRPWAVEFORM,
	OLSSC_FIFO_SIZE_IN_K,
	OLSSC_SUP_SIMULTANEOUS_CLOCKING,
	OLSSC_FIXED_PULSE_WIDTH,
	OLSSC_QUADRATURE_DECODER,
    OLSSC_SUP_CTMODE_CONT_MEASURE,
}
OLSSC;


#define OLSSCMIN      OLSSC_MAXSECHANS
#define OLSSCMAX      OLSSC_SUP_GAPFREE_DUALDMA
#define OLSSCCOUNT    (OLSSCMAX - OLSSCMIN + 1)

#define OLSSCEMIN     OLSSCE_MAXTHROUGHPUT
#define OLSSCEMAX     OLSSCE_BASECLOCK 
#define OLSSCECOUNT   (OLSSCEMAX - OLSSCEMIN + 1)
#define OLSSCEOFFSET  (OLSSCE_RESOLUTION - OLSSCEMIN +1)

#define OLSSCNMIN     OLSSC_SUP_PLS_HIGH2LOW
#define OLSSCNMAX     OLSSC_SUP_CTMODE_CONT_MEASURE 
#define OLSSCNCOUNT   (OLSSCNMAX - OLSSCNMIN + 1)


typedef enum oldc_tag
   {
   OLDC_ADELEMENTS,
   OLDC_DAELEMENTS,
   OLDC_DINELEMENTS,
   OLDC_DOUTELEMENTS,
   OLDC_SRLELEMENTS,
   OLDC_CTELEMENTS,
   OLDC_TOTALELEMENTS=~0
   }
OLDC;

#define OLDCMIN      OLDC_ADELEMENTS
#define OLDCMAX      OLDC_CTELEMENTS
#define OLDCOUNT     (OLDCMAX - OLDCMIN) + 1

/* for OLDRV_SETCHANNELTYPE         */
#define OLx_CHNT_SINGLEENDED         0
#define OLx_CHNT_DIFFERENTIAL        1

/* for Encoding property            */
#define OLx_ENC_BINARY               0
#define OLx_ENC_2SCOMP                1

/* for trigger property             */
#define OLx_TRG_SOFT                 0
#define OLx_TRG_EXTERN               1
#define OLx_TRG_THRESHPOS            2
#define OLx_TRG_THRESHNEG            3
#define OLx_TRG_ANALOGEVENT          4
#define OLx_TRG_DIGITALEVENT         5
#define OLx_TRG_TIMEREVENT           6
#define OLx_TRG_EXTRA                7

/* for ClockSource property         */
#define OLx_CLK_INTERNAL             0
#define OLx_CLK_EXTERNAL             1
#define OLx_CLK_EXTRA                2

/* for GateType property            */
#define OLx_GATE_NONE                0
#define OLx_GATE_HIGH_LEVEL          1
#define OLx_GATE_LOW_LEVEL           2
#define OLx_GATE_HIGH_EDGE           3
#define OLx_GATE_LOW_EDGE            4
#define OLx_GATE_LEVEL               5
#define OLx_GATE_HIGH_LEVEL_DEBOUNCE 6
#define OLx_GATE_LOW_LEVEL_DEBOUNCE  7
#define OLx_GATE_HIGH_EDGE_DEBOUNCE  8
#define OLx_GATE_LOW_EDGE_DEBOUNCE   9
#define OLx_GATE_LEVEL_DEBOUNCE      10

/* for PulseType property           */
#define OLx_PLS_HIGH2LOW             0
#define OLx_PLS_LOW2HIGH             1

/* for CTMode property         */
#define OLx_CTMODE_COUNT             0
#define OLx_CTMODE_RATE              1
#define OLx_CTMODE_ONESHOT           2
#define OLx_CTMODE_ONESHOT_RPT       3
#define OLx_CTMODE_UP_DOWN			 4
#define OLx_CTMODE_MEASURE			 5
#define OLx_CTMODE_CONT_MEASURE		 6

/* for MeasureStartEdge, MeasureStopEdge */
#define OLx_GATE_RISING				 0
#define OLx_GATE_FALLING			 1
#define OLx_CLOCK_RISING			 2
#define OLx_CLOCK_FALLING			 3

/* for DataFlow property */
#define OLx_DF_CONTINUOUS            0   
#define OLx_DF_SINGLEVALUE           1
#define OLx_DF_DTCONNECT_CONTINUOUS  2   
#define OLx_DF_DTCONNECT_BURST       3   
#define OLx_DF_CONTINUOUS_PRETRIG     4   
#define OLx_DF_CONTINUOUS_ABOUTTRIG   5   

/* for CascadeMode Property		*/
#define OLx_CT_CASCADE               0
#define OLx_CT_SINGLE                1

/* for WrapMode Property		*/
#define OLx_WRP_NONE                 0
#define OLx_WRP_MULTIPLE             1
#define OLx_WRP_SINGLE               2

/* for QueueSize Property		*/
#define OLx_QUE_READY                0
#define OLx_QUE_DONE                 1
#define OLx_QUE_INPROCESS            2


/* for RetriggerMode Property		*/
#define OLx_RETRIGGER_INTERNAL          0
#define OLx_RETRIGGER_SCAN_PER_TRIGGER  1
#define OLx_RETRIGGER_EXTRA			    2


#define OLDA_WM_TRIGGER_ERROR     (WM_USER+100)
#define OLDA_WM_UNDERRUN_ERROR    (WM_USER+101)
#define OLDA_WM_OVERRUN_ERROR     (WM_USER+102)
#define OLDA_WM_BUFFER_DONE       (WM_USER+103)
#define OLDA_WM_QUEUE_DONE        (WM_USER+104)
#define OLDA_WM_BUFFER_REUSED     (WM_USER+105)
#define OLDA_WM_QUEUE_STOPPED     (WM_USER+106)
#define OLDA_WM_EVENT_ERROR       (WM_USER+107)
#define OLDA_WM_MEASURE_DONE      (WM_USER+108)
#define OLDA_WM_DTCONNECT_DONE    (WM_USER+109)
#define OLDA_WM_DTCONNECT_STOPPED (WM_USER+110)
#define OLDA_WM_EVENT_DONE        (WM_USER+111)
#define OLDA_WM_PRETRIGGER_BUFFER_DONE  (WM_USER+112)
#define OLDA_WM_DEVICE_REMOVAL    (WM_USER+113)
#define OLDA_WM_IO_COMPLETE		  (WM_USER+114)


// Definitions of bits within OLSSC_SUP_CTMODE_MEASURE that indicate which
// types of measurement is supported on a given board
#define SUP_GATE_RISING_BIT			0x01
#define SUP_GATE_FALLING_BIT		0x02
#define SUP_CLOCK_RISING_BIT		0x04
#define SUP_CLOCK_FALLING_BIT		0x08
   
#define DTACQ32_ERRORBASE  22010
#endif



