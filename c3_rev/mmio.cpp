#include "mmio.h"
#include "variables.h"
#include <direct.h>
#include <time.h>

void fun_strtoupper(char *a1)
{
  unsigned __int8 v1; // [sp+4Ch] [bp-4h]@2

  while ( *a1 )
  {
    v1 = *a1;
    if ( (signed int)*a1 >= 'a' )
    {
      if ( (signed int)v1 <= 'z' )
        v1 -= 32;
    }
    *a1++ = v1;
  }
}

void fun_strncpy(const char *src, char *dst, int len)
{
  int i; // [sp+4Ch] [bp-4h]@1

  for ( i = 0; i < len; ++i )
    dst[i] = src[i];
}


void fun_storeFileExtension(const char *filename)
{
  unsigned __int8 v1; // [sp+4Ch] [bp-4h]@1
  char *v2; // [sp+58h] [bp+8h]@4

  do
    v1 = *filename++;
  while ( v1 != '.' && v1 );
  if ( v1 == '.' )
  {
    fun_strncpy( filename, current_fileExtension, 3 );

    v2 = (char *)(filename + 1);
    getextension_result_1 = *v2;
    getextension_result_2 = v2[1];
    getextension_result_3 = 0;
  }
}


int fun_chdirToCdBasedOnExtension(const char *filename)
{
  signed int result; // eax@8
  int v2; // [sp+4Ch] [bp-8h]@1
  signed int i; // [sp+50h] [bp-4h]@9

  v2 = 0;
  fun_storeFileExtension(filename);
  fun_strtoupper(current_fileExtension);
  if ( strcmp("WAV", current_fileExtension) )
  {
    if ( strcmp("SMK", current_fileExtension) )
    {
      if ( !strcmp("555", current_fileExtension) )
        v2 = 0;
    }
    else
    {
      v2 = 0;
    }
  }
  else
  {
    v2 = 0;
  }
  if ( v2 )
  {
    for ( i = 0; i < 40; ++i )
      chdir_extension_value[i] = 0;

    if ( strcmp("WAV", current_fileExtension) )
    {
      if ( strcmp("SMK", current_fileExtension) )
      {
        if ( !strcmp("555", current_fileExtension) )
          fun_strncpy("c:\\", chdir_extension_value, 10);
      }
      else
      {
        fun_strncpy("c:\\", chdir_extension_value, 10);
      }
    }
    else
    {
      fun_strncpy("c:\\", chdir_extension_value, 10);
    }
    chdir_extension_value[0] = setting_cddrive;
    if ( _chdir(chdir_extension_value) )
      result = 0;
    else
      result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

void fun_chdirHome()
{
  if ( _chdir(chdir_home) )
  {
    _getcwd(getcwd_result, 100);
    fun_logDebugMessage("ERR:CHD  not reserted is now", getcwd_result, 0);
  }
}

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
    fun_chdirToCdBasedOnExtension(pszFileName);
    hmmio = mmioOpenA((LPSTR)pszFileName, 0, 0x10000u);
    if ( !hmmio )
    {
      resultCode = 1;
      fun_chdirHome();
      goto LABEL_28;
    }
    fun_chdirHome();
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
              _LOWORD(v7) = 0;
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
              *((_WORD *)*out_a3 + 8) = v7;
              if ( (_WORD)v7
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

int mmio_close(HMMIO *mmio, HGLOBAL *a2)
{
  if ( *a2 )
  {
    GlobalFree(*a2);
    *a2 = 0;
  }
  if ( *mmio )
  {
    mmioClose(*mmio, 0);
    *mmio = 0;
  }
  return 0;
}


int mmio_530B40(HMMIO hmmio, int length1, char* a3, int a4, int a5)
{
  size_t v6; // [sp+4Ch] [bp-58h]@12
  unsigned int v7; // [sp+50h] [bp-54h]@3
  unsigned int v8; // [sp+54h] [bp-50h]@5
  signed int v9; // [sp+58h] [bp-4Ch]@2
  struct _MMIOINFO pmmioinfo; // [sp+5Ch] [bp-48h]@1

  if( mmioGetInfo(hmmio, &pmmioinfo, 0) != 0 )
  {
    v9 = 1;
LABEL_20:
    *(_DWORD *)a5 = 0;
    return v9;
  }
  v7 = length1;
  if( length1 > *(_DWORD *)(a4 + 4) )
    v7 = *(_DWORD *)(a4 + 4);
  *(_DWORD *)(a4 + 4) -= v7;
  v8 = 0;
  while ( v8 < v7 )
  {
    if ( pmmioinfo.pchNext == pmmioinfo.pchEndRead )
    {
      if ( mmioAdvance(hmmio, &pmmioinfo, 0) )
      {
        v9 = 2;
        goto LABEL_20;
      }
      if ( pmmioinfo.pchNext == pmmioinfo.pchEndRead )
      {
        v9 = 3;
        goto LABEL_20;
      }
    }
    v6 = pmmioinfo.pchEndRead - pmmioinfo.pchNext;
    if ( (HPSTR)(v7 - v8) < (HPSTR)(pmmioinfo.pchEndRead - pmmioinfo.pchNext) )
      v6 = v7 - v8;
    if ( v6 > 0x30D40 )
    {
      v9 = 5;
      goto LABEL_20;
    }
    memcpy((void *)(v8 + a3), pmmioinfo.pchNext, v6);
    v8 += v6;
    pmmioinfo.pchNext += v6;
  }
  if ( mmioSetInfo(hmmio, &pmmioinfo, 0) )
  {
    v9 = 4;
    goto LABEL_20;
  }
  *(_DWORD *)a5 = v7;
  return 0;
}

void __cdecl fun_writeLogMessageToStatusTxt(DWORD nNumberOfBytesToWrite)
{
  int v1; // [sp+54h] [bp-4h]@2

  if ( !log_statusTxtExists )
  {
    v1 = _open("status.txt", 0x8301u);          // overwrite
    if ( v1 == -1 )
      return;
LABEL_8:
    _write(v1, log_message, nNumberOfBytesToWrite);
    _close(v1);
    log_statusTxtExists = 1;
    return;
  }
  v1 = _open("status.txt", 0x8101u);            // append
  if ( v1 != -1 )
  {
    _lseek(v1, 0, SEEK_END);
    goto LABEL_8;
  }
}

void fun_getTimeAsString()
{
  struct tm *v0; // ST50_4@1
  time_t Time; // [sp+50h] [bp-Ch]@1
  int i; // [sp+58h] [bp-4h]@1

  time(&Time);
  v0 = localtime(&Time);
  /*_HIDWORD(Time) = asctime(v0);
  for ( i = 0; i < 40; ++i )
  {
    log_currentTime_date[i] = *_HIDWORD(Time);
    Time += 4294967296;
  }*/
}

int fun_intToString(signed int number, int offset, int addPlusSign)
{
  int width; // [sp+4Ch] [bp-Ch]@27
  signed int numDigits; // [sp+50h] [bp-8h]@9
  signed int i; // [sp+54h] [bp-4h]@1

  for ( i = 0; i < 100; ++i )
    intToString_result[i] = 0;

  if ( number >= 0 )
  {
    if ( addPlusSign )
      intToString_result[  offset++] = '+';
  }
  else
  {
    number = -number;
    intToString_result[offset++] = '-';
  }

  if ( number >= 10 )
  {
    if ( number >= 100 )
    {
      if ( number >= 1000 )
      {
        if ( number >= 10000 )
        {
          if ( number >= 100000 )
          {
            if ( number >= 1000000 )
            {
              if ( number >= 10000000 )
              {
                if ( number >= 100000000 )
                {
                  if ( number >= 1000000000 )
                    numDigits = 0;
                  else
                    numDigits = 9;
                }
                else
                {
                  numDigits = 8;
                }
              }
              else
              {
                numDigits = 7;
              }
            }
            else
            {
              numDigits = 6;
            }
          }
          else
          {
            numDigits = 5;
          }
        }
        else
        {
          numDigits = 4;
        }
      }
      else
      {
        numDigits = 3;
      }
    }
    else
    {
      numDigits = 2;
    }
  }
  else
  {
    numDigits = 1;
  }
  width = offset + numDigits;
  while ( 1 )
  {
    --numDigits;
    if ( numDigits < 0 )
      break;
    intToString_result[numDigits + offset] = number % 10 + '0';
    number /= 10;
  }
  return width;
}





void fun_getGameTextString(int group, int number)
{
  gametext_result = (char *)&c3eng_data + c3eng_index[group].offset;
  while ( number > 0 )
  {
    if ( !*gametext_result )
    {
      if ( (signed int)(unsigned __int8)*(gametext_result - 1) >= ' ' || !*(gametext_result - 1) )
        --number;
    }
    ++gametext_result;
  }
  while ( (signed int)(unsigned __int8)*gametext_result < ' ' )
    ++gametext_result;
}

int cvtdate(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11)
{
  int result; // eax@20
  int v12; // [sp+0h] [bp-14h]@16
  int v13; // [sp+4h] [bp-10h]@10
  int v14; // [sp+8h] [bp-Ch]@3
  int v15; // [sp+Ch] [bp-8h]@5
  int v16; // [sp+Ch] [bp-8h]@6
  int v17; // [sp+10h] [bp-4h]@5

  if ( a2 == 1 )
  {
    if ( a3 & 3 )
      v14 = dword_606D98[a4];
    else
      v14 = (int)(&off_606D64)[2 * a4];

    v15 = v14 + 1;
    v17 = (365 * (a3 - 70) + v14 + 1 + ((a3 - 1) >> 2) - 13) % 7;
    if ( (365 * (a3 - 70) + v14 + 1 + ((a3 - 1) >> 2) - 13) % 7 >= a6 )
      v16 = a6 - v17 + v15 + 7 * a5;
    else
      v16 = a6 - v17 + v15 + 7 * (a5 - 1);
    if ( a5 == 5 )
    {
      if ( a3 & 3 )
        v13 = dword_606D9C[a4];
      else
        v13 = dword_606D68[a4];
      if ( v16 > v13 )
        v16 -= 7;
    }
  }
  else
  {
    if ( a3 & 3 )
      v12 = dword_606D98[a4];
    else
      v12 = (int)(&off_606D64)[2 * a4];
    v16 = a7 + v12;
  }
  if ( a1 == 1 )
  {
    dword_606AAC = v16;
    result = 1000 * (60 * (60 * a8 + a9) + a10);
    dword_606AB0 = result + a11;
    dword_606AA8 = a3;
  }
  else
  {
    dword_606ABC = v16;
    dword_606AC0 = 1000 * (60 * (60 * a8 + a9) + a10) + a11;
    result = 1000 * dword_606A18 + dword_606AC0;
    dword_606AC0 += 1000 * dword_606A18;
    if ( dword_606AC0 >= 0 )
    {
      if ( dword_606AC0 >= 86400000 )
      {
        result = dword_606AC0 - 86400000;
        dword_606AC0 -= 86400000;
        ++dword_606ABC;
      }
    }
    else
    {
      dword_606AC0 += 86400000;
      --dword_606ABC;
    }
    dword_606AB8 = a3;
  }
  return result;
}
