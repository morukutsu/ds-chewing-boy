/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

//General entity File
//G�re tout les types d'�ntit�s que l'on va pouvoir trouver dans le jeu

#ifndef _ENTITY_CAMERA_H
#define _ENTITY_CAMERA_H

#include "Entity.h"
#include "../LowLevel/Platform.h"

//Classes
class eCamera : public CEntity
{
public:
	//Destructeur
	eCamera();
    ~eCamera();
	//Fonctions g�n�rales
    void Draw(int layer);
    void Update();
    void Destroy();
    void Load();
    void CheckInteractions();
	//Fonctions d'action entit�s
	void Move(int move_type, int p1, int p2);

public:
};

#endif