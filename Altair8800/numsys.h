#ifndef NUMSYS_H
#define NUMSYS_H

#define NUMSYS_OCT 0
#define NUMSYS_DEC 1
#define NUMSYS_HEX 2

void     numsys_set(byte sys);
void     numsys_toggle();
void     numsys_print_word(uint16_t w);
void     numsys_print_byte(byte b);
void     numsys_print_mem(uint16_t addr, byte num, bool printBrackets);
uint16_t numsys_read_word();

#endif
