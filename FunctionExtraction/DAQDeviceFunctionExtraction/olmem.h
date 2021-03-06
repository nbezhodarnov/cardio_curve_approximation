
#if !defined(_OLMEM_)
#define _OLMEM_

/*-----------------------------------------------------------------------

FILE NAME: olmem.h

ABSTRACT:

   This unit contains declarations for the DT Open Layers Memory Manager


---------------------------------------------------------------------------*/

#include "oltypes.h"

typedef struct tagTDS 
   {
   WORD  wYear;
   WORD  wMonth;
   WORD  wDay;
   WORD  wHour;
   WORD  wMin;
   WORD  wSec;
   } 
TDS;

typedef TDS FAR* LPTDS;


/*********************** BCB's **********************/

DECLARE_HANDLE(HBUF);

typedef HBUF FAR* LPHBUF;
typedef HBUF* PHBUF;

#ifdef __cplusplus
extern "C" {
#endif


#ifndef _OLACI_        // if oldaaci module is included - do NOT
                       // redefine these routines!!!  The names 
                       // are the same as V0.9

ECODE WINAPI olDmAllocBuffer (UINT, DWORD, LPHBUF);
ECODE WINAPI olDmFreeBuffer (HBUF);
ECODE WINAPI olDmReAllocBuffer (UINT, DWORD, LPHBUF);
ECODE WINAPI olDmGetTimeDateStamp (HBUF, LPTDS);
ECODE WINAPI olDmGetBufferPtr (HBUF, LPVOID FAR*);
ECODE WINAPI olDmGetBufferECode (HBUF, LPECODE);
ECODE WINAPI olDmGetBufferSize (HBUF, DWORD FAR*);
ECODE WINAPI olDmGetVersion (LPSTR);
ECODE WINAPI olDmCopyBuffer (HBUF, LPVOID);
ECODE WINAPI olDmCopyToBuffer (HBUF hBuf, LPVOID lpAppBuffer, ULNG ulNumSamples);

#endif

ECODE WINAPI olDmCallocBuffer (UINT, UINT, DWORD, UINT, LPHBUF);
ECODE WINAPI olDmMallocBuffer (UINT, UINT, DWORD, LPHBUF);
ECODE WINAPI olDmLockBuffer (HBUF);
ECODE WINAPI olDmUnlockBuffer (HBUF);
ECODE WINAPI olDmReCallocBuffer (UINT, UINT, DWORD, UINT, LPHBUF);
ECODE WINAPI olDmReMallocBuffer (UINT, UINT, DWORD, LPHBUF);
ECODE WINAPI olDmGetDataBits (HBUF, UINT FAR*);
ECODE WINAPI olDmSetDataWidth (HBUF, UINT);
ECODE WINAPI olDmGetDataWidth (HBUF, UINT FAR*);
ECODE WINAPI olDmGetMaxSamples (HBUF, DWORD FAR*);
ECODE WINAPI olDmSetValidSamples (HBUF, DWORD);
ECODE WINAPI olDmGetValidSamples (HBUF, DWORD FAR*);
ECODE WINAPI olDmCopyFromBuffer(HBUF hBuf, LPVOID lpAppBuffer, ULNG ulMaxSamples);
   
ECODE WINAPI olDmGetExtraBytes (HBUF hBuf, ULNG FAR *lpulExtra1,ULNG FAR *lpulExtra2);
ECODE WINAPI olDmSetExtraBytes (HBUF hBuf, ULNG ulExtra1, ULNG ulExtra2);

ECODE WINAPI olDmLockBufferEx (HBUF hBuf, BOOL fEnableScatter);
ECODE WINAPI olDmUnlockBufferEx (HBUF hBuf, BOOL fEnableScatter);

ECODE WINAPI drvDmSetCurrentTDS (HBUF hBuf);
ECODE WINAPI drvDmSetDataBits (HBUF hBuf, UINT Bits);

/*********************** BTL's **********************/


DECLARE_HANDLE(HLIST);

typedef HLIST FAR* LPHLIST;
typedef HLIST* PHLIST;

typedef BOOL (CALLBACK* BUFPROC)(HBUF, LPARAM);
typedef BOOL (CALLBACK* LISTPROC)(HLIST, LPARAM);

#ifndef WIN32
typedef BOOL (__far __pascal * INTPROC)(LPARAM);    // must be FIXED
#endif

ECODE WINAPI drvDmPutBufferToListForDriver (HLIST, HBUF);
ECODE WINAPI drvDmGetBufferFromListForDriver (HLIST, LPHBUF);
VOID  WINAPI drvDmLockBufferList( HLIST hList );
VOID  WINAPI drvDmUnLockBufferList( HLIST hList );

ECODE WINAPI olDmCreateList (LPHLIST, UINT, LPCSTR, LPCSTR);
ECODE WINAPI olDmEnumLists (LISTPROC, LPARAM);
ECODE WINAPI olDmEnumBuffers (HLIST, BUFPROC, LPARAM);
ECODE WINAPI olDmFreeList (HLIST);
ECODE WINAPI olDmPutBufferToList (HLIST, HBUF);
ECODE WINAPI olDmGetBufferFromList (HLIST, LPHBUF);
ECODE WINAPI olDmPeekBufferFromList (HLIST, LPHBUF);
ECODE WINAPI olDmGetListCount (HLIST, UINT FAR*);
ECODE WINAPI olDmGetListHandle (HBUF, LPHLIST);
ECODE WINAPI olDmGetListIds (HLIST, LPSTR, UINT, LPSTR, UINT);

ECODE WINAPI olDmLockBufferEx (HBUF hBuf, BOOL fEnableScatter);
ECODE WINAPI olDmUnLockBufferEx (HBUF hBuf, BOOL fEnableScatter);

LPSTR WINAPI olDmGetErrorString (ECODE eCode, LPSTR lpStr, UINT uiMaxSize);

#ifdef __cplusplus
}
#endif

                        
#endif








