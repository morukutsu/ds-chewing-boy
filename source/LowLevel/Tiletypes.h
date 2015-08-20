/* ITM2                        */
/* --------------------------- */

//Includes
#ifndef _TILETYPES_H
#define _TILETYPES_H

//Types implicites (donn�s par la couleur sur le tile de collision)
#define TILE_IMP_NOTHING           0    //Tile vide
#define TILE_IMP_NOPASS            1    //Bloc solide
#define TILE_IMP_SLOPE             2    //Pente
#define TILE_IMP_PASSUP            3    //Ne r�agit pas � la collision avec la t�te
#define TILE_IMP_SLOPE_PASSUP      4    //Pente + ci dessus

//Enum�ration des types de tiles possibles
#define TILE_TYPE_NOTHING          0    //Type de tile avec rien dessus
#define TILE_TYPE_WATER            1    //Transforme le tile en eau
#define TILE_TYPE_DISCO            2    //S'illumine quand on marche dessus (et les cases aux alentours)
#define TILE_TYPE_DISAPPEAR        3    //Disparait (puis r�apparait) quand on marche dessus
#define TILE_TYPE_AUTODISAPPEAR    4    //Disparait (puis r�apparait) avec un d�lai (d�calage 0)
#define TILE_TYPE_DANGEROUS        5    //Inflige des d�g�ts
#define TILE_TYPE_KILL             6    //Tue le h�ros directement
#define TILE_TYPE_ICE              7    //Surface glissante
#define TILE_TYPE_DANGEROUS_HIT    8    //Inflige des d�gats et expulse

#endif