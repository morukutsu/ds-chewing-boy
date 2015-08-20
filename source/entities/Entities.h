/* --------------------------- */
/* Star plot                   */
/* --------------------------- */

//Includes
#ifndef _ENTITIES_H
#define _ENTITIES_H

//Entités
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
//Types d'entités
#define ENT_TYPE_HERO            0
#define ENT_TYPE_LOGIC           1
#define ENT_TYPE_OBJET           2
#define ENT_TYPE_ENNEMY          3

//Entités HERO 0
#define ENT_HERO_HERO           0    //Entité héros

//Entités LOGIC 1
#define ENT_LOGIC_STARTPOS      0    //Position de départ
#define ENT_LOGIC_ENDPOS        1    //Position de fin
#define ENT_LOGIC_CAMERA		2    //Camera (vue)

//Entités OBJET 2
#define ENT_OBJET_GEMME			0	//Gemme
#define ENT_OBJET_WATER         1
#define ENT_OBJET_FULLSTAR      2
#define ENT_OBJET_DBLOC         3
#define ENT_OBJET_EXTRALIFE     4

//Entités ENNEMY 3
#define ENT_ENNEMY_ENNEMY           0    //Entité ennemie
#define ENT_ENNEMY_WOMIE            1
#define ENT_ENNEMY_PEAKS            2
#define ENT_ENNEMY_WOPIC            3

#endif
