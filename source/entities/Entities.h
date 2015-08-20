/* --------------------------- */
/* Star plot                   */
/* --------------------------- */

//Includes
#ifndef _ENTITIES_H
#define _ENTITIES_H

//Entit�s
#include "eCamera.h"
#include "eHero.h"
#include "eEnnemy.h"
#include "eGemme.h"
#include "eWater.h"
#include "eFullstar.h"
#include "eWomie.h"
#include "ePeaks.h"
#include "eWopic.h"
#include "eDBloc.h"
#include "eExtralife.h"

//Defines
//Types d'entit�s
#define ENT_TYPE_HERO            0
#define ENT_TYPE_LOGIC           1
#define ENT_TYPE_OBJET           2
#define ENT_TYPE_ENNEMY          3

//Entit�s HERO 0
#define ENT_HERO_HERO           0    //Entit� h�ros

//Entit�s LOGIC 1
#define ENT_LOGIC_STARTPOS      0    //Position de d�part
#define ENT_LOGIC_ENDPOS        1    //Position de fin
#define ENT_LOGIC_CAMERA		2    //Camera (vue)

//Entit�s OBJET 2
#define ENT_OBJET_GEMME			0	//Gemme
#define ENT_OBJET_WATER         1
#define ENT_OBJET_FULLSTAR      2
#define ENT_OBJET_DBLOC         3
#define ENT_OBJET_EXTRALIFE     4

//Entit�s ENNEMY 3
#define ENT_ENNEMY_ENNEMY           0    //Entit� ennemie
#define ENT_ENNEMY_WOMIE            1
#define ENT_ENNEMY_PEAKS            2
#define ENT_ENNEMY_WOPIC            3

#endif
