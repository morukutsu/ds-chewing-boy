/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

#include "eCamera.h"
#include "../LevelState.h"
#include "../WorldManager.h"

eCamera::eCamera()
{
	//Définition du type et du genre
	type = ENT_TYPE_LOGIC;
	genre = ENT_LOGIC_CAMERA;
	
	//Paramatrage physique et hitbox
	density = 0;
	vx = vy = 0;
	real = false;
	toDelete = false;
	w = 32;
	h = 32;
	x = y = 0;
	
	//Renseignement du worldManager
	worldMan = LevelState::Instance()->worldMan;
	layer = 0;
}

eCamera::~eCamera()
{

}

void eCamera::Load()
{
	
}

void eCamera::Draw(int lay)
{

}

void eCamera::Update()
{
	
}

void eCamera::Destroy()
{

}

void eCamera::CheckInteractions()
{

}

void eCamera::Move(int move_type, int p1, int p2)
{

}