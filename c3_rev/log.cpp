#include "log.h"
#include "windows.h"
#include "mmio.h"
#include "variables.h"

void fun_logDebugMessage(const char *msg, const char *paramStr, int paramInt)
{
  DWORD v3; // [sp+4Ch] [bp-Ch]@4
  DWORD v4; // [sp+4Ch] [bp-Ch]@14
  DWORD v5; // [sp+4Ch] [bp-Ch]@21
  char cr; // [sp+50h] [bp-8h]@1
  char lf; // [sp+51h] [bp-7h]@1
  int i; // [sp+54h] [bp-4h]@4

  cr = '\r';
  lf = '\n';
  fun_getTimeAsString();
  //if ( a3 > 0 )
  //  fun_intToString(a3 - 1, 0, 0);

  if ( log_statusTxtExists )
  {
    v5 = 0;
    for ( i = 0; i < 10; ++i )                  // log the time
      log_message[v5++] = log_currentTime_date[i + 10];
    for ( i = 0; i < 2; ++i )                   // separator (2 spaces)
      log_message[v5++] = ' ';
    for ( i = 0; i < 100 && msg[i]; ++i )       // message
      log_message[v5++] = msg[i];
    if ( a2 )                                   // string parameter
    {
      for ( i = 0; i < 100 && a2[i]; ++i )
        log_message[v5++] = a2[i];
      for ( i = 0; i < 2; ++i )
        log_message[v5++] = ' ';
    }
    if ( a3 )
    {
      for ( i = 0; i < 100 && *(&intToString_result + i); ++i )
        log_message[v5++] = intToString_result[i];
      for ( i = 0; i < 2; ++i )
        log_message[v5++] = ' ';
    }
    for ( i = 0; i < 2; ++i )                   // endl
      log_message[v5++] = *(&cr + i);
    fun_writeLogMessageToStatusTxt(v5);
  }
  else
  {
    v3 = 0;
    for ( i = 0; i < 100 && msg[i]; ++i )
      log_message[v3++] = msg[i];
    for ( i = 0; i < 10; ++i )
      log_message[v3++] = log_currentTime_date[i];
    for ( i = 0; i < 2; ++i )
      log_message[v3++] = *(&cr + i);           // endl
    fun_writeLogMessageToStatusTxt(v3);
    v4 = 0;
    for ( i = 0; i < 60; ++i )
      log_message[v4++] = '-';
    for ( i = 0; i < 2; ++i )                   // endl
      log_message[v4++] = *(&cr + i);
    fun_writeLogMessageToStatusTxt(v4);
  }
}


