/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

#include "eWater.h"

eWater::eWater()
{
	//Définition du type et du genre
	type = ENT_TYPE_OBJET;
	genre = ENT_OBJET_WATER;
	
	//Paramatrage physique et hitbox
	density = 0;
	vx = vy = 0;
	real = false;
	toDelete = false;
	actif = true;
	ignoreCollision = true;

	//Renseignement du worldManager
	worldMan = LevelState::Instance()->worldMan;
	layer = 0;
}

eWater::~eWater()
{
	
}

void eWater::Load()
{
	//Calcul de la largeur une partie
	def = 16;
    largeur = w/def;
    bootNumber = 0;
    actif = false;
	int xinc = 0;
    //On génère la géométrie verticale de l'eau
    for(int k = 0; k < def; k++)
    {
        //On régle tout les y a 0
        pointsy[k] = 0;
		xtime[k] = xinc;
		xinc += 2000;
        psens[k] = true;
        //pboot[k] = false;
		pboot[k] = true;
    }
    movh = itofx(movh);

    //Renseignement de l'hitbox
    mHitRect.x1 = 0, mHitRect.y1 = 0;
    mHitRect.x2 = w, mHitRect.y2 = h;
	mHitRect.mMirrored = false;
	
	dangerous = 0;
	tint = 20896;
	tint2 = 11457;

	//On ajoute le body
	bodyID = worldMan->AddBody(0, 0, this, BODY_WATER);
	worldMan->mBodies[bodyID].aabb = &mHitRect;
	worldMan->mBodies[bodyID].isSensor = true;
	worldMan->mBodies[bodyID].isSleeping = false;
}

void eWater::Draw(int lay)
{
	//On affiche la géométrie
    if(actif && lay == layer)
    {
        //On dessiner un quad nom-texture
        ulDisableTexture();

        //On affiche la seconde couche d'eau
        ulSetAlpha(UL_FX_ALPHA, 18, 4);

		//Eau
        ulVertexBegin(GL_QUAD_STRIP);
        
		for (int k = 0; k < def-1; k++)
        {
			//Eau
            ulVertexColor(tint);
            ulVertexXY(x_screen + largeur*k, y_screen + fxtoi(pointsy[k]));
            ulVertexColor(tint2);
            ulVertexXY(x_screen + largeur*k, y_screen + h);
            ulVertexColor(tint);
            ulVertexXY(x_screen + largeur*(k+1), y_screen + fxtoi(pointsy[k+1]));
            ulVertexColor(tint2);
            ulVertexXY(x_screen + largeur*(k+1), y_screen + h);
        }
		//Eau
        ulVertexColor(tint);
        ulVertexXY(x_screen + largeur*(def-1), y_screen + fxtoi(pointsy[def-1]));
        ulVertexColor(tint2);
        ulVertexXY(x_screen + largeur*def-1, y_screen + h);
        ulVertexColor(tint);
        ulVertexXY(x_screen + largeur*(def), y_screen + fxtoi(pointsy[0]));
        ulVertexColor(tint2);
        ulVertexXY(x_screen + largeur*(def), y_screen + h);

        //Retour à la profondeur d'origine
        ulVertexEnd();
        ulVertexHandleDepth();
		
		//Surface de l'eau
		ulSetAlpha(UL_FX_ALPHA, 25, 5);
		ulVertexBegin(GL_QUAD_STRIP);
        for (int k = 0; k < def-1; k++)
        {
			//Surface blanche de l'eau
			ulVertexColor(32767);
            ulVertexXY(x_screen + largeur*k, y_screen + fxtoi(pointsy[k]));
			ulVertexColor(tint);
            ulVertexXY(x_screen + largeur*k, y_screen + 5 + fxtoi(pointsy[k]));
			ulVertexColor(32767);
            ulVertexXY(x_screen + largeur*(k+1), y_screen + fxtoi(pointsy[k+1]));
			ulVertexColor(tint);
            ulVertexXY(x_screen + largeur*(k+1), y_screen + 5 + fxtoi(pointsy[k+1]));
        }
		//Surface blanche de l'eau
		ulVertexColor(32767);
        ulVertexXY(x_screen + largeur*(def-1), y_screen + fxtoi(pointsy[def-1]));
		ulVertexColor(tint);
        ulVertexXY(x_screen + largeur*(def-1), y_screen + 5 + fxtoi(pointsy[def-1]));
		ulVertexColor(32767);
        ulVertexXY(x_screen + largeur*(def), y_screen + fxtoi(pointsy[0]));
		ulVertexColor(tint);
        ulVertexXY(x_screen + largeur*(def), y_screen + 5 + fxtoi(pointsy[0]));
        ulVertexEnd();
        ulVertexHandleDepth();

        ulSetAlpha(UL_FX_DEFAULT, 0, 4);

    }
	
}

void eWater::Update()
{
	actif = false;
	if(x_screen > (0-w-largeur) && x_screen < (256+w+largeur) && y_screen > (0-h) && y_screen < (256+h))
	{
		actif = true;
		//worldMan->mOnScreenWater++;
		//On fait bouger la surface
        for (int k = 0; k < def; k++)
        {
            //On démarre
            /*if (bootNumber == k)
                pboot[k] = true;*/

            //On vérifie si l'eau est démarrée
            /*if (pboot[k])
            {*/
                //On fait bouger l'eau
                /*if (psens[k])
				{
                    pointsy[k] += speed;
				}
                else
				{
                    pointsy[k] -= speed;
				}
                //On change le sens
                if (pointsy[k] >= movh)
                    psens[k] = false;
                if (pointsy[k] <= -movh)
                    psens[k] = true;*/
				pointsy[k] = sinLerp(xtime[k])>>2;
				//ulDebug("l:%d x:%d\n", k, xtime[k] );
				xtime[k] += 500;
            /*}*/
        }

        if (bootNumber < def)
            bootNumber++;
		/*if(worldMan->mBodies[bodyID].collide)
		{
			for(int k = 0; k < worldMan->mBodies[bodyID].bodyContactCount; k++)
			{
				if(worldMan->mBodies[bodyID].with[k]->userdata == BODY_HERO)
				{
					worldMan->mBodies[bodyID].with[k]->who->isInWater = true;
					//LowLevel::ParticleGenerator::getInstance()->AddGenerator(PARTICLES_SHORT_EAU, fxtoi(x)+8, fxtoi(y)+8);
				}
			}
		}
		else
		{
			worldMan->mHero->isInWater = false;
		}*/
	}

	//Calcul des coordonnées écran
	x_screen = fxtoi(x + worldMan->scrollX);
	y_screen = fxtoi(y + worldMan->scrollY);
}

void eWater::Destroy()
{

}

void eWater::CheckInteractions()
{

}

void eWater::Move(int move_type, int p1, int p2)
{

}