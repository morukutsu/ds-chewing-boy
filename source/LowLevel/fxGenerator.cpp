/* --------------------------- */
/* Inside the machine 2        */
/* --------------------------- */

#include "fxGenerator.h"
#include "Platform.h"
#include "../WorldManager.h"

//Presets
namespace LowLevel
{
	fxGenerator::fxGenerator()
	{
		free = true;
		stopGen = false;
	}

	void fxGenerator::Set(int v_x, int v_y, int tex, int mod, int num, int couleur, int couleur2, int wh, int f, int range_x, int range_y, int boot_freq, int imp_freq, int end_method, int gravity)
	{
		active = true;
		curImpFreq = curBootFreq = 0;
		isNewPartAccepted = true;
		toDelete = false;
		stopGen = false;

		polygroupswaping = 1;
		texture = tex;
		mode = mod;
		number = num;
		color = couleur;
		color2 = couleur2;
		fade = f;
		taille = wh;
		rangex = range_x;
		rangey = range_y;
		bootFreq = boot_freq;
		impFreq = imp_freq;
		endMethod = end_method;
		g = gravity;
		vx = v_x;
		vy = v_y;
	}

	void fxGenerator::Set(sGenPreset preset)
	{
		Set(preset.vx, preset.vy, preset.texture, preset.mode, preset.number, preset.couleur, 
			preset.couleur2, preset.taille, preset.fade, preset.range_x, preset.range_y, preset.boot_freq, 
			preset.imp_freq, preset.end_method, preset.gravity);
	}

	void fxGenerator::Load()
	{
		//On initialise les particules
		for (int k = 0; k < number; k++)
			particules[k].actif = false;

		//On charge la bonne texture en fonction du type
		switch (texture)
		{
			case PART_TEX_BALL:
				img.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Ball.png", UL_IN_VRAM, UL_PF_PAL4));
				break;
			case PART_TEX_SMOKE:
				img.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Smoke.png", UL_IN_VRAM, UL_PF_PAL4));
				break;
			case PART_TEX_BULLES:
				img.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Bulles.png", UL_IN_VRAM, UL_PF_PAL4));
				break;
			case PART_TEX_ROCK:
				img.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Rock.png", UL_IN_VRAM, UL_PF_PAL4));
				break;
			case PART_TEX_EPINES:
				img.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Epines.png", UL_IN_VRAM, UL_PF_PAL2));
				break;
			case PART_TEX_EPINES2:
				img.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Epines2.png", UL_IN_VRAM, UL_PF_PAL2));
				break;
			case PART_TEX_FLARE:
				img.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/spr_flare_1.png", UL_IN_VRAM, UL_PF_PAL3_A5));
				break;
		}

		//On ajoute d�j� toutes les particules
		if(mode == GENERATOR_SHORT)
		{
			for (int k = 0; k < number; k++)
			{
				//Si non, on v�rifie si il faut reg�n�rer la particule
				particules[k].life = 1024;
				particules[k].actif = true;
				particules[k].x = itofx(x); //Passage de la variable en fixpt (multiplication)
				particules[k].y = itofx(y);
				//On teste si la prochaine particule sera une impuret�
				if (curImpFreq == 0)
					particules[k].color = color2;
				else
					particules[k].color = color;
				//curImpFreq = LowLevel::Rand(0, 1);
				//On calcule le vecteur de direction
				particules[k].vx = vx + LowLevel::Rand(-rangex, rangex);
				particules[k].vy = vy + LowLevel::Rand(-rangey, rangey);
				//On calcule la vitesse de disparition
				particules[k].fade = fade + LowLevel::Rand(-8, 8);
				//On update le polygroup
				particules[k].polyg = polygroupswaping;
				polygroupswaping++;
				if(polygroupswaping >= 10)
					polygroupswaping = 2;
				curImpFreq++;
				if (curImpFreq > impFreq)
					curImpFreq = 0;
			}
			total_number = number;
		}
	}

