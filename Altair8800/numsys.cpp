#include <Arduino.h>
#include "numsys.h"
#include "mem.h"

static byte numsys = NUMSYS_HEX;

static byte hexToDec(int hc)
{
  if( hc>96 ) hc -= 32;

  if( hc >= 65 && hc <= 70 )
    return hc - 65 + 10;
  else if( hc >= 48 && hc <= 57 )
    return hc - 48;

  return 255;
}


void numsys_toggle()
{
  numsys = (numsys+1) % 3;
}

void numsys_set(byte sys)
{
  numsys = sys;
}


void numsys_print_byte(byte b)
{
  if( numsys==NUMSYS_HEX )
    {
      static char hexchars[17] = "0123456789ABCDEF";
      Serial.write(hexchars[b>>4]);
      Serial.write(hexchars[b&0x0f]);
    }
  else if( numsys==NUMSYS_OCT )
    {
      byte d;
      d = (b&0700) >> 6;
      Serial.print(d);
      d = (b&0070) >> 3;
      Serial.print(d);
      d = (b&0007);
      Serial.print(d);
    }
  else
    Serial.print(b);
}


void numsys_print_word(uint16_t w)
{
  if( numsys==NUMSYS_HEX )
    {
      numsys_print_byte(w >> 8);
      numsys_print_byte(w & 0xff);
    }
  else if( numsys==NUMSYS_OCT )
    {
      Serial.print((w>>15) & 007);
      Serial.print((w>>12) & 007);
      Serial.print((w>> 9) & 007);
      Serial.print((w>> 6) & 007);
      Serial.print((w>> 3) & 007);
      Serial.print( w      & 007);
    }
  else
    {
      Serial.print(w);
    }
}


void numsys_print_mem(uint16_t addr, byte num, bool printBrackets)
{
  byte i;
  if( printBrackets ) Serial.print('['); 
  for(i=0; i<num; i++)
    { numsys_print_byte(MREAD(addr+i)); if(i+1<num) Serial.print(' '); }
  if( printBrackets ) Serial.print(']'); 
}


uint16_t numsys_read_word()
{
  byte b;
  uint16_t w = 0;
  int c = -1;

  while( c!=13 && c!=10 && c!=32 && c!=9 && c!='-' && c!=':' )
    {
      c=-1;
      while(c<0) c = Serial.read();

      if( numsys==NUMSYS_HEX && (b=hexToDec(c))!=255 )
        {
          Serial.write(c);
          w = w << 4 | b;
        }
      else if( numsys==NUMSYS_OCT && c>=48 && c<=55 )
        {
          Serial.write(c);
          w = w << 3 | (c-48);
        }
      else if( c>=48 && c<=57 )
        {
          Serial.write(c);
          w = w * 10 + (c-48);
        }
    }

  return w;
}
