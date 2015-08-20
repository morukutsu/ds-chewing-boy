//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Sprite files : 
#include "Pointer.c"

// Background files : 
#include "Hero.c"

// Palette files : 
#include "Pointer.pal.c"
#include "Hero.pal.c"

// Background Pointers :
PAGfx_struct Hero = {(void*)Hero_Map, 768, (void*)Hero_Tiles, 1728, (void*)Hero_Pal, (int*)Hero_Info };


#ifdef __cplusplus
}
#endif