	void fxGenerator::Draw()
	{
		//On affiche toute les particules
		if(active)
		{
			//On parcourt toutes les particules
			for (int k = 0; k < number; k++)
			{
				if (particules[k].actif)
				{
					//On cr�e les positions �cran
					int scrx = particules[k].x + worldMan->scrollX;
					int scry = particules[k].y + worldMan->scrollY;
					//ulDebug("scx:%d scy:%d\n", UNFIX(scrx), UNFIX(scry));
					//Couleur
					img.Tint(particules[k].color);
					//On d�finit les param�tres selon la m�thode de fin
					if (endMethod == END_METHOD_FADE)
					{
						//On positionne la particule
						img.MoveTo(fxtoi(scrx), fxtoi(scry));
						//On redimensionne
						img.Scale(taille, taille);
						//On calcule/applique l'alpha
						int alpha = particules[k].life>>5;
						if (alpha < 1)
							alpha = 1;
						if (alpha > 31)
							alpha = 31;
						img.Alpha(alpha, particules[k].polyg);
						//On affiche
						img.Draw();
					}
					if (endMethod == END_METHOD_SCALE)
					{
						//On r�duit le sprite
						taille = particules[k].life>>5;
						//Taille
						img.Scale(taille, taille);
						//On positionne la particule
						img.MoveTo(fxtoi(scrx)-(taille>>1), fxtoi(scry)-(taille>>1));
						img.Draw();
					}
					if (endMethod == END_METHOD_SCALEFADE)
					{
						//On r�duit le sprite
						taille = particules[k].life>>5;
						//Taille
						img.Scale(taille, taille);
						//On positionne la particule
						//ulDebug("px:%d - py:%d\n", fxtoi(scrx)-(taille>>1), fxtoi(scry)-(taille>>1));
						img.MoveTo(fxtoi(scrx)-(taille>>1), fxtoi(scry)-(taille>>1));
						//On calcule/apllique l'alpha
						int alpha = particules[k].life>>5;
						if (alpha < 1)
							alpha = 1;
						if (alpha > 31)
							alpha = 31;
						img.Alpha(alpha, particules[k].polyg);
						//On affiche
						img.Draw();
					}
				}
			}
		}
	}

	void fxGenerator::Update()
	{
		//Calcul de la position �cran de l'entit�
		x_screen = x + worldMan->scrollX;
		y_screen = y + worldMan->scrollY;

		//On v�rifie si le g�n�rateur est � l'�cran
		/*if (x_screen < (0-taille) || x_screen > (256+taille) || y_screen < 0 || y_screen > 256)
			active = false;
		else
			active = true;*/

		//On met � jour le g�n�rateur
		if (active)
		{
			isNewPartAccepted = true;

			//On parcourt toutes les particules
			for (int k = 0; k < number; k++)
			{
				//On v�rifie si la particule concern�e est active
				if (particules[k].actif)
				{
					//Si oui, on met a jour la position de la particule
					particules[k].x += particules[k].vx;
					particules[k].y += particules[k].vy;
					//On fait baisser la vie de la particule
					particules[k].life -= particules[k].fade;
					//On ajoute une gravit� sommaire
					particules[k].vy += g;
					//On v�rifie si il faut d�sactiver la particule
					if (particules[k].life <= 0)
					{
						particules[k].actif = false;
						/*if(mode == GENERATOR_SHORT)
						{*/
							total_number--;
						/*}*/
					}
				}
				else
				{
					if (!stopGen && curBootFreq == 0 && isNewPartAccepted && mode == GENERATOR_CONSTANT)
					{
						isNewPartAccepted = false;
						//Si non, on v�rifie si il faut reg�n�rer la particule
						particules[k].life = 1024;
						particules[k].actif = true;
						particules[k].x = itofx(x); //Passage de la variable en fixpt (multiplication)
						particules[k].y = itofx(y);
						//On teste si la prochaine particule sera une impuret�
						if (curImpFreq == 0)
							particules[k].color = color2;
						else
							particules[k].color = color;
						//On calcule le vecteur de direction
						particules[k].vx = vx + LowLevel::Rand(-rangex, rangex);
						particules[k].vy = vy + LowLevel::Rand(-rangey, rangey);
						//On calcule la vitesse de disparition
						particules[k].fade = fade + LowLevel::Rand(-8, 8);
						//On update le polygroup
						total_number++;
						particules[k].polyg = polygroupswaping;
						polygroupswaping++;
						if(polygroupswaping >= 10)
							polygroupswaping = 2;
					}
				}
			}

			//On incr�mente les frequences de g�n�ration d'une particule
			curImpFreq++;
			curBootFreq++;

			//On v�rifie leurs limites
			if (curImpFreq > impFreq)
				curImpFreq = 0;
			if (curBootFreq > bootFreq)
				curBootFreq = 0;
		}
		//Test de suppression
		if(/*mode == GENERATOR_SHORT &&*/ total_number == 0)
			toDelete = true;
	}

	void fxGenerator::Reset()
	{

	}
};