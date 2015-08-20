#ifndef __ideas_h__
#define __ideas_h__

// The following code can be used to output
// formatted strings to the iDeaS debug console.
// iDeaS is a Nintendo DS emulator, see http://www.ideasemu.org
//
// It detects if it is executed on real hardware
// and discards all calls if so.
// In order to remove the entire debug output facility,
// set _RELEASE to 1, either with #define _RELEASE 1 or
// through flags via the compiler command line.
//
// 2009-05-17 by Peter Schraut, http://www.console-dev.de
//

#if __cplusplus
  extern "C" {
#endif

#if !_RELEASE

int IdeasEnableDebugOutput(int enable);
int IdeasOutputDebugString(const char* format, ...);

#else

// in release mode use empty functions, so the compiler
// optimizes any call to them away.
inline int IdeasEnableDebugOutput(int /*enable*/) { return 0; }
inline int IdeasOutputDebugString(const char* /*format*/, ...) { return 0; }

#endif // _RELEASE

int IdeasIsEmulator();

#if __cplusplus
  } // extern "C"
#endif

#endif // __ideas_h__
