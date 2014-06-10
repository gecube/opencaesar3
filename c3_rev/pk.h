#ifndef __PK_INCLUDE__
#define __PK_INCLUDE__

#include "defs.h"

int  fun_pkDecompress(int *hashcode, char *compressedData, int compressedLength, char *destBuffer, int destLength);

signed int  fun_pk_implode(int ( *inputFunc)(char *, int *, struct PKToken *),
                                  void ( *outputFunc)(char *, int *, struct PKToken *),
                                  PKCompBuffer *buffer, struct PKToken *token, int *hasLiteralEncoding, int *dictionarySize);

void  fun_pk_implodeData(struct PKCompBuffer *buffer);
signed int  fun_pk_implodeDetermineCopy(struct PKCompBuffer *buffer, _BYTE *inputPtr);
void  fun_pk_implode_flushFullBuffer(struct PKCompBuffer *buffer);
void  fun_pk_implode_analyzeInput(struct PKCompBuffer *buffer, unsigned __int8 *inputStartPtr, unsigned __int8 *inputEndPtr);
int fun_pk_explode(int ( *inputFunc)(char *, int *, struct PKToken *),
                   void ( *outputFunc)(char *, int *, struct PKToken *), PKDecompBuffer *buf, PKToken *token);
int  fun_pk_explodeData(struct PKDecompBuffer *buf);
int  fun_pk_decodeNextToken(struct PKDecompBuffer *buf);
int  fun_pk_getCopyOffset(struct PKDecompBuffer *buf, int copylength);
void  fun_pk_implode_memset(void *buffer, unsigned __int8 fillChar, unsigned int length);
signed int  fun_pk_setBitsUsed(struct PKDecompBuffer *buf, unsigned int numBits);
void  fun_pk_constructTable(int size, char *a2, char *table, char *a4);
void  fun_pk_implodeCopyMem(char *dst, const char *src, unsigned int length);
void  fun_pk_writeBits(struct PKCompBuffer *buffer, unsigned int numBits, unsigned int value);
void  fun_pk_constructLiteralDecoder(struct PKDecompBuffer *buf);
void  fun_pk_explodeCopyMem(char *dst, char *src, unsigned int length);
int  fun_pkCompress(int *hashcode, int *compressedSize, char *inputBuffer, char *outputBuffer, int inputLength);
int  fun_pk_hashBuffer(char *a1, int a2, int *a3);
void  fun_pk_outputFunc(char *Src, int *a2, struct PKToken *a3);
int  fun_pk_inputFunc(char *buffer, int *length, struct PKToken *token);
#endif
