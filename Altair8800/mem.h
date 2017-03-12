#ifndef MEM_H
#define MEM_H

#include "config.h"
#include "host.h"
#include "prog_basic.h"
#include "breakpoint.h"

extern byte Mem[MEMSIZE];
extern word mem_have_rom_basic;

#if USE_PROTECT>0

extern word protected_flag;
extern byte protected_flags[32];
#define MEM_IS_PROTECTED(a) (protected_flag ? (protected_flags[((a)>>8)/8] & (1<<(((a)>>8)&0x07))) : 0)

void mem_protect(uint16_t a);
void mem_unprotect(uint16_t a);

#else
#define MEM_IS_PROTECTED(a) false
#define mem_protect(a)      while(0)
#define mem_unprotect(a)    while(0)
#endif

#if MEMSIZE < 0x10000
#define MREAD(a)    ((a)>=0xC000 && mem_have_rom_basic ? prog_basic_read_16k(a) : ((a) < MEMSIZE ? Mem[a] : 0x00))
#define MWRITE(a,v) {if( (a) < MEMSIZE && !MEM_IS_PROTECTED(a) ) Mem[a]=v;}
#else
// If we have 64k memory then we just copy ROM basic to the upper 16k and write-protect
// that area.  Faster to check the address on writing than reading since there are far more
// reads than writes. Also we can skip memory bounds checking because addresses are 16 bit.
#define MREAD(a)    (Mem[a])
#define MWRITE(a,v) {if( ((a)<0xC000 || !mem_have_rom_basic) && !MEM_IS_PROTECTED(a) ) Mem[a]=v; }
#endif

// WARNING: arguments to MEM_READ and MEM_WRITE macros should not have side effects
// (e.g. MEM_READ(addr++)) => any side effects will be executed multiple times!
#define MEM_READ(a)       ( host_read_status_led_WAIT() ? MEM_READ_STEP(a) :         ( host_set_status_leds_READMEM(),  host_set_addr_leds(a), host_set_data_leds(MREAD(a)) ))
#define MEM_WRITE(a, v) if( host_read_status_led_WAIT() ) MEM_WRITE_STEP(a, v); else { host_set_status_leds_WRITEMEM(); host_set_addr_leds(a); MWRITE(a, v); }

byte MEM_READ_STEP(uint16_t a);
void MEM_WRITE_STEP(uint16_t a, byte v);

void mem_setup();

#endif
