#include "variables.h"
#include "basic.h"

int  fun_getStringWidth(const char *str, int fontId)
{
  unsigned __int8 c; // [sp+4Ch] [bp-14h]@12
  signed int letterSpacing; // [sp+50h] [bp-10h]@3
  signed int spaceWidth; // [sp+54h] [bp-Ch]@3
  int stringWidth; // [sp+58h] [bp-8h]@1
  signed int maxlen; // [sp+5Ch] [bp-4h]@1

  maxlen = 10000;
  stringWidth = 0;
  if ( fontId != graphic_font + F_LargePlain && fontId != graphic_font + F_LargeBlack )
  {
    if ( fontId == graphic_font + F_LargeBrown )
    {
      spaceWidth = 10;
      letterSpacing = 1;
    }
    else
    {
      if ( fontId == graphic_font + F_SmallPlain )
      {
        spaceWidth = 4;
        letterSpacing = 1;
      }
      else
      {
        if ( fontId == graphic_font )
        {
          spaceWidth = 6;
          letterSpacing = 1;
        }
        else
        {
          spaceWidth = 6;
          letterSpacing = 0;
        }
      }
    }
  }
  else
  {
    spaceWidth = 10;
    letterSpacing = 1;
  }
  while ( maxlen > 0 )
  {
    c = *str++;
    if ( !c )
      return stringWidth;
    if ( c == ' ' )
    {
      stringWidth += spaceWidth;
    }
    else
    {
      graphic_currentGraphicId = (unsigned __int8)map_char_to_fontGraphic[c];
      if ( graphic_currentGraphicId )
        stringWidth += letterSpacing + c3_sg2[fontId + graphic_currentGraphicId - 1].width;
    }
    --maxlen;
  }
  return stringWidth;
}

int  fun_strlen(const char *str)
{
  char c; // ST4C_1@3
  int len; // [sp+50h] [bp-8h]@1
  signed int max; // [sp+54h] [bp-4h]@1

  max = 10000;
  len = 0;
  while ( max > 0 )
  {
    c = *str++;
    if ( !c )
      return len;
    ++len;
    --max;
  }
  return len;
}

int  fun_getCharWidth(unsigned __int8 c, int fontId)
{
  int result; // eax@2

  if ( c )
  {
    if ( c == ' ' )
    {
      result = 4;
    }
    else
    {
      graphic_currentGraphicId = (unsigned __int8)map_char_to_fontGraphic[c];
      if ( graphic_currentGraphicId )
        result = c3_sg2[fontId + graphic_currentGraphicId - 1].width + 1;
      else
        result = 0;
    }
  }
  else
  {
    result = 0;
  }
  return result;
}

int  fun_strToInt(char *str)
{
  int result; // eax@11
  char *ptr; // [sp+4Ch] [bp-14h]@1
  char *v3; // [sp+4Ch] [bp-14h]@6
  signed int negative; // [sp+50h] [bp-10h]@1
  int v5; // [sp+58h] [bp-8h]@1
  int numChars; // [sp+5Ch] [bp-4h]@1

  ptr = str;
  negative = 0;
  v5 = 0;
  numChars = 0;
  if ( (unsigned __int8)*str == '-' )
  {
    negative = 1;
    ptr = str + 1;
  }
  while ( (signed int)(unsigned __int8)*ptr >= '0' && (signed int)(unsigned __int8)*ptr <= '9' )
  {
    ++numChars;
    ++ptr;
  }
  v3 = str;
  if ( (unsigned __int8)*str == '-' )
    v3 = str + 1;
  while ( numChars )
  {
    --numChars;
    v5 += atoi_multipliers[numChars] * ((unsigned __int8)*v3++ - '0');
  }
  if ( negative )
    result = -v5;
  else
    result = v5;
  return result;
}

