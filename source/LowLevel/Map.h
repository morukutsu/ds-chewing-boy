/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_MAP
#define H_MAP

#include "Drawable.h"
#include <ulib/ulib.h>
#include <string>
#include "../xml/tinyxml.h"
#include "Sprite.h"
#include "../System/RessourceManager.h"
#include <stdio.h>
#include <stdlib.h>

struct sParallaxElement
{
	int x, y, tileID;
};

extern u16 sMapArray[512*512];
extern sParallaxElement sPElemArray[64];

inline int GetTileX(u16 tile)
{
	return (tile >> 12) & 0xF;
}

inline int GetTileY(u16 tile)
{
	return (tile >> 8) & 0xF;
}

inline bool GetTileMirrorX(u16 tile)
{
	return (tile >> 7) & 0x1;
}

inline bool GetTileMirrorY(u16 tile)
{
	return (tile >> 6) & 0x1;
}

inline int GetTileType(u16 tile)
{
	return (tile >> 2) & 0xF;
}

u16 MakeTile(int x, int y, int mx, int my, int type);

class WorldManagerOffline;
namespace LowLevel
{
    class Map : public Drawable
    {
		public:
			Map();
			~Map();
			
			void Cull();
			void DrawMap(int layer);
			void Draw();
			int GetMapPixel(int x, int y);

			void Load(char* filename);
			void LoadLayer(const unsigned char* bLayer, int layerID);

		public:
			bool mMapLoaded;
			int  mMapWidth, mMapHeight;
			int  mMapWidthHeight;
			std::string mMapName, mMapDataFilename, mMapSlopeFilename;
			int  mChipset, mFx, mMusique;
			int pElemCount;

			int cullX1, cullX2, cullY1, cullY2;
			Sprite *mChipsets[3];
			Image *mChipsetColl;
			WorldManagerOffline *worldMan;
    };

}

#endif
