/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

//General entity File
//Gère tout les types d'éntités que l'on va pouvoir trouver dans le jeu

#ifndef _ENTITY_WATER_H
#define _ENTITY_WATER_H

#include "Entity.h"
#include "../LowLevel/Platform.h"
#include "../LevelState.h"
#include "../WorldManager.h"

#define MAX_DEF            16
#define WATERTYPE_WATER    0

//Paramètres du plan d'eau
// TYPE :
// p1 et p2 : w h
// Autres : definition teinte teinte2 mouvement_h_max speed dangerous

//Classes
class eWater : public CEntity
{
public:
	//Destructeur
	eWater();
    ~eWater();
	//Fonctions générales
    void Draw(int layer);
    void Update();
    void Destroy();
    void Load();
    void CheckInteractions();
	//Fonctions d'action entités
	void Move(int move_type, int p1, int p2);

public:
	
	int  def, tint, tint2, movh, speed, dangerous;   //Definition teinte mouvement_h_max dangerous
    int  largeur;									 //Largeur de l'eau
    int  pointsy[MAX_DEF];							 //Coord. y relatives de l'eau
    bool psens[MAX_DEF];							 //Sens de déplacement de la surface
    bool pboot[MAX_DEF];							 //Phase de démarrage de l'eau
    int  bootNumber;								 //Numéro du point à démarrer
	int  waterType;

	//Tests
	int xtime[MAX_DEF];
};

#endif