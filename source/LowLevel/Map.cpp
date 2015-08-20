/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#include "Map.h"
#include "../WorldManager.h"
#include "base64.h"

u16 sMapArray[512*512];
sParallaxElement sPElemArray[64];

/*int GetTileX(u16 tile)
{
	return (tile >> 12) & 0xF;
}

int GetTileY(u16 tile)
{
	return (tile >> 8) & 0xF;
}

bool GetTileMirrorX(u16 tile)
{
	return (tile >> 7) & 0x1;
}

bool GetTileMirrorY(u16 tile)
{
	return (tile >> 6) & 0x1;
}

int GetTileType(u16 tile)
{
	return (tile >> 2) & 0xF;
}*/

u16 MakeTile(int x, int y, int mx, int my, int type)
{
	//return (x << 24) | (y << 16) ;
	//return (x << 8) | y;
	//return (x << 4) | y;
	return (x << 12) | (y << 8) | (mx << 7) | (my << 6) | (type << 2);
}

namespace LowLevel
{
    Map::Map()
    {
		mMapLoaded = false;
		mMapWidth = mMapHeight = 0;
		pElemCount = 0;
    }

    Map::~Map()
    {
        //image->FreeImage();
    }
	
	void Map::Draw()
	{
		Cull();
		DrawMap(0);
		//DrawMap(1);
		//DrawMap(2);
	}

	void Map::Cull()
    {
		cullX1 = -posX/16;
		cullY1 = -posY/16;

		cullX2 = cullX1 + 16 + 1;
		cullY2 = cullY1 + 12 + 1;

		if(cullX1 <= 0)
			cullX1 = 0;
		if(cullX2 > mMapWidth)
			cullX2 = mMapWidth;
		if(cullY1 <= 0)
			cullY1 = 0;
		if(cullY2 > mMapHeight)
			cullY2 = mMapHeight;
		//ulDebug("cx1:%d cy1:%d cx2:%d cy2:%d\n", cullX1, cullY1, cullX2, cullY2);
	}

    void Map::DrawMap(int layer)
    {
		//Affichage de la map
		if(layer == 0 || layer == 1 || layer == 2)
		{
			for(int x = cullX1; x < cullX2; ++x)
			{
				for(int y = cullY1; y < cullY2; ++y)
				{
					u16 block = sMapArray[layer * (mMapWidthHeight) + (x * mMapHeight) + y]; //Un peu comme ax² + by + c...
					if(block != 0)
					{
						mChipsets[1]->MoveTo(posX + (x<<4), posY + (y<<4));
						mChipsets[1]->Clip(GetTileX(block)<<4, GetTileY(block)<<4, 16, 16);
						mChipsets[1]->Mirror(GetTileMirrorX(block), GetTileMirrorY(block) );
						mChipsets[1]->Draw();
					}
				}
			}
		}

		//Affichage du front parallax
		/*if(layer == 3)
		{
			mChipsets[0]->MoveTo(fxtoi(Mulfx(posX<<8, 384))%256, (mMapHeight<<4) -64 + posY);
			mChipsets[0]->Draw();
			mChipsets[0]->MoveTo(fxtoi(Mulfx(posX<<8, 384))%256+256, (mMapHeight<<4) -64 + posY);
			mChipsets[0]->Draw();
		}*/
    }

	void Map::Load(char* filename)
	{
		//B : On charge les datas
		
		

		
		/*FILE* fichier = fopen(tFullName.c_str(), "rb");
		for(int x = 0; x < mMapWidth; x++)
		{
			for(int y = 0; y < mMapHeight; y++)
			{
				for(int k = 0; k < 4; k++)
				{
					u16 * block = &sMapArray[k * (mMapWidth*mMapHeight) + (x * mMapHeight) + y];
					fread((void*)block, sizeof(u16), 1, fichier);
				}
			}
		}*/
		
		//u16 * block = &sMapArray[k][x][y];
		//fread((void*)sMapArray, sizeof(u16), 4*mMapWidth*mMapHeight, fichier);

		//C : Chargemment des chipsets correspondants
		/*std::string tFileBase = "efs:/data/chipsets/ch";
		std::string tFileEnd = ".png";
		char tFileNumber[8];
		sprintf(tFileNumber, "%d", mChipset);*/ 
		
		//mChipsets[0] = new Sprite();
		mChipsets[1] = new Sprite();
		//mChipsets[2] = new Sprite();
		//mChipsetColl = new Image();

		/*tTemp = tFileBase + tFileNumber + "_n" + tFileEnd;*/
		mChipsets[1]->Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/chipsets/ch_stand.png", UL_IN_VRAM, UL_PF_PAL8));
		mChipsets[1]->Scale(16, 16);

		mChipsetColl = System::CRessourceManager::getInstance()->LoadImage("efs:/data/chipsets/ch_stand_c.png" , UL_IN_RAM, UL_PF_PAL8);
	
		mMapLoaded = true;	
	}
	
