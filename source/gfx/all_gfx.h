//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif


// Sprite files : 
extern const unsigned char Pointer_Sprite[256] __attribute__ ((aligned (4))) ;  // Pal : Pointer_Pal

// Background files : 
extern const int Hero_Info[3]; // BgMode, Width, Height
extern const unsigned short Hero_Map[768] __attribute__ ((aligned (4))) ;  // Pal : Hero_Pal
extern const unsigned char Hero_Tiles[1728] __attribute__ ((aligned (4))) ;  // Pal : Hero_Pal
extern PAGfx_struct Hero; // background pointer


// Palette files : 
extern const unsigned short Pointer_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short Hero_Pal[3] __attribute__ ((aligned (4))) ;


#endif

