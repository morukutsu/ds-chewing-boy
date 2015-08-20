/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#include "Image.h"
#include "../System/RessourceManager.h";

char pngbuffer[16*1024];

namespace LowLevel
{
    Image::Image()
    {
        isLoadedWithRessourceManager = false;
        colorKey = RGB15(31, 0, 31); //Rose
    }

	Image::~Image()
	{
		IdeasOutputDebugString("Destructor" );
		//delete img;
	}

    void Image::SetImage(UL_IMAGE *image)
    {
        img = image;
        height = img->stretchY;
        width = img->stretchX;
    }

    void Image::SetManagerLoad()
    {
        isLoadedWithRessourceManager = true;
    }

    void Image::FreeImage()
    {
        /*if (isLoadedWithRessourceManager)
        {
            //On libère la texture à l'aide du ressource manager
            isLoadedWithRessourceManager = false;
            System::CRessourceManager::getInstance()->DeleteImage(this);
        }
        else
        {*/

        //On libère directemment l'image
        ulDeleteImage(img);
        //img = NULL;

        /*}*/
    }

    void Image::PNGLoadFromFS(char* filename, int location, int pixelFormat)
    {
        //On définit quelques variables
        FILE* file;
        char* buffer;
        int size;
		//memset(pngbuffer, 0, 16*1024);
        //On ouvre le fichier
        file = fopen(filename, "rb");

        if (file != NULL)
        {
            //Cn charge le fichier en mémoire
            fseek(file, 0, SEEK_END);
            size = ftell(file);
            fseek(file, 0, SEEK_SET);

            //On alloue de la mémoire
            //buffer = (char*)malloc(sizeof(char)*size);

            /*if (buffer == NULL)
            {
                ulDebug("buffer break!");
            }*/

            int result = fread(pngbuffer, 1, size, file);

            if (result != size)
            {
                ulDebug("read break!");
            }
            fclose (file);

            //On charge le png
            ulSetTransparentColor(colorKey);
            ulSetLoadUtilitySource(VF_MEMORY);
            SetImage(ulLoadImageFilePNG((const char*)pngbuffer, size, location, pixelFormat));
            ulDisableTransparentColor();
            //free(buffer);
        }
        else
            img = NULL;
    }
}
