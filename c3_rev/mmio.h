#ifndef __MMIO_INCLUDE__
#define __MMIO_INCLUDE__

#include "windows.h"

int mmio_loadFile(const char *pszFileName, HMMIO *outMmio, void **out_a3, const MMCKINFO *pmmckiParent);

#endif
