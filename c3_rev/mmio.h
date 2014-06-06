#ifndef __MMIO_INCLUDE__
#define __MMIO_INCLUDE__

#include "windows.h"

int mmio_loadFile(const char *pszFileName, HMMIO *outMmio, void **out_a3, const MMCKINFO *pmmckiParent);
int mmio_530B40(HMMIO hmmio, int length1, char* a3, int a4, int a5);
int mmio_close(HMMIO *mmio, HGLOBAL *a2);
int cvtdate(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);
void __cdecl fun_writeLogMessageToStatusTxt(DWORD nNumberOfBytesToWrite);
void fun_logDebugMessage(const char *msg, const char *paramStr, int paramInt);
int fun_chdirToCdBasedOnExtension(const char *filename);
void fun_storeFileExtension(const char *filename);
void fun_strncpy(const char *src, char *dst, int len);
void fun_strtoupper(char* a1);
void fun_getGameTextString(int group, int number);
void fun_drawHelpDialogWithVideo();


void fun_chdirHome();
void fun_getTimeAsString();
int fun_intToString(signed int number, int offset, int addPlusSign);

#endif
