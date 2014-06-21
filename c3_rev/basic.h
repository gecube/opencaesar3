#ifndef __C3_BASIC_INCLUDE__
#define __C3_BASIC_INCLUDE__

int  fun_getStringWidth(const char *str, int fontId);
int  fun_strlen(const char *str);
int  fun_getCharWidth(unsigned __int8 c, int fontId);
void  unused_addToGameTextString(char *str, int group, int number, signed int len);
void  fun_strMoveLeft(char *start, const char *end);
int  fun_strToInt(char *str);
int  fun_strNumDigitChars(char *str);

#endif

