/* --------------------------- */
/* ITM2                        */
/* --------------------------- */

#ifndef H_HITBOXES
#define H_HITBOXES

#include <ulib/ulib.h>

struct sHitTestPoints
{
	bool mMirrored;
	int pTopX1, pTopY1;                 //Point de la hitbox au niveau de la tête (collision plafond)
	int pTopX2, pTopY2;
	int pBottomX, pBottomY;             //Point de collision au niveau des pieds
	int pFallX1, pFallY1;
	int pFallX2, pFallY2;
	int pTopSideX, pTopSideY;           //Point de collision murs en haut à droite
	int pBottomSideX, pBottomSideY;     //Point de collision murs en bas à droite
	int pTopSideXr, pBottomSideXr;      //A gauche
};

struct sHitBoxRect
{
	int x1, y1, x2, y2;                 //Points de la hitbox rectangulaire
	bool mMirrored;
};

void MirrorXHitTestPoints(int w, bool s, sHitTestPoints* pts);
void MirrorXBoxRect(int w, bool s, sHitBoxRect* rect);
#endif