int  fun_strNumDigitChars(char *str)
{
  signed int check; // [sp+4Ch] [bp-8h]@1
  int numChars; // [sp+50h] [bp-4h]@1

  numChars = 0;
  check = 0;
  while ( 1 )
  {
    ++check;
    if ( check >= 1000 )
      break;
    if ( (signed int)(unsigned __int8)*str >= '0' && (signed int)(unsigned __int8)*str <= '9'
      || (unsigned __int8)*str == '-' )
      break;
    ++str;
    ++numChars;
  }
  return numChars;
}

void  unused_addToGameTextString(char *str, int group, int number, signed int len)
{
  unsigned __int8 i; // [sp+4Ch] [bp-10h]@9
  char *target; // [sp+54h] [bp-8h]@1

  target = (char *)&c3eng_data
         + 256 * (unsigned __int8)byte_6ADEFE[4 * group]
         + (unsigned __int8)byte_6ADEFF[4 * group]
         + 28;
  while ( number > 0 )
  {
    if ( !*target )
    {
      if ( (signed int)(unsigned __int8)*(target - 1) >= ' ' )
        --number;
    }
    ++target;
  }
  while ( (signed int)(unsigned __int8)*target < ' ' )
    ++target;
  for ( i = 0; i < len; ++i )
    target[i] = str[i];
}

int  unused_copyGameTextString(char *dst, int group, int number, signed int maxlen)
{
  int result; // eax@10
  unsigned __int8 i; // [sp+4Ch] [bp-8h]@9
  char *text; // [sp+50h] [bp-4h]@1

  text = (char *)&c3eng_data + c3eng_index[group].offset;
  while ( number > 0 )
  {
    if ( !*text )
    {
      if ( (signed int)(unsigned __int8)*(text - 1) >= 32 )
        --number;
    }
    ++text;
  }
  while ( (signed int)(unsigned __int8)*text < 32 )
    ++text;
  for ( i = 0; ; ++i )
  {
    result = i;
    if ( i >= maxlen )
      break;
    dst[i] = text[i];
  }
  return result;
}

void  fun_getGameTextString_forMessagebox(int group, int number)
{
  c3eng_textstring_forMessagebox = (char *)&c3eng_data + c3eng_index[group].offset;
  while ( number > 0 )
  {
    if ( !*c3eng_textstring_forMessagebox )
    {
      if ( (signed int)(unsigned __int8)*(c3eng_textstring_forMessagebox - 1) >= ' '
        || !*(c3eng_textstring_forMessagebox - 1) )
        --number;
    }
    ++c3eng_textstring_forMessagebox;
  }
  while ( (signed int)(unsigned __int8)*c3eng_textstring_forMessagebox < ' ' )
    ++c3eng_textstring_forMessagebox;
}

int  fun_getGameTextStringWidth(int group, int number, int fontId)
{
  gametext_result = (char *)&c3eng_data + c3eng_index[group].offset;
  while ( number > 0 )
  {
    if ( !*gametext_result )
    {
      if ( (signed int)(unsigned __int8)*(gametext_result - 1) >= 32 || !*(gametext_result - 1) )
        --number;
    }
    ++gametext_result;
  }
  while ( (signed int)(unsigned __int8)*gametext_result < 32 )
    ++gametext_result;
  return fun_getStringWidth(gametext_result, fontId);
}

void  fun_strMoveLeft(char *start, const char *end)
{
  while ( start < end )
  {
    *start = start[1];
    ++start;
  }
  *start = 0;
}

void  unused_inputRemoveSpaces()
{
  int i; // [sp+4Ch] [bp-4h]@1

  for ( i = 0; i <= input_length[inputtext_lastUsed] && input_text[inputtext_lastUsed][i]; ++i )
  {
    if ( (unsigned __int8)input_text[inputtext_lastUsed][i] == ' ' )
      fun_strMoveLeft(
        &input_text[inputtext_lastUsed][i],
        &input_text[inputtext_lastUsed][input_length[inputtext_lastUsed]]);
  }
}
