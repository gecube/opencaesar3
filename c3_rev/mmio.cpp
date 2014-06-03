#include "mmio.h"

int mmio_loadFile(const char *pszFileName, HMMIO *outMmio, void **out_a3, const MMCKINFO *pmmckiParent)
{
  LONG v4; // eax@23
  signed int resultCode; // [sp+4Ch] [bp-30h]@3
  int v7; // [sp+50h] [bp-2Ch]@17
  char Src[4]; // [sp+54h] [bp-28h]@14
  struct _MMCKINFO pmmcki; // [sp+64h] [bp-18h]@10
  HMMIO hmmio; // [sp+78h] [bp-4h]@1

  *out_a3 = 0;
  hmmio = mmioOpenA((LPSTR)pszFileName, 0, 0x10000u);
  if ( !hmmio )
  {
    j_fun_chdirToCdBasedOnExtension(pszFileName);
    hmmio = mmioOpenA((LPSTR)pszFileName, 0, 0x10000u);
    if ( !hmmio )
    {
      resultCode = 1;
      j_fun_chdirHome();
      goto LABEL_28;
    }
    j_fun_chdirHome();
  }
  if ( mmioDescend(hmmio, (LPMMCKINFO)pmmckiParent, 0, 0) )
  {
    resultCode = 2;
  }
  else
  {
    if ( pmmckiParent->ckid == 'FFIR' && pmmckiParent->fccType == 'EVAW' )
    {
      pmmcki.ckid = ' tmf';
      if ( mmioDescend(hmmio, &pmmcki, pmmckiParent, 0x10u) )
      {
        resultCode = 4;
      }
      else
      {
        if ( pmmcki.cksize >= 0x10 )
        {
          if ( mmioRead(hmmio, Src, 16) == 16 )
          {
            if ( *(unsigned __int16 *)Src == 1 )
            {
              v7 &= 0xffff0000;
            }
            else
            {
              if ( mmioRead(hmmio, (HPSTR)&v7, 2) != 2 )
              {
                resultCode = 7;
                goto LABEL_28;
              }
            }
            *out_a3 = GlobalAlloc(0, (unsigned __int16)v7 + 18);
            if ( *out_a3 )
            {
              memcpy(*out_a3, Src, 0x10u);
              *((WORD*)*out_a3 + 8) = v7;
              if ( (WORD)v7
                && (v4 = mmioRead(hmmio, (HPSTR)*out_a3 + 18, (unsigned __int16)v7), v4 != (unsigned __int16)v7) )
              {
                resultCode = 9;
              }
              else
              {
                if ( !mmioAscend(hmmio, &pmmcki, 0) )
                {
                  resultCode = 0;
                  goto LABEL_32;
                }
                resultCode = 10;
              }
            }
            else
            {
              resultCode = 8;
            }
          }
          else
          {
            resultCode = 6;
          }
        }
        else
        {
          resultCode = 5;
        }
      }
    }
    else
    {
      resultCode = 3;
    }
  }
LABEL_28:
  if ( *out_a3 )
  {
    GlobalFree(*out_a3);
    *out_a3 = 0;
  }
  if ( hmmio )
  {
    mmioClose(hmmio, 0);
    hmmio = 0;
  }
LABEL_32:
  *outMmio = hmmio;
  return resultCode;
}