	void Map::LoadLayer(const unsigned char* bLayer, int layerID)
	{
		//Décompression base64
		size_t out = 0;
		unsigned char* data = base64_decode(bLayer, strlen((const char*)bLayer), &out);
		int cptx, cpty;
		cptx = 0;
		cpty = 0;
		int gid, tx, ty;
		int max = 0;
		for(int i = 0; i < out; i += 4)
		{
			gid = data[i] | data[i + 1] << 8 | data[i + 2] << 16 |  data[i + 3] << 24;

			if(layerID == 0)
			{
				gid -= 1;
				ty = (gid)>>4;
				tx = (gid)%16;
				//tx = gid-(tx<<4);
				//Vérification de tiles spécial entités
				if(tx == 1 && ty == 0)
				{
					//Gemme
					worldMan->LoadEntity("gemme", (cptx*16)<<8, (cpty*16)<<8, 16, 16, 0, 0, 0);
					tx = ty = 0;
				}
				else if(tx == 2 && ty == 0)
				{
					//Gemme
					worldMan->LoadEntity("fullstar", (cptx*16)<<8, (cpty*16)<<8, 16, 16, 0, 0, 0);
					tx = ty = 0;
				}
				else if(tx == 3 && ty == 0)
				{
					//Peaks vers le haut
					worldMan->LoadEntity("peaks", (cptx*16)<<8, (cpty*16)<<8, 16, 16, 0, 0, 0);
					tx = ty = 0;
				}
				if(tx == 4 && ty == 0)
				{
					//Gemme
					worldMan->LoadEntity("dbloc", (cptx*16)<<8, (cpty*16)<<8, 16, 16, 0, 0, 0);
					tx = ty = 0;
				}
				else if(tx == 6 && ty == 0)
				{
					//Gemme
					worldMan->LoadEntity("womie", (cptx*16), (cpty*16), 16, 16, 0, 0, 0);
					tx = ty = 0;
				}
				else if(tx == 7 && ty == 0)
				{
					//Gemme
					worldMan->LoadEntity("wopic", (cptx*16), (cpty*16), 16, 16, 0, 0, 0);
					tx = ty = 0;
				}
				else if(tx == 8 && ty == 0)
				{
					//Gemme
					worldMan->LoadEntity("extralife", (cptx*16)<<8, (cpty*16)<<8, 16, 16, 0, 0, 0);
					tx = ty = 0;
				}
				else if(tx == 9 && ty == 0)
				{
					//Peaks vers le bas
					worldMan->LoadEntity("peaks", (cptx*16)<<8, (cpty*16)<<8, 16, 16, 1, 0, 0);
					tx = ty = 0;
				}
				else if(tx == 10 && ty == 0)
				{
					//Peaks vers le bas
					worldMan->LoadEntity("peaks", (cptx*16)<<8, (cpty*16)<<8, 16, 16, 2, 0, 0);
					tx = ty = 0;
				}
				else if(tx == 11 && ty == 0)
				{
					//Peaks vers le bas
					worldMan->LoadEntity("peaks", (cptx*16)<<8, (cpty*16)<<8, 16, 16, 3, 0, 0);
					tx = ty = 0;
				}
			}

			if(layerID == 3)
			{
				gid -= 257;
				ty = (gid)/8;
				tx = (gid)%8;
			}

			/*if(layerID == 3)
			{
				if(gid != 0)
				{
					ulDebug("gid : %d - tx : %d , ty : %d\n", gid, tx, ty);
				}
			}*/

			sMapArray[layerID * (mMapWidthHeight) + (cptx * mMapHeight) + cpty] = MakeTile(tx, ty, 0, 0, 0);
			if(max < layerID * (mMapWidthHeight) + (cptx * mMapHeight) + cpty)
				max = layerID * (mMapWidthHeight) + (cptx * mMapHeight) + cpty;
			cptx++;
			if(cptx > mMapWidth-1)
			{
				cptx = 0;
				cpty++;
			}
		}
		
		//Liberation mémoire
		free(data);

		//ulDebug("%d/%d\n", max, 512*512);
	}

	int Map::GetMapPixel(int x, int y)
	{
		if(x > 0 && x < mMapWidth*16 && y > 0 && y < mMapHeight*16)
		{
			if(worldMan->isAnyNonSensorOnScreen)
			{
				for(int k = 0; k < worldMan->mNonSensorOnScreenCount; k++)
				{
					if(RectPointCollision(x, y, worldMan->mBodies[worldMan->mNonSensorOnScreen[k]].x, worldMan->mBodies[worldMan->mNonSensorOnScreen[k]].y, worldMan->mBodies[worldMan->mNonSensorOnScreen[k]].aabb))
						return 2;
				}
			}

			//Variables
			int mx, my, u0, v0;
			u16 blocID;

			//On cherche sur quel bloc se trouve ce pixel de la map
			mx = x >> 4;
			my = y >> 4;
			blocID = sMapArray[3 * (mMapWidthHeight) + (mx * mMapHeight) + my];
			
			//On récupère la position du bloc dans le tileset de collisions
			u0 = GetTileX(blocID)<<4;
			v0 = GetTileY(blocID)<<4;
			
			//On regarde si le tile est flipped
			int diffx = x - (mx << 4);
			int diffy = y - (my << 4);

			/*if(GetTileMirrorX(blocID) == true)
				diffx = 15-diffx;
			if(GetTileMirrorY(blocID) == true)
				diffy = 15-diffy;*/
			u0 += diffx;
			v0 += diffy;

			//On récupère le pixel
			void* pPixel = ulGetImageLineAddr(mChipsetColl->img, v0);

			return ((u8*)pPixel)[u0];	
		}
		else
			return 0;
	}
}

