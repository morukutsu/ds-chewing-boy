#include "ideas.h"
#include <stdio.h>
#include <stdarg.h>

#if !_RELEASE

static int IdeasDebugOutputEnabled=1;

// Gets if the program runs in an emulator.
__attribute__ ((noinline))
int IdeasIsEmulator()
{
// The idea behind the code is to overwrite
// the "mov r0, #0" instruction with "mov r0, r0" (NOP).
// On real hardware, the instruction would have been fetched,
// so the overwrite has no effect for the first time executed.
// In order to be able to call the function more than once,
// the original instruction is being restored.
// see http://forum.gbadev.org/viewtopic.php?t=910
#ifdef __thumb__
  int mov_r0_r0 = 0x1c00;   // mov r0, r0
  int mov_r0_0  = 0x2000;   // mov r0, #0

  asm volatile (
    "mov  r0, %1     \n\t"  // r0 = mov_r0_r0
    "mov  r2, %2     \n\t"  // r2 = mov_r0_0
    "mov  r1, pc     \n\t"  // r1 = program counter
    "strh r0, [r1]   \n\t"  // Overwrites following instruction with mov_r0_r0
    "mov  r0, #0     \n\t"  // r0 = 0
    "strh r2, [r1]   \n\t"  // Restore previous instruction
    : "=r"(mov_r0_r0)       // output registers
    : "r"(mov_r0_r0), "r"(mov_r0_0)    // input registers
    : "%r1","%r2"           // clobbered registers
    );

#else
  int mov_r0_r0 = 0xe1a00000; // mov r0, r0
  int mov_r0_0  = 0xe3a00000; // mov r0, #0

  asm volatile (
    "mov  r0, %1     \n\t"  // r0 = mov_r0_r0
    "mov  r2, %2     \n\t"  // r2 = mov_r0_0
    "mov  r1, pc     \n\t"  // r1 = program counter
    "str  r0, [r1]   \n\t"  // Overwrites following instruction with mov_r0_r0
    "mov  r0, #0     \n\t"  // r0 = 0
    "str  r2, [r1]   \n\t"  // Restore previous instruction
    : "=r"(mov_r0_r0)       // output registers
    : "r"(mov_r0_r0), "r"(mov_r0_0)    // input registers
    : "%r1","%r2"           // clobbered registers
    );
#endif

  return mov_r0_r0 != 0;
}

// This function must be noinline, because
// iDeaS expects the text to output in register r0.
// If this code is inlined somewhere, it's not guaranteed
// that text is located in r0 anymore, thus will not work.
static __attribute__ ((noinline))
void IdeasOutputDebugStringInternal(const char* text)
{
#ifdef __thumb__
  asm volatile ("swi #0xfc");
#else
  asm volatile ("swi #0xfc000");
#endif
}


// Prints formatted output to the iDeaS debug console
// Returns false when text has not been printed, true otherwise.
int IdeasOutputDebugString(const char* format, ...)
{
  va_list args;
  char    buffer[128]; // increase to support more characters

  if(!IdeasDebugOutputEnabled || !IdeasIsEmulator())
    return 0;

  va_start(args,0);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  IdeasOutputDebugStringInternal(buffer);
  return 1;
}

// Enables or disables debug output.
// Returns the previous enabled state.
int IdeasEnableDebugOutput(int enable)
{
  int old = IdeasDebugOutputEnabled;
  IdeasDebugOutputEnabled = enable;
  return old;
}

#endif // !_RELEASE


/*
void IdeasDebugOutputTest()
{
  char ch = 'h';
  const char  *string = "computer";
  int count = -9234;
  double fp = 251.7366;

  // Display integers.
  IdeasOutputDebugString( "Integer formats: Decimal: %d  Justified: %.6d  Unsigned: %u", count, count, count, count );
  IdeasOutputDebugString( "Decimal %d as: Hex: %Xh  C hex: 0x%x  Octal: %o", count, count, count, count );

  // Display in different radixes.
  IdeasOutputDebugString( "Digits 10 equal: Hex: %i  Octal: %i  Decimal: %i", 0x10, 010, 10 );

  // Display characters.
  IdeasOutputDebugString("Characters in field (1): %10c%5hc", ch, ch);

  // Display strings.
  IdeasOutputDebugString("Strings in field (1): %25s %25.4hs", string, string);

  // Display real numbers.
  IdeasOutputDebugString( "Real numbers: %f %.2f %e %E", fp, fp, fp, fp );

  // Display pointer.
  IdeasOutputDebugString( "Address as:   %p\n", &count);

  // Count characters printed.
  IdeasOutputDebugString( "Display to here:" );
  IdeasOutputDebugString( "1234567890123456%n78901234567890\n", &count );
  IdeasOutputDebugString( "   Number displayed: %d", count );
}
*/

