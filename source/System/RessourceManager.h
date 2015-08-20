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
        MD2Entity* LoadModel(string filename, int widthheight, float scale, bool mapMode = false);  //Fonction de chargement de mod�le
		MD2Entity* LoadModelBin(string filename, int widthheight, float scale, bool mapMode = false);  //Fonction de chargement de mod�le
        void DeleteImage(LowLevel::Image* img);       //Suppression d'images
		void DeleteImage2D(sImage* img);              //Suppression d'images
        void DeleteModel(MD2Entity* model);           //Suppression de mod�le
        void Free();                                  //On vide tout le manager
        unsigned int GetImageCount();                 //Retourne le nombre d'images charg�es
        unsigned int GetModelCount();                 //Retourne le nombre de mod�les charg�s
		unsigned int GetImage2DCount();                 //Retourne le nombre d'images charg�es

        //Variables
    private:
        ImageList imgList;                            //std::map d'images
        ModelList modList;                            //std::map de mod�les
		Image2DList img2dList;						  //std::map d'images 2d
        unsigned int img_num;                         //Nombre d'images en m�moire
		unsigned int img2d_num;
        unsigned int mod_num;

		int palColNumber;
    };
}
#endif
