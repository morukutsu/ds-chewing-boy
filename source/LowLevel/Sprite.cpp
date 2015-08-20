/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#include "Sprite.h"

namespace LowLevel
{
    Sprite::Sprite()
    {
        centerX = centerY = 0;
        posX = posY = 0;
        scaleX = scaleY = 0;
        angle = 0;
        mirrorX = mirrorY = false;
        alpha = 31;
        polygroup = 0;

        frameCounter = currentFrame = 0;
        animNbr = 0;
        frameW = frameH = 0;
        currentAnim = defaultAnim = 0;
        isStarted = isPaused = isLoop = false;
		col = RGB15(31, 31, 31);
    }

    Sprite::~Sprite()
    {
        //image->FreeImage();
    }

    void Sprite::Draw()
    {
        //On applique les différents paramètres à l'image
		if(currentAnim != -1)
		{
			ulSetImageTint(image->img, col);
			ulMirrorImageH(image->img, mirrorX);
			ulMirrorImageV(image->img, mirrorY);
			image->img->stretchX = scaleX;
			image->img->stretchY = scaleY;
			image->img->centerX = centerX;
			image->img->centerY = centerY;
			image->img->angle = angle;

			ulSetAlpha(UL_FX_ALPHA, alpha, polygroup);
			ulDrawImageXY(image->img, posX, posY);
			ulSetAlpha(UL_FX_DEFAULT, 0, 0);
		}
    }

    void Sprite::Play()
    {
        if (!isPaused && isStarted && currentAnim != -1)
        {
            //On update le framecounter
            frameCounter++;
            //On vérifie l'état du framecounter
            if (frameCounter > animations[currentAnim].fps)
            {
                frameCounter = 0;
                currentFrame++;
            }
            //On vérifie l'état de la frame courante (cas loop)
            if (isLoop && currentFrame > animations[currentAnim].lengh)
                currentFrame = 0;
            //On vérifie si l'animation est finie
            if (!isLoop && currentFrame > animations[currentAnim].lengh)
                this->Stop();

            //On clippe le sprite
            Clip(animations[currentAnim].startx * frameW + currentFrame * frameW,
                 animations[currentAnim].posy* frameH, frameW, frameH);
        }
    }

    void Sprite::Clip(int x, int y, int w, int h)
    {
        ulSetImageTile(image->img, x, y, x + w, y + h);
    }

    void Sprite::Tint(int color)
    {
        col = color;
    }

    void Sprite::SetFrameWH(int w, int h)
    {
        frameW = w;
        frameH = h;
        Scale(frameW, frameH);
    }

    int Sprite::AddAnimation(u8 py, u8 stx, u8 len, u8 fps)
    {
        //Déclaration d'une structure anim temporaire
        sSpriteAnim tmpAnim = {py, stx, len, fps};
        //Ajout au vector
        animations[animNbr+1] = tmpAnim;
        animNbr++;
        return animNbr;
    }

    void Sprite::PlayAnim(int mode, int anim)
    {
        //Reset de l'ancienne animation
		if(anim != -1)
		{
			switch (mode)
			{
			case ANIM_LOOP:
				//Reset de l'ancienne animation
				if (currentAnim != anim)
				{
					frameCounter = currentFrame = 0;
					//Démarrage d'une animation
					isStarted = true;
					isLoop = true;
					//Settings
					currentAnim = anim;
				}
				break;

			case ANIM_ONCE:
				if (currentAnim != anim)
				{
					frameCounter = currentFrame = 0;
					//Démarrage d'une animation
					isStarted = true;
					isLoop = false;
					//Settings
					currentAnim = anim;
				}
				break;
			}
		}
		else
			currentAnim = -1;
    }

    void Sprite::SetAnimationSpeed(u8 animation, int speed)
    {
        animations[animation].fps = speed;
    }

    void Sprite::Pause()
    {
        isPaused = true;
    }

    void Sprite::Restart()
    {
        isPaused = false;
    }

    void Sprite::Stop()
    {
        frameCounter = 0;
        currentFrame--;
        isStarted = false;
    }
}
