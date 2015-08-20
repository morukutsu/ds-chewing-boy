/* --------------------------- */
/* Obsolete                    */
/* --------------------------- */

#ifndef _BACKBUFFER_H
#define _BACKBUFFER_H

#include "Platform.h"

#define EFFECT_FIRE    1
#define EFFECT_NEON    2

void InitBackBuffer();
void ClearBackBuffer();
void UpdateBackBuffer(int effect);

#endif