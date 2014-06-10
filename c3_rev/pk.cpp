#include "pk.h"
#include "log.h"
#include "variables.h"

int  fun_pkDecompress(int *hashcode, char *compressedData, int compressedLength, char *destBuffer, int destLength)
{
  int result; // eax@2
  struct PKDecompBuffer *buf; // [sp+4Ch] [bp-44h]@3
  struct PKDecompBuffer *buffer; // [sp+50h] [bp-40h]@1
  struct PKToken token; // [sp+54h] [bp-3Ch]@5
  int ok; // [sp+88h] [bp-8h]@1
  int explodeErr; // [sp+8Ch] [bp-4h]@5

  ok = 1;
  buffer = (struct PKDecompBuffer *)GlobalAlloc(0x42u, 0x8DD8u);
  if ( buffer )
  {
    buf = (struct PKDecompBuffer *)GlobalLock(buffer);
    if ( buf )
    {
      memset(&token, 0, 0x34u);
      token.field_24 = 2;
      token.hashcode = -1;
      token.compressedData = compressedData;
      token.compressedLength = compressedLength;
      token.destData = destBuffer;
      token.destLength = destLength;
      explodeErr = fun_pk_explode(fun_pk_inputFunc, fun_pk_outputFunc, buf, &token);
      if ( explodeErr || token.stop )
      {
        fun_logDebugMessage("ERR:COMP Error uncompressing.", 0, 0);
        ok = 0;
      }
      else
      {
        token.hashcode = ~token.hashcode;
        *hashcode = token.hashcode;
      }
      GlobalUnlock(buffer);
      GlobalFree(buffer);
      result = ok;
    }
    else
    {
      GlobalFree(buffer);
      result = 0;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}

int  fun_pk_inputFunc(char *buffer, int *length, struct PKToken *token)
{
  int result; // eax@2
  int v4; // [sp+50h] [bp-10h]@1
  int v5; // [sp+54h] [bp-Ch]@9
  void *v6; // [sp+58h] [bp-8h]@4
  struct PKToken *v7; // [sp+5Ch] [bp-4h]@1

  v4 = *length;
  v7 = token;
  if ( token->stop == 1 )
  {
    result = 0;
  }
  else
  {
    if ( v7->field_24 == 1 )
      v6 = v7;
    else
      v6 = &v7->compressedData;
    if ( *((_DWORD *)v6 + 1) >= *((_DWORD *)v6 + 2) )
    {
      v5 = 0;
    }
    else
    {
      if ( *((_DWORD *)v6 + 2) - *((_DWORD *)v6 + 1) < (unsigned int)v4 )
        v4 = *((_DWORD *)v6 + 2) - *((_DWORD *)v6 + 1);
      memcpy(buffer, (const void *)(*((_DWORD *)v6 + 1) + *(_DWORD *)v6), v4);
      *((_DWORD *)v6 + 1) += v4;
      v5 = v4;
    }
    if ( v7->field_24 == 1 )
      v7->hashcode = fun_pk_hashBuffer(buffer, (int)&v5, &v7->hashcode);
    result = v5;
  }
  return result;
}

void  fun_pk_outputFunc(char *Src, int *a2, struct PKToken *a3)
{
  size_t Size; // [sp+4Ch] [bp-Ch]@1
  char **v4; // [sp+50h] [bp-8h]@4
  struct PKToken *v5; // [sp+54h] [bp-4h]@1

  Size = *a2;
  v5 = a3;
  if ( a3->stop != 1 )
  {
    if ( v5->field_24 == 1 )
    {
      v4 = &v5->compressedData;
      v5->field_2C += Size;
    }
    else
    {
      v4 = &v5->destData;
    }
    if ( v4[1] >= v4[2] )
    {
      fun_logDebugMessage("ERR:COMP2 Out of buffer space.", 0, 0);
      v5->stop = 1;
    }
    else
    {
      if ( v4[2] - v4[1] >= Size )
      {
        memcpy((void *)((_DWORD)v4[1] + (_DWORD)*v4), Src, Size);
        v4[1] += Size;
        if ( v5->field_24 == 2 )
          v5->hashcode = fun_pk_hashBuffer(Src, (int)&Size, &v5->hashcode);
      }
      else
      {
        fun_logDebugMessage("ERR:COMP1 Corrupt.", 0, 0);
        v5->stop = 1;
      }
    }
  }
}

int  fun_pk_hashBuffer(char *a1, int a2, int *a3)
{
  int result; // eax@1
  int v4; // esi@1
  char *v5; // edx@2
  char v6; // cl@3
  int v7; // ecx@3

  result = *a3;
  v4 = *(_DWORD *)a2 - 1;
  if ( *(_DWORD *)a2 )
  {
    v5 = a1;
    do
    {
      v6 = *v5++;
      result = ((unsigned int)result >> 8) ^ byte_605A68[(unsigned __int8)(result ^ v6)];
      v7 = v4--;
    }
    while ( v7 );
  }
  return result;
}

int  fun_pkCompress(int *hashcode, int *compressedSize, char *inputBuffer, char *outputBuffer, int inputLength)
{
  int result; // eax@2
  struct PKCompBuffer *v6; // [sp+4Ch] [bp-44h]@3
  HGLOBAL v7; // [sp+50h] [bp-40h]@1
  struct PKToken token; // [sp+54h] [bp-3Ch]@5
  int v9; // [sp+88h] [bp-8h]@1
  int v10; // [sp+8Ch] [bp-4h]@5

  v9 = 1;
  v7 = GlobalAlloc(0x42u, 0x8DD8u);
  if ( v7 )
  {
    v6 = (struct PKCompBuffer *)GlobalLock(v7);
    if ( v6 )
    {
      memset(&token, 0, 0x34u);
      token.field_24 = 1;
      token.hashcode = -1;
      token.inputData = inputBuffer;
      token.inputLength = inputLength;
      token.compressedData = outputBuffer;
      token.compressedLength = inputLength;
      v10 = fun_pk_implode(fun_pk_inputFunc, fun_pk_outputFunc, v6, &token, &hasLiteralEncoding, &dictionarySize);
      if ( v10 || token.stop )
      {
        fun_logDebugMessage("ERR:COMP Error occurred while compressing.", 0, 0);
        v9 = 0;
      }
      else
      {
        token.hashcode = ~token.hashcode;
        *hashcode = token.hashcode;
        *compressedSize = token.field_2C;
      }
      GlobalUnlock(v7);
      GlobalFree(v7);
      result = v9;
    }
    else
    {
      GlobalFree(v7);
      result = 0;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}

void  fun_pk_explodeCopyMem(char *dst, char *src, unsigned int length)
{
  char *d; // edx@1
  char *s; // ecx@1
  unsigned int i; // esi@1
  int tmp; // edi@2
  unsigned int j; // eax@3
  char tmpb; // bl@4

  s = src;
  d = dst;
  for ( i = length >> 2; i; --i )               // copy 4 bytes at the time
  {
    tmp = *(_DWORD *)s;
    s += 4;
    *(_DWORD *)d = tmp;
    d += 4;
  }
  for ( j = length & 3; j; --j )                // copy remaining 0-3 bytes
  {
    tmpb = *s++;
    *d++ = tmpb;
  }
}

void  fun_pk_constructLiteralDecoder(struct PKDecompBuffer *buf)
{
  signed int index; // edx@1
  unsigned __int16 *v2; // esi@1
  char v3; // cl@2
  char *v4; // ebp@2
  unsigned int v5; // eax@3
  char v6; // cl@7
  int v7; // eax@8
  char v8; // cl@8
  int v9; // eax@11
  char v10; // cl@11
  int v11; // eax@14
  char v12; // cl@14

  index = 255;
  v2 = (unsigned __int16 *)&pk_literalSequencesEnd;
  do
  {
    v3 = *v4;
    if ( (unsigned __int8)*v4 > 8u )
    {
      if ( *v2 & 0xFF )
      {
        buf->literalDecode1[*v2 & 0xFF] = -1;
        v6 = *v4;
        if ( *(_BYTE *)v2 & 0x3F )
        {
          v8 = v6 - 4;
          *v4 = v8;
          v4 = (char *)(1 << v8);
          v7 = *v2 >> 4;
          do
          {
            buf->literalDecode2[v7] = index;
            v7 += (int)v4;
          }
          while ( (unsigned int)v7 < 0x100 );
        }
        else
        {
          v10 = v6 - 6;
          *v4 = v10;
          v4 = (char *)(1 << v10);
          v9 = *v2 >> 6;
          do
          {
            buf->literalDecode3[v9] = index;
            v9 += (int)v4;
          }
          while ( (unsigned int)v9 < 0x80 );
        }
      }
      else
      {
        v12 = v3 - 8;
        *v4 = v12;
        v4 = (char *)(1 << v12);
        v11 = *v2 >> 8;
        do
        {
          buf->literalDecode4[v11] = index;
          v11 += (int)v4;
        }
        while ( (unsigned int)v11 < 0x100 );
      }
    }
    else
    {
      v4 = (char *)(1 << v3);
      v5 = *v2;
      do
      {
        buf->literalDecode1[v5] = index;
        v5 += (unsigned int)v4;
      }
      while ( v5 < 0x100 );
    }
    --v2;
    --index;
  }
  while ( (unsigned int)v2 >= (unsigned int)pk_literalSequences );
}

void  fun_pk_writeBits(struct PKCompBuffer *buffer, unsigned int numBits, unsigned int value)
{
  unsigned int numBitsToWrite; // edi@1
  unsigned int valueToWrite; // ebx@2
  struct PKCompBuffer *buf; // esi@2
  int currentBitsUsed; // eax@4
  int totalBits; // ecx@4
  int nextOutputPtr; // edx@5
  int newBitsUsed; // ecx@6

  numBitsToWrite = numBits;
  if ( numBits <= 8 )
  {
    buf = buffer;
    valueToWrite = value;
  }
  else
  {
    buf = buffer;
    numBitsToWrite = numBits - 8;
    valueToWrite = value >> 8;
    fun_pk_writeBits(buffer, 8u, value);
  }
  currentBitsUsed = buf->currentOutputBitsUsed;
  totalBits = currentBitsUsed + numBitsToWrite;
  buf->outputData[buf->outputPtr] |= (_BYTE)valueToWrite << buf->currentOutputBitsUsed;
  buf->currentOutputBitsUsed = currentBitsUsed + numBitsToWrite;
  if ( currentBitsUsed + numBitsToWrite <= 8 )
  {
    newBitsUsed = totalBits & 7;
    buf->currentOutputBitsUsed = newBitsUsed;
    if ( !newBitsUsed )
      ++buf->outputPtr;
  }
  else
  {
    nextOutputPtr = buf->outputPtr + 1;
    buf->outputPtr = nextOutputPtr;
    buf->outputData[nextOutputPtr] = valueToWrite >> (8 - currentBitsUsed);
    buf->currentOutputBitsUsed &= 7u;
  }
  if ( buf->outputPtr >= 0x800u )
    fun_pk_implode_flushFullBuffer(buf);
}

void  fun_pk_implodeCopyMem(char *dst, const char *src, unsigned int length)
{
  char *d; // edx@1
  const char *s; // ecx@1
  unsigned int i; // esi@1
  int tmp; // edi@2
  unsigned int j; // eax@3
  char tmp2; // bl@4

  s = src;
  d = dst;
  for ( i = length >> 2; i; --i )
  {
    tmp = *(_DWORD *)s;
    s += 4;
    *(_DWORD *)d = tmp;
    d += 4;
  }
  for ( j = length & 3; j; --j )
  {
    tmp2 = *s++;
    *d++ = tmp2;
  }
}


void  fun_pk_constructTable(int size, char *a2, char *table, char *a4)
{
  int i; // ebx@1
  unsigned int v5; // eax@2
  char v6; // cl@2

  for ( i = size - 1; i >= 0; --i )
  {
    v6 = a2[i];
    v5 = (unsigned __int8)table[i];
    do
    {
      a4[v5] = i;
      v5 += 1 << v6;
    }
    while ( v5 < 0x100 );
  }
}

signed int  fun_pk_setBitsUsed(struct PKDecompBuffer *buf, unsigned int numBits)
{
  int available; // eax@1
  int *v4; // edi@3
  struct PKToken *token; // ST08_4@4
  int bytesRead; // eax@4
  int v7; // edx@7
  unsigned int v8; // edx@7
  int v9; // eax@7

  available = buf->currentInput_bitsAvailable;
  if ( available >= numBits )
  {
    buf->currentInput_bitsAvailable = available - numBits;
    buf->currentInputByte = (unsigned int)buf->currentInputByte >> numBits;
    return 0;
  }
  v4 = &buf->inputBufferPtr;
  buf->currentInputByte = (unsigned int)buf->currentInputByte >> available;
  if ( buf->inputBufferPtr == buf->inputBufferEnd )
  {
    token = buf->token;
    *v4 = 2048;
    bytesRead = buf->inputFunc(buf->inputBuffer, &buf->inputBufferPtr, token);
    buf->inputBufferEnd = bytesRead;
    if ( !bytesRead )
      return 1;
    *v4 = 0;
  }
  v7 = buf->inputBuffer[*v4++] << 8;
  v8 = buf->currentInputByte | v7;
  v9 = buf->currentInput_bitsAvailable;
  buf->currentInputByte = v8;
  buf->currentInputByte = v8 >> ((_BYTE)numBits - v9);
  buf->currentInput_bitsAvailable = v9 - numBits + 8;
  return 0;
}

void  fun_pk_implode_memset(void *buffer, unsigned __int8 fillChar, unsigned int length)
{
  char *v3; // edx@1
  int v4; // ecx@1
  unsigned int lengthInInts; // ebp@1
  unsigned int v6; // esi@5
  int v7; // eax@8
  int i; // ecx@8
  int v9; // edi@8

  v3 = (char *)buffer;
  lengthInInts = length >> 2;
  v4 = fillChar << 16;
  if ( length >> 2 )
  {
    if ( lengthInInts <= 1 )
      lengthInInts = 1;
    memset(buffer, fillChar | v4 | ((v4 | fillChar) << 8), 4 * lengthInInts);
    v3 = (char *)buffer + 4 * lengthInInts;
  }
  v6 = length & 3;
  if ( length & 3 )
  {
    if ( v6 <= 1 )
      v6 = 1;
    BYTE1(v4) = fillChar;
    _LOBYTE(v4) = fillChar;
    v7 = v4 << 16;
    _LOWORD(v7) = v4;
    memset(v3, v7, 4 * (v6 >> 2));
    v9 = (int)&v3[4 * (v6 >> 2)];
    for ( i = v6 & 3; i; --i )
      *(_BYTE *)v9++ = v7;
  }
}

int  fun_pk_getCopyOffset(struct PKDecompBuffer *buf, int copylength)
{
  int v2; // edi@1
  int v4; // edi@4
  int v5; // ST04_4@6

  v2 = buf->copyOffsetTable[buf->currentInputByte & 0xFF];
  if ( fun_pk_setBitsUsed(buf, buf->copyOffsetBits[v2]) )
    return 0;
  if ( copylength == 2 )
  {
    v4 = buf->currentInputByte & 3 | 4 * v2;
    if ( fun_pk_setBitsUsed(buf, 2u) )
      return 0;
  }
  else
  {
    v5 = buf->windowSize;
    v4 = buf->currentInputByte & buf->dictionarySize | (v2 << v5);
    if ( fun_pk_setBitsUsed(buf, v5) )
      return 0;
  }
  return v4 + 1;
}


int  fun_pk_decodeNextToken(struct PKDecompBuffer *buf)
{
  int v2; // edi@4
  char v3; // cl@6
  int v4; // ebx@7
  int returnValue; // edi@11
  int v6; // ecx@18

  if ( buf->currentInputByte & 1 )
  {
    if ( fun_pk_setBitsUsed(buf, 1u) )
      return 774;
    v2 = buf->copyLengthTable[buf->currentInputByte & 0xFF];
    if ( fun_pk_setBitsUsed(buf, buf->copyLengthBaseBits[v2]) )
      return 774;
    v3 = buf->copyLengthExtraBits[v2];
    if ( v3 )
    {
      v4 = buf->currentInputByte & ((1 << v3) - 1);
      if ( fun_pk_setBitsUsed(buf, (unsigned __int8)v3) && v2 + v4 != 270 )
        return 774;
      v2 = *(_WORD *)&buf->copyLengthBaseValue[2 * v2] + v4;
    }
    return v2 + 256;
  }
  if ( fun_pk_setBitsUsed(buf, 1u) )
    return 774;
  if ( !buf->hasLiteralEncoding )
  {
    returnValue = buf->currentInputByte & 0xFF;
    if ( fun_pk_setBitsUsed(buf, 8u) )
      return 774;
    return returnValue;
  }
  v6 = buf->currentInputByte;
  if ( (unsigned __int8)buf->currentInputByte )
  {
    returnValue = buf->literalDecode1[(unsigned __int8)buf->currentInputByte];
    if ( returnValue == 255 )
    {
      if ( v6 & 0x3F )
      {
        if ( fun_pk_setBitsUsed(buf, 4u) )
          return 774;
        returnValue = buf->literalDecode2[buf->currentInputByte & 0xFF];
      }
      else
      {
        if ( fun_pk_setBitsUsed(buf, 6u) )
          return 774;
        returnValue = buf->literalDecode3[buf->currentInputByte & 0x7F];
      }
    }
  }
  else
  {
    if ( fun_pk_setBitsUsed(buf, 8u) )
      return 774;
    returnValue = buf->literalDecode4[buf->currentInputByte & 0xFF];
  }
  if ( !fun_pk_setBitsUsed(buf, buf->literalDecode5[returnValue]) )
    return returnValue;
  return 774;
}

int  fun_pk_explodeData(struct PKDecompBuffer *buf)
{
  int v1; // eax@2
  int v2; // eax@5
  char *v3; // edx@6
  char *v4; // ecx@6
  int v5; // ebp@6
  char v6; // al@7
  PKToken *v8; // ST08_4@9
  PKToken *v10; // ST08_4@11
  int v11; // [sp+10h] [bp-8h]@2
  int v12; // [sp+14h] [bp-4h]@11

  buf->outputBufferPtr = 4096;
  while ( 1 )
  {
    v1 = fun_pk_decodeNextToken(buf);
    v11 = v1;
    if ( (unsigned int)v1 >= 0x305 )
      break;
    if ( (unsigned int)v1 >= 0x100 )
    {
      v11 -= 254;
      v2 = fun_pk_getCopyOffset(buf, v11);
      if ( !v2 )
      {
        v11 = 774;
        break;
      }
      v5 = buf->outputBufferPtr;
      v3 = &buf->outputBuffer[v5];
      v4 = &buf->outputBuffer[v5] - v2;
      buf->outputBufferPtr = v5 + v11;
      do
      {
        v6 = *v4++;
        *v3++ = v6;
      }
      while ( v11-- != 1 );
    }
    else
    {
      buf->outputBuffer[buf->outputBufferPtr++] = v1;
    }
    if ( buf->outputBufferPtr >= 8192u )
    {
      v8 = buf->token;
      v11 = 4096;
      buf->outputFunc(&buf->outputBuffer[4096], &v11, v8);
      fun_pk_explodeCopyMem(buf->outputBuffer, &buf->outputBuffer[4096], buf->outputBufferPtr - 4096);
      buf->outputBufferPtr -= 4096;
    }
  }
  v10 = buf->token;
  v12 = buf->outputBufferPtr - 4096;
  buf->outputFunc(&buf->outputBuffer[4096], &v12, v10);
  return v11;
}

signed int  fun_pk_implode(int ( *inputFunc)(char *, int *, struct PKToken *),
                                  void ( *outputFunc)(char *, int *, struct PKToken *),
                                  PKCompBuffer *buffer, struct PKToken *token, int *hasLiteralEncoding, int *dictionarySize)
{
  int dictSize; // eax@1
  signed int v7; // ecx@1
  __int16 *v9; // eax@10
  unsigned int index; // edx@10
  __int16 v11; // di@10
  __int16 *bufferLiteralValuePtr; // eax@13
  __int16 *literalValuePtr; // edi@13
  unsigned int copyIndex; // eax@15
  unsigned int v15; // ebx@16
  __int16 *literalValues; // edi@17
  __int16 v17; // bp@18

  buffer->inputFunc = inputFunc;
  buffer->outputFunc = outputFunc;
  _HIWORD(v7) = _HIWORD(hasLiteralEncoding);
  buffer->dictionarySize = *dictionarySize;
  buffer->hasLiteralEncoding = *hasLiteralEncoding;
  buffer->token = token;
  buffer->windowSize = 4;
  buffer->copyOffsetExtraMask = 15;
  dictSize = *dictionarySize;
  if ( *dictionarySize != 1024 )
  {
    if ( dictSize != 2048 )
    {
      if ( dictSize != 4096 )
        return 1;
      ++buffer->windowSize;
      buffer->copyOffsetExtraMask |= 0x20u;
    }
    ++buffer->windowSize;
    buffer->copyOffsetExtraMask |= 0x10u;
  }
  if ( *hasLiteralEncoding )
  {
    if ( *hasLiteralEncoding != 1 )
      return 2;
    index = 0;
    literalValuePtr = pk_literalValues;
    bufferLiteralValuePtr = buffer->literalValues;
    do
    {
      ++literalValuePtr;
      ++bufferLiteralValuePtr;
      buffer->literalBits[index] = pk_literalLengths[index] + 1;
      ++index;
      _LOWORD(v7) = 2 * *(literalValuePtr - 1);
      *(bufferLiteralValuePtr - 1) = v7;
    }
    while ( (unsigned int)literalValuePtr < (unsigned int)"PKWARE Data Compression Library for Win32\r\nCopyright 1989-1995 PKWARE Inc.  All Rights Reserved\r\nPatent No. 5,051,745\r\nPKWARE Data Compression Library Reg. U.S. Pat. and Tm. Off.\r\nVersion 1.11" );
  }
  else
  {
    v11 = 0;
    index = 0;
    v9 = buffer->literalValues;
    v7 = 9;
    do
    {
      buffer->literalBits[index] = 9;
      ++v9;
      *(v9 - 1) = v11;
      ++index;
      v11 += 2;
    }
    while ( index < 0x100 );
  }
  copyIndex = 0;
  do
  {
    v15 = 0;
    if ( 1 << pk_implode_copyLengthBaseBits[copyIndex] )
    {
      literalValues = &buffer->literalValues[index];
      do
      {
        v17 = v15++;
        ++literalValues;
        buffer->literalBits[index++] = pk_implode_copyLengthExtraBits[copyIndex]
                                     + pk_implode_copyLengthBaseBits[copyIndex]
                                     + 1;
        _LOBYTE(v7) = pk_implode_copyLengthBaseCode[copyIndex];
        v7 &= 0xFFFF00FFu;
        _LOWORD(v7) = 2 * v7;
        *(literalValues - 1) = v7 | (unsigned __int16)(v17 << (pk_implode_copyLengthExtraBits[copyIndex] + 1)) | 1;
      }
      while ( 1 << pk_implode_copyLengthBaseBits[copyIndex] > v15 );
    }
    ++copyIndex;
  }
  while ( copyIndex < 0x10 );
  fun_pk_implodeCopyMem(buffer->copyOffsetCode, pk_implode_copyOffsetCode, 0x40u);
  fun_pk_implodeCopyMem(buffer->copyOffsetBits, pk_implode_copyOffsetBits, 0x40u);
  fun_pk_implodeData(buffer);
  return 0;
}


void  fun_pk_implodeData(struct PKCompBuffer *buffer)
{
  unsigned char* inputPtr; // edi@1
  int dictSize; // eax@1
  int bufferUsedBytes; // ebx@2
  signed int bufferFreeBytes; // ebp@2
  struct PKToken *v5; // eax@3
  int token; // eax@3
  int dictSz; // ecx@9
  signed int copyLength; // ebx@19
  unsigned int origCopyOffset; // ebp@25
  unsigned int inputByteEncodedValue; // ecx@28
  int inputByte; // eax@28
  unsigned int v12; // ecx@29
  int v13; // eax@29
  int copyOffset; // eax@41
  unsigned int origCopyLength; // [sp+10h] [bp-14h]@25
  char *inputEndPtr[2]; // [sp+14h] [bp-10h]@1
  int bytesToRead; // [sp+1Ch] [bp-8h]@3
  int hasDataMarker; // [sp+20h] [bp-4h]@1

  hasDataMarker = 0;
  inputEndPtr[1] = 0;
  dictSize = buffer->dictionarySize;
  buffer->outputData[0] = LOBYTE(buffer->hasLiteralEncoding);
  inputPtr = (unsigned char*)&buffer->inputData[dictSize + 516];
  buffer->outputData[1] = LOBYTE(buffer->windowSize);
  buffer->outputPtr = 2;
  fun_pk_implode_memset(&buffer->outputData[2], 0, 2048u);
  buffer->currentOutputBitsUsed = 0;
  while ( 2 )
  {
    bufferFreeBytes = 4096;
    bufferUsedBytes = 0;
    while ( 1 )
    {                                           // fill buffer
      v5 = buffer->token;
      bytesToRead = bufferFreeBytes;
      token = buffer->inputFunc(&buffer->inputData[bufferUsedBytes + 516] + buffer->dictionarySize, &bytesToRead, v5);
      if ( !token )
        break;
      bufferUsedBytes += token;
      bufferFreeBytes -= token;
      if ( !bufferFreeBytes )
        goto lbl_bufferFull;
    }
    if ( !bufferUsedBytes && !hasDataMarker )
    {
      fun_pk_writeBits(buffer, buffer->copyLengthBits[517], buffer->copyLengthEof);
      if ( buffer->currentOutputBitsUsed )
        ++buffer->outputPtr;
      buffer->outputFunc(buffer->outputData, &buffer->outputPtr, buffer->token);
      return;
    }
    inputEndPtr[1] = (char *)1;                 // EOF but v19 is true
lbl_bufferFull:
    dictSz = buffer->dictionarySize;
    inputEndPtr[0] = &buffer->inputData[bufferUsedBytes] + dictSz;
    if ( inputEndPtr[1] )                       // EOF
      inputEndPtr[0] = &buffer->inputData[bufferUsedBytes + 516] + dictSz;
    if ( !hasDataMarker )
    {
      fun_pk_implode_analyzeInput(buffer, (unsigned __int8 *)inputPtr, (unsigned __int8 *)inputEndPtr[0] + 1);
      ++hasDataMarker;
      if ( buffer->dictionarySize == 4096 )
        goto lbl_startInputLoop;
LABEL_17:
      ++hasDataMarker;
      goto lbl_startInputLoop;
    }
    if ( hasDataMarker == 1 )
    {
      fun_pk_implode_analyzeInput(
        buffer,
        (unsigned __int8 *)&inputPtr[-dictSz + 516],
        (unsigned __int8 *)inputEndPtr[0] + 1);
      goto LABEL_17;
    }
    fun_pk_implode_analyzeInput(buffer, (unsigned __int8 *)&inputPtr[-dictSz], (unsigned __int8 *)inputEndPtr[0] + 1);
lbl_startInputLoop:
    while ( inputEndPtr[0] > (char *)inputPtr )
    {
      copyLength = fun_pk_implodeDetermineCopy(buffer, inputPtr);
      if ( copyLength )
      {
        while ( 1 )
        {
          if ( copyLength == 2 && buffer->copyOffset >= 0x100u )
            goto lbl_writeLiteralValue;
          if ( (unsigned __int64)(unsigned int)&inputPtr[copyLength] > *(__int64*)inputEndPtr )
            break;
          if ( (unsigned int)copyLength >= 8 || (char *)(inputPtr + 1) >= inputEndPtr[0] )
            goto lbl_writeCopyLengthBits_0;
          origCopyOffset = buffer->copyOffset;
          origCopyLength = copyLength;
          copyLength = fun_pk_implodeDetermineCopy(buffer, inputPtr + 1);
          if ( origCopyLength >= copyLength || origCopyLength + 1 >= copyLength && origCopyOffset <= 0x80 )
          {
            copyLength = origCopyLength;
            buffer->copyOffset = origCopyOffset;
lbl_writeCopyLengthBits_0:
            fun_pk_writeBits(buffer, buffer->literalBits[copyLength + 254], buffer->copyLengthValues[copyLength]);
            copyOffset = buffer->copyOffset;
            if ( copyLength == 2 )
            {
              fun_pk_writeBits(
                buffer,
                buffer->copyOffsetBits[(unsigned int)copyOffset >> 2],
                buffer->copyOffsetCode[(unsigned int)copyOffset >> 2]);
              inputPtr += 2;
              fun_pk_writeBits(buffer, 2u, buffer->copyOffset & 3);
            }
            else
            {
              fun_pk_writeBits(
                buffer,
                buffer->copyOffsetBits[(unsigned int)copyOffset >> LOBYTE(buffer->windowSize)],
                buffer->copyOffsetCode[(unsigned int)copyOffset >> LOBYTE(buffer->windowSize)]);
              inputPtr += copyLength;
              fun_pk_writeBits(buffer, buffer->windowSize, buffer->copyOffset & buffer->copyOffsetExtraMask);
            }
            goto lbl_continueInputLoop;
          }
          _HIWORD(inputByteEncodedValue) = 0;
          inputByte = (unsigned __int8)*inputPtr;
          _LOWORD(inputByteEncodedValue) = buffer->literalValues[inputByte];
          ++inputPtr;
          fun_pk_writeBits(buffer, buffer->literalBits[inputByte], inputByteEncodedValue);
          if ( !copyLength )
            goto lbl_writeLiteralValue;
        }
        copyLength = (signed int)&inputEndPtr[0][-(_DWORD)inputPtr];
        if ( (unsigned int)&inputEndPtr[0][-(_DWORD)inputPtr] >= 2
          && (&inputEndPtr[0][-(_DWORD)inputPtr] != (char *)2 || buffer->copyOffset < 0x100u) )
          goto lbl_writeCopyLengthBits_0;
      }
lbl_writeLiteralValue:
      _HIWORD(v12) = 0;
      v13 = (unsigned __int8)*inputPtr;
      _LOWORD(v12) = buffer->literalValues[v13];
      ++inputPtr;
      fun_pk_writeBits(buffer, buffer->literalBits[v13], v12);
lbl_continueInputLoop:
      ;
    }
    if ( !inputEndPtr[1] )                      // if (!EOF)
    {
      inputPtr -= 4096;
      fun_pk_implodeCopyMem(buffer->inputData, &buffer->inputData[4096], buffer->dictionarySize + 516);
      continue;
    }
    break;
  }
  fun_pk_writeBits(buffer, buffer->copyLengthBits[517], buffer->copyLengthEof);
  if ( buffer->currentOutputBitsUsed )
    ++buffer->outputPtr;
  buffer->outputFunc(buffer->outputData, &buffer->outputPtr, buffer->token);
}

signed int  fun_pk_implodeDetermineCopy(struct PKCompBuffer *buffer, _BYTE *inputPtr)
{
  signed int result; // eax@1
  unsigned __int16 *analyze2ptr; // edx@1
  __int16 *analyze1ptr; // edi@1
  unsigned int minMatchIndex; // esi@1
  int analyze1value; // ecx@1
  __int16 *v7; // edi@4
  _BYTE *startMatch; // esi@4
  _BYTE *v9; // ecx@6
  _BYTE *v10; // ecx@9
  signed int numMatchedBytes; // ebx@9
  _BYTE *v12; // esi@9
  char v13; // dl@10
  unsigned __int16 *v14; // ebx@20
  __int16 offset9BC_0; // di@21
  signed int v16; // esi@26
  unsigned __int16 *v17; // ebx@29
  unsigned char *v18; // ecx@30
  __int16 *v19; // esi@35
  signed __int16 index_9BC; // [sp+12h] [bp-12h]@21
  unsigned char *v21; // [sp+14h] [bp-10h]@26
  int analyze1valueNew; // [sp+18h] [bp-Ch]@1
  unsigned __int8 *prevInputPtr; // [sp+1Ch] [bp-8h]@4
  char v24; // [sp+20h] [bp-4h]@32

  result = 1;
  _HIWORD(analyze1value) = 0;
  analyze1ptr = &buffer->analyze1[5 * inputPtr[1] + 4 * *inputPtr];
  _LOWORD(analyze1value) = *analyze1ptr;
  minMatchIndex = &inputPtr[-buffer->dictionarySize] - (_BYTE *)buffer - 10187;
  analyze1valueNew = analyze1value;
  analyze2ptr = (unsigned __int16 *)&buffer->analyze2[analyze1value];
  if ( *analyze2ptr < minMatchIndex )
  {
    do
    {
      ++analyze2ptr;
      ++analyze1valueNew;
    }
    while ( *analyze2ptr < minMatchIndex );
    *analyze1ptr = analyze1valueNew;
  }
  prevInputPtr = inputPtr - 1;
  v7 = (short*)&buffer->analyze2[analyze1valueNew];
  startMatch = (unsigned char*)&buffer->inputData[(unsigned __int16)*v7];
  if ( inputPtr - 1 > startMatch )
  {
    v9 = inputPtr;
    while ( 1 )
    {
      if ( startMatch[result - 1] == v9[result - 1] )
      {
        if ( *startMatch == *v9 )
        {
          v12 = startMatch + 1;
          v10 = v9 + 1;
          numMatchedBytes = 2;
          do
          {
            v13 = v10[1];
            ++v12;
            ++v10;
            if ( *v12 != v13 )
              break;
            ++numMatchedBytes;
          }
          while ( numMatchedBytes < 516 );
          v9 = inputPtr;
          if ( numMatchedBytes >= result )
          {
            buffer->copyOffset = numMatchedBytes + inputPtr - v12 - 1;
            result = numMatchedBytes;
            if ( numMatchedBytes > 10 )
              break;
          }
        }
      }
      ++v7;
      ++analyze1valueNew;
      startMatch = (unsigned char*)&buffer->inputData[(unsigned __int16)*v7];
      if ( prevInputPtr <= startMatch )
      {
        if ( result < 2 )
          result = 0;
        return result;
      }
    }
    if ( numMatchedBytes == 516 )
    {
      result = 516;
      --buffer->copyOffset;
    }
    else
    {
      v14 = (unsigned __int16 *)((char *)buffer + 2 * analyze1valueNew);
      if ( (unsigned int)&buffer->inputData[v14[9449]] < (unsigned int)prevInputPtr )
      {
        offset9BC_0 = 0;
        buffer->field_9BC[0] = -1;
        index_9BC = 1;
        buffer->field_9BC[1] = 0;
        do
        {
          if ( inputPtr[index_9BC] != inputPtr[offset9BC_0] )
          {
            offset9BC_0 = buffer->field_9BC[offset9BC_0];
            if ( offset9BC_0 != -1 )
              continue;
          }
          ++index_9BC;
          ++offset9BC_0;
          buffer->field_9BC[index_9BC] = offset9BC_0;
        }
        while ( index_9BC < result );
        v16 = result;
        v21 = (unsigned char*)&buffer->inputData[result] + v14[9448];
LABEL_27:
        v16 = buffer->field_9BC[v16];
        if ( v16 == -1 )
          v16 = 0;
        v17 = (unsigned __int16 *)&buffer->analyze2[analyze1valueNew];
        while ( 1 )
        {
          ++v17;
          ++analyze1valueNew;
          v18 = (unsigned char*)&buffer->inputData[*v17];
          if ( (unsigned int)v18 >= (unsigned int)prevInputPtr )
            break;
          if ( &v18[v16] >= v21 )
          {
            v24 = inputPtr[result - 2];
            if ( v18[result - 2] != v24 )
            {
              v19 = (short*)&buffer->analyze2[analyze1valueNew];
              while ( 1 )
              {
                ++v19;
                ++analyze1valueNew;
                v18 = (unsigned char*)&buffer->inputData[(unsigned __int16)*v19];
                if ( (unsigned int)v18 >= (unsigned int)prevInputPtr )
                  return result;
                if ( v18[result - 2] == v24 && *inputPtr == *v18 )
                {
                  v16 = 2;
                  v21 = v18 + 2;
                  goto LABEL_40;
                }
              }
            }
            if ( &v18[v16] != v21 )
            {
              v16 = 0;
              v21 = (unsigned char*)&buffer->inputData[*v17];
            }
LABEL_40:
            while ( inputPtr[v16] == *v21 )
            {
              ++v16;
              if ( v16 >= 516 )
                break;
              ++v21;
            }
            if ( v16 >= result )
            {
              buffer->copyOffset = inputPtr - v18 - 1;
              if ( v16 > result )
              {
                result = v16;
                if ( v16 == 516 )
                  return result;
                do
                {
                  if ( inputPtr[index_9BC] != inputPtr[offset9BC_0] )
                  {
                    offset9BC_0 = buffer->field_9BC[offset9BC_0];
                    if ( offset9BC_0 != -1 )
                      continue;
                  }
                  ++index_9BC;
                  ++offset9BC_0;
                  buffer->field_9BC[index_9BC] = offset9BC_0;
                }
                while ( index_9BC < v16 );
              }
            }
            goto LABEL_27;
          }
        }
      }
    }
  }
  else
  {
    result = 0;
  }
  return result;
}


void  fun_pk_implode_flushFullBuffer(struct PKCompBuffer *buffer)
{
  int outputPtr; // eax@1
  char byte2048; // bl@1
  int indexOfLastByte; // eax@1
  char byteLast; // [sp+Fh] [bp-5h]@1
  int bytesToWrite; // [sp+10h] [bp-4h]@1

  bytesToWrite = 2048;
  buffer->outputFunc(buffer->outputData, &bytesToWrite, buffer->token);
  byte2048 = buffer->outputData[2048];
  indexOfLastByte = buffer->outputPtr - 2048;
  byteLast = buffer->outputData[indexOfLastByte + 2048];
  buffer->outputPtr = indexOfLastByte;
  fun_pk_implode_memset(buffer->outputData, 0, 0x802u);
  outputPtr = buffer->outputPtr;
  if ( outputPtr )
    buffer->outputData[0] = byte2048;
  if ( buffer->currentOutputBitsUsed )
    buffer->outputData[outputPtr] = byteLast;
}

int fun_pk_explode(int ( *inputFunc)(char *, int *, struct PKToken *),
                   void ( *outputFunc)(char *, int *, struct PKToken *), PKDecompBuffer *buf, PKToken *token)
{
  int v4; // eax@1
  int result; // eax@2
  int windowSize; // edx@3
  int hasLiteralEncoding; // eax@5

  buf->inputFunc = inputFunc;
  buf->outputFunc = outputFunc;
  buf->token = token;
  buf->inputBufferPtr = 2048;
  v4 = buf->inputFunc(buf->inputBuffer, &buf->inputBufferPtr, buf->token);
  buf->inputBufferEnd = v4;
  if ( (unsigned int)v4 <= 4 )
    return 3;
  buf->hasLiteralEncoding = buf->inputBuffer[0];
  buf->windowSize = buf->inputBuffer[1];
  buf->currentInputByte = buf->inputBuffer[2];
  buf->currentInput_bitsAvailable = 0;
  buf->inputBufferPtr = 3;
  windowSize = buf->windowSize;
  if ( (unsigned int)windowSize < 4 || (unsigned int)windowSize > 6 )
  {
    result = 1;
  }
  else
  {
    buf->dictionarySize = 65535 >> (16 - windowSize);
    hasLiteralEncoding = buf->hasLiteralEncoding;
    if ( hasLiteralEncoding )
    {
      if ( hasLiteralEncoding != 1 )
        return 2;
      fun_pk_explodeCopyMem(buf->literalDecode5, pk_literalTable, 0x100u);
      fun_pk_constructLiteralDecoder(buf);
    }
    fun_pk_explodeCopyMem(buf->copyLengthBaseBits, pk_copyLengthBaseBits, 0x10u);
    fun_pk_constructTable(16, buf->copyLengthBaseBits, pk_copyLengthBaseCode, buf->copyLengthTable);
    fun_pk_explodeCopyMem(buf->copyLengthExtraBits, pk_copyLengthExtraBits, 0x10u);
    fun_pk_explodeCopyMem(buf->copyLengthBaseValue, (char *)pk_copyLengthBaseValue, 0x20u);
    fun_pk_explodeCopyMem(buf->copyOffsetBits, pk_copyOffsetBits, 0x40u);
    fun_pk_constructTable(64, buf->copyOffsetBits, pk_copyOffsetCode, buf->copyOffsetTable);
    result = (unsigned int)(fun_pk_explodeData(buf) - 774) < 1 ? 4 : 0;
  }
  return result;
}

void  fun_pk_implode_analyzeInput(struct PKCompBuffer *buffer, unsigned __int8 *inputStartPtr, unsigned __int8 *inputEndPtr)
{
  unsigned __int8 *inputPtr; // eax@1
  __int16 *hashPtr; // ebx@1
  __int16 v5; // bp@1
  unsigned __int8 *inputPtr2; // esi@4
  int hashvalue; // eax@5
  _WORD inputIndex; // di@5
  __int16 *hashPtr2; // eax@5
  __int16 v10; // cx@5

  hashPtr = buffer->analyze1;
  v5 = 0;
  memset(buffer->analyze1, 0, sizeof(buffer->analyze1));
  inputPtr = inputStartPtr;
  do
  {
    ++*(&buffer->analyze1[5 * inputPtr[1]] + 4 * *inputPtr);
    ++inputPtr;
  }
  while ( inputEndPtr > inputPtr );
  do
  {
    v5 += *hashPtr;
    ++hashPtr;
    *(hashPtr - 1) = v5;
  }
  while ( (unsigned int)&buffer->field_1FC8 > (unsigned int)hashPtr );
  inputPtr2 = inputEndPtr - 1;
  do
  {
    hashvalue = 8 * *inputPtr2 + 10 * inputPtr2[1];
    inputIndex = (_WORD)( (inputPtr2--) - (unsigned char*)buffer - 0x27CC );
    hashPtr2 = (__int16 *)((char *)buffer->analyze1 + hashvalue);
    v10 = *hashPtr2 - 1;
    *hashPtr2 = v10;
    buffer->analyze2[(unsigned __int16)v10] = inputIndex;
  }
  while ( inputStartPtr <= inputPtr2 );
}
