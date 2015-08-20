/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_RESSOURCEMANAGER
#define H_RESSOURCEMANAGER

//Includes generaux
#include "../Platform.h"
#include "../LowLevel/Image.h"
#include "../LowLevel/MD2.h"

#include <nds.h>
#include <map>
#include <string>
#include "singleton.h"

extern char sprbuffer[16*1024];

namespace System
{
    class LowLevel::Image;
    using namespace std;
    typedef map<string, LowLevel::Image*> ImageList;
    typedef map<string, MD2Entity*> ModelList;
	typedef map<string, sImage*> Image2DList;

    class CRessourceManager : public Singleton<CRessourceManager>
    {
        friend class Singleton<CRessourceManager>;
        //Fonctions
    public:
        CRessourceManager();                          //Constructeur
        ~CRessourceManager();                         //Destructeur

        LowLevel::Image* LoadImage(string filename, int location, int pixelFormat);         //Fonction de chargement d'images
		sImage* LoadImage2D(string filename);		//Fonction de chargement d'images
        MD2Entity* LoadModel(string filename, int widthheight, float scale, bool mapMode = false);  //Fonction de chargement de modèle
		MD2Entity* LoadModelBin(string filename, int widthheight, float scale, bool mapMode = false);  //Fonction de chargement de modèle
        void DeleteImage(LowLevel::Image* img);       //Suppression d'images
		void DeleteImage2D(sImage* img);              //Suppression d'images
        void DeleteModel(MD2Entity* model);           //Suppression de modèle
        void Free();                                  //On vide tout le manager
        unsigned int GetImageCount();                 //Retourne le nombre d'images chargées
        unsigned int GetModelCount();                 //Retourne le nombre de modèles chargés
		unsigned int GetImage2DCount();                 //Retourne le nombre d'images chargées

        //Variables
    private:
        ImageList imgList;                            //std::map d'images
        ModelList modList;                            //std::map de modèles
		Image2DList img2dList;						  //std::map d'images 2d
        unsigned int img_num;                         //Nombre d'images en mémoire
		unsigned int img2d_num;
        unsigned int mod_num;

		int palColNumber;
    };
}
#endif
