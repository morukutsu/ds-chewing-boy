/* --------------------------- */
/* ITM2                        */
/* --------------------------- */

#include "HitBoxes.h"

void MirrorXHitTestPoints(int w, bool s, sHitTestPoints* pts)
{
	if(s != pts->mMirrored)
	{
		pts->mMirrored = s;
		pts->pTopSideXr = w-pts->pTopSideX;
		pts->pBottomSideXr = w-pts->pBottomSideX;
		pts->pFallX2 = w-pts->pFallX1;
		pts->pFallY2 = pts->pFallY1;
		pts->pTopX2 = w-pts->pTopX1;
		pts->pTopY2 = pts->pTopY1;
	}
}

void MirrorXBoxRect(int w, bool s, sHitBoxRect* rect)
{
	if(s != rect->mMirrored)
	{
		rect->mMirrored = s;
		int temp = rect->x2;
		rect->x2 = w-rect->x2;
		rect->x1 = w-rect->x1;
	}
}