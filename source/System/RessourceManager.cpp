/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#include "RessourceManager.h"

char sprbuffer[16*1024];

namespace System
{
    CRessourceManager::CRessourceManager()
    {
        //MAJ de variables
        img_num = 0;
        mod_num = 0;
		palColNumber = 0;
		img2d_num = 0;
        imgList.clear();
        modList.clear();
		img2dList.clear();
    }

    CRessourceManager::~CRessourceManager()
    {
        //Nettoyage
        imgList.clear();
        modList.clear();
		img2dList.clear();
    }

    LowLevel::Image* CRessourceManager::LoadImage(string filename, int location, int pixelFormat)
    {
        //Verification de l'existance possible de l'image
        //Iterateur
        for (ImageList::iterator i = imgList.begin(); i!=imgList.end(); ++i)
        {
            if (i->first == filename)
            {
                //ulDebug("[Notif] Image deja chargée (%s)", filename.c_str() );
                return i->second;
            }
        }

        //Chargement depuis le fichier
        LowLevel::Image* img;
        img = new LowLevel::Image;
        img->PNGLoadFromFS((char*)filename.c_str(), location, pixelFormat);
        img->SetManagerLoad();

        imgList[filename] = img;
        img_num++;

        //Envoi de l'image
        return imgList[filename];
    }

	sImage* CRessourceManager::LoadImage2D(string filename)
	{
		//Verification de l'existance possible de l'image
        //Iterateur
        for (Image2DList::iterator i = img2dList.begin(); i!=img2dList.end(); ++i)
        {
            if (i->first == filename)
            {
                //ulDebug("[Notif] Image deja chargée (%s)", filename.c_str() );
                return i->second;
            }
        }

		sImage* tmp;
		//On définit quelques variables
        FILE* file;
        char* buffer;
        int size;
        //On ouvre le fichier
        file = fopen(filename.c_str(), "rb");

        if (file != NULL)
        {
            //Cn charge le fichier en mémoire
            fseek(file, 0, SEEK_END);
            size = ftell(file);
            fseek(file, 0, SEEK_SET);
            int result = fread(sprbuffer, 1, size, file);

            if (result != size)
            {
                ulDebug("read break!");
            }
            fclose (file);

            //On charge le png
            //ulSetTransparentColor(colorKey);
            //ulSetLoadUtilitySource(VF_MEMORY);
            //SetImage(ulLoadImageFilePNG((const char*)sprbuffer, size, location, pixelFormat));
            //ulDisableTransparentColor();
            //free(buffer);
			loadPCX((u8*)sprbuffer, tmp);
			//tile it so it is usefull as sprite data
			imageTileData(tmp);
			
			//Gestion des palettes (très lent...)
			/*for(int k = 0; k < 256; k++)
			{
				//Parmi les 256 couleurs du pcx
				bool found = false;
				for(int l = 0; l < palColNumber; l++)
				{
					//Si on trouve une couleur identique à celle ci dans la liste
					//On ne modifie pas la palette mais on modifie l'image pour que la couleur coincide
					if(SPRITE_PALETTE_SUB[l] == tmp->palette[k])
					{
						for(int i = 0; i < tmp->width*tmp->height; i++)
						{
							if(tmp->image.data8[i] == k)
							{
								NOGBA("found %d to %d\n", k, l);
								tmp->image.data8[i] = l;
							}
						}
						found = true;
						break;
					}
				}
				
				//On a pas trouvé cette couleur dans notre palette
				if(!found)
				{
					SPRITE_PALETTE_SUB[palColNumber] = tmp->palette[k];
					palColNumber++;
				}
			}*/
			u32 buf, color;
			u32 i, ipx, palSize=2; 
			for(int i = 0; i < tmp->width*tmp->height; i++)
			{
				ipx= tmp->image.data8[i];
				if(ipx > 0)
				{
					color = tmp->palette[ipx];      // Get pixel color.
			            
					//Search for color.
					for(ipx=1; ipx<palSize; ipx++)
						if(color == SPRITE_PALETTE_SUB[ipx])
						   break;
			               
					 // Not found: add new color.
					 if(ipx==palSize)
					 {
						SPRITE_PALETTE_SUB[ipx]= color;
						palSize++;
						//# PONDER: what to do if palSize > 256 ?!?
					 }
				  }
			      
				// NOTE: VRAM needs 2-pixel access, so even i store
				// to buffer and odd i actually write to VRAM.
				tmp->image.data8[i] = ipx; 
			}
		}
					
		img2dList[filename] = tmp;
		img2d_num++;

		//Envoi de l'image
        return tmp;
	}

    MD2Entity* CRessourceManager::LoadModel(string filename, int widthheight, float scale, bool mapMode)
    {
        //Verification de l'existance possible de l'image
        //Iterateur
        for (ModelList::iterator i = modList.begin(); i!=modList.end(); ++i)
        {
            if (i->first == filename)
            {
                //ulDebug("[Notif] Modèle deja chargé (%s)", filename.c_str() );
                return i->second;
            }
        }

        //Chargement depuis le fichier
        MD2Entity* model;
        model = new MD2Entity;

        //On charge un modèle depuis le fichier
        bool texturereverse = false;
        FILE *fp = fopen((char*)filename.c_str(),"rb");
        if (fp == NULL)
        {
            ulDebug("wasnt found");
            return NULL;
        }

        int i;

        /* read header */
        fread(&model->header, 1, sizeof (md2_header_t), fp);

        if ((model->header.ident != 844121161) || (model->header.version != 8))
        {
            /* error! */
            iprintf("error: bad version!");
            while (1==1)
            {
                //Can't go on so lets hang arround
            }
        }

        /* memory allocation */
        model->texcoords = (md2_texCoord_t *)malloc (sizeof (md2_texCoord_t) * model->header.num_st);
        model->triangles = (md2_triangle_t *)malloc (sizeof (md2_triangle_t) * model->header.num_tris);
        model->frames = (md2_frame_t *)malloc (sizeof(md2_frame_t) * model->header.num_frames);

        //Read float textcoords
        fseek (fp, model->header.offset_st, SEEK_SET);
        fread (model->texcoords, sizeof (md2_texCoord_t), model->header.num_st, fp);

        //read that float triangles
        fseek (fp, model->header.offset_tris, SEEK_SET);
        fread (model->triangles, sizeof (md2_triangle_t), model->header.num_tris, fp);

        /* read frames */
        fseek (fp, model->header.offset_frames, SEEK_SET);

        for (i = 0; i < model->header.num_frames; ++i)
        {
            /* memory allocation for vertices of this frame */
            model->frames[i].verts = (md2_vertex_t *)
                                     malloc (sizeof (md2_vertex_t) * model->header.num_vertices);

            /* read frame data */
            fread (model->frames[i].scale, sizeof (vec3_t), 1, fp);
            fread (model->frames[i].translate, sizeof (vec3_t), 1, fp);
            fread (model->frames[i].name, sizeof (char), 16, fp);
            fread (model->frames[i].verts, sizeof (md2_vertex_t), model->header.num_vertices, fp);
        }

        fclose (fp);

        //<<<<<<<<Extatcly here we got a standard md2 model stored in a standard way
        //Now we convert them for a superfast nds renderway
        //ok erstma den shit mallocen
        model->uvs = (nds_texCoord_t *)malloc (sizeof (nds_texCoord_t) * model->header.num_st);
        model->tris = (nds_triangle_t *)malloc (sizeof (nds_triangle_t) * model->header.num_tris);
        model->frms = (nds_frame_t *)malloc (sizeof(nds_frame_t) * model->header.num_frames);
		
        for (i = 0; i < model->header.num_frames; ++i)
            model->frms[i].verts = (nds_vertex_t *) malloc (sizeof (nds_vertex_t) * model->header.num_vertices);

        //nun schön brav eins nach dem anderen von float und den dreck in fixed point math konvertieren
        int I,j,N;
        float s,t;
        vec3_t v;
        md2_frame_t *pframe;
        md2_vertex_t *pvert;//der kram is nötig dafür

        //also framedata in rahmendaten umwandeln
        for (N = 0; N < model->header.num_frames; ++N)
        {
            for (I = 0; I < model->header.num_tris; ++I)
            {
                for (j = 0; j < 3; ++j)
                {
                    pframe = &model->frames[N];
                    pvert = &pframe->verts[model->triangles[I].vertex[j]];
                    //dekomresse die aktuelle framedate
                    v[0] = (pframe->scale[0] * pvert->v[0]) + pframe->translate[0];
                    v[1] = (pframe->scale[1] * pvert->v[1]) + pframe->translate[1];
                    v[2] = (pframe->scale[2] * pvert->v[2]) + pframe->translate[2];
                    //Lager die unkompressed framevertexe
                    model->frms[N].verts[model->triangles[I].vertex[j]].v[0]=floattov16(float(v[0]/128)*scale);
                    model->frms[N].verts[model->triangles[I].vertex[j]].v[1]=floattov16(float(v[1]/128)*scale);
					model->frms[N].verts[model->triangles[I].vertex[j]].v[2]=floattov16(float(v[2]/128)*scale);

                    //model->frms[N].verts[model->triangles[I].vertex[j]].v[2]=floattov16(float(v[2]/128)*scale);
                    //Kopiere den framename
                    strcpy(model->frms[N].name,pframe->name);
                }
            }
        }

        //Normal indices
        for (N = 0; N < model->header.num_frames; ++N)
        {
            for (I = 0; I < model->header.num_vertices; ++I)
            {
                model->frms[N].verts[I].normalindex=model->frames[N].verts[I].normalIndex;
            }
        }

        //Nun zu den texturkooridinaten
        //texturereverse = true;
        for (I = 0; I < model->header.num_tris; ++I)
        {
            for (j = 0; j < 3; ++j)
            {
                if (texturereverse)
                {
                    s = (float)model->texcoords[model->triangles[I].st[j]].s / model->header.skinwidth;
                    t = (float)model->texcoords[model->triangles[I].st[j]].t / model->header.skinheight;
                    model->uvs[model->triangles[I].st[j]].s=floattot16(float(s)*widthheight);
                    model->uvs[model->triangles[I].st[j]].t=floattot16(float(1-t)*widthheight);
                }

                if (!texturereverse)
                {
                    s = (float)model->texcoords[model->triangles[I].st[j]].s / model->header.skinwidth;
                    t = (float)model->texcoords[model->triangles[I].st[j]].t / model->header.skinheight;
                    model->uvs[model->triangles[I].st[j]].s=floattot16(float(s)*widthheight);
                    model->uvs[model->triangles[I].st[j]].t=floattot16(float(t)*widthheight);
                }
                model->tris[I].st[j]=model->triangles[I].st[j];//Index for texture coords
                model->tris[I].vertex[j]=model->triangles[I].vertex[j];//Index for vertices
            }
        }


        for (N = 0; N < model->header.num_frames; ++N)
            free(model->frames[N].verts);
        free(model->frames);
        free(model->texcoords);
        free(model->triangles);

        /*ulDebug("vert:%d\n", model->header.num_vertices);
        ulDebug("tris:%d\n", model->header.num_tris);
        ulDebug("frames:%d\n", model->header.num_frames);
        ulDebug("framew:%d - skinw:%d", model->header.skinwidth, widthheight);*/

        //Déclaration d'un pointeur vers un vertex
        /*nds_vertex_t* vert = model->frms[0].verts;
		
		//Génération de la callist
		if(model->header.num_frames == 1)
		{
			//On génère la calllist pour ce modèle
			int commandCount = 3 + model->header.num_tris*3*4;
			ulDebug("commandCount:%d\n", commandCount);

			//On alloue de quoi stocker la calllist
			model->calllist = (u32*)malloc((commandCount+1)*sizeof(u32*) );

			//On remplit la callist
			model->calllist[0] = (u32)commandCount;
			model->calllist[1] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_NOP, FIFO_NOP, FIFO_NOP);
			model->calllist[2] = GL_TRIANGLES;
			int mcID = 3;

			for(int i = 0; i < model->header.num_tris; ++i)
			{
        		for(int j = 0; j < 3; ++j)
        		{
        			int stnum = model->tris[i].st[j];
        			int vertnum = model->tris[i].vertex[j];
					
					model->calllist[mcID] = FIFO_COMMAND_PACK(FIFO_NORMAL, FIFO_TEX_COORD, FIFO_VERTEX10, FIFO_NOP);
        			mcID++;
					model->calllist[mcID] = NORMAL_PACK(LowLevel::Model::anormtable16[vert[vertnum].normalindex][0], LowLevel::Model::anormtable16[vert[vertnum].normalindex][1], LowLevel::Model::anormtable16[vert[vertnum].normalindex][2]);
        			mcID++;
					model->calllist[mcID] = TEXTURE_PACK(model->uvs[stnum].s, model->uvs[stnum].t);
					mcID++;
					model->calllist[mcID] = DSPack10(vert[vertnum].v[0], vert[vertnum].v[1], vert[vertnum].v[2]);
					mcID++;
        		}
			}
			model->calllist[mcID] = FIFO_COMMAND_PACK(FIFO_END, FIFO_NOP, FIFO_NOP, FIFO_NOP);
			model->calllistGenerated = 1;

			//On libère l'espace...
		}
		else
			model->calllistGenerated = 0;*/

		//model->calllistGenerated = 0;

        modList[filename] = model;
        mod_num++;
		
		//ulDebug("Mem Free : %d\n", getMemFree() );
        //Envoi du modèle
		IdeasOutputDebugString("Mem Free (loading model): %d\n", getMemFree() );
        return modList[filename];
    }

	MD2Entity* CRessourceManager::LoadModelBin(string filename, int widthheight, float scale, bool mapMode)
	{
		/*//Verification de l'existance possible de l'image
        //Iterateur
        for (ModelList::iterator i = modList.begin(); i!=modList.end(); ++i)
        {
            if (i->first == filename)
            {
                //ulDebug("[Notif] Modèle deja chargé (%s)", filename.c_str() );
                return i->second;
            }
        }

        //Chargement depuis le fichier
        MD2Entity* model;
        model = new MD2Entity;

		//Lecture de la calllist
		FILE* file = fopen(filename.c_str(), "rb");
		
		//On lit la taille des commandes
		fread(&model->calllistSize, sizeof(u32), 1, file);
		//ulDebug("size:%d\n", model->calllistSize);
		IdeasOutputDebugString("cz: %d\n", model->calllistSize );
		rewind(file);

		//On alloue de la place pour la calllist
		model->calllist = (u32*)malloc((model->calllistSize+1)*sizeof(u32) );
		model->calllistGenerated = 1;

		fread(model->calllist, sizeof(u32), (model->calllistSize+1), file);

		fclose(file);
		
		//ulDebug("Mem Free : %d\n", getMemFree() );
		IdeasOutputDebugString("Mem Free (loading callist): %d\n", getMemFree() );

		modList[filename] = model;
        mod_num++;
		*/
		return modList[filename];
	}

    void CRessourceManager::DeleteImage(LowLevel::Image* img)
    {
        //Iterateur
        for (ImageList::iterator i = imgList.begin(); i!=imgList.end(); ++i)
        {
            if (i->second == img)
            {
                //ulDebug("Image supprimée (%s)\n", i->first.c_str());
				//On supprime l'image
				img->FreeImage();
				img_num--;
                imgList.erase(i->first);
				//delete img;
                break;
            }
        }

    }

    void CRessourceManager::DeleteModel(MD2Entity* model)
    {
        //Iterateur
        for (ModelList::iterator i = modList.begin(); i!=modList.end(); ++i)
        {
            if (i->second == model)
            {
                //ulDebug("Image supprimée (%s)\n", i->first.c_str());
				//On supprime le modèle
				/*if(model->calllistGenerated == 1)
				{
					free(model->calllist);
					IdeasOutputDebugString("Deleted Callist" );
				}
				else
				{*/
					int N;
					for (N = 0; N < model->header.num_frames; ++N)
						free(model->frms[N].verts);

					free(model->frms);
					free(model->uvs);
					free(model->tris);
					IdeasOutputDebugString("Deleted Model" );
				/*}*/

				mod_num--;
                modList.erase(i->first);
				delete model;

				IdeasOutputDebugString("Mem Free (deletion) : %d\n", getMemFree() );
                break;
            }
        }
    }

    void CRessourceManager::Free()
    {
        //On supprime les images
        img_num = 0;
        mod_num = 0;
		
        //Iterateur
        for (ImageList::iterator i = imgList.begin(); i != imgList.end();)
        {
            //ulDebug("Image supprimée (%s)\n", i->first.c_str());
            (i->second)->FreeImage();
            imgList.erase(i++);
        }
        imgList.clear();
        //Iterateur
        for (ModelList::iterator i = modList.begin(); i != modList.end();)
        {
            //ulDebug("Image supprimée (%s)\n", i->first.c_str());
            int N;
            for (N = 0; N < (i->second)->header.num_frames; ++N)
                free((i->second)->frms[N].verts);

            free((i->second)->frms);
            free((i->second)->uvs);
            free((i->second)->tris);

            modList.erase(i++);
        }
        modList.clear();
    }

    unsigned int CRessourceManager::GetImageCount()
    {
        return img_num;
    }

    unsigned int CRessourceManager::GetModelCount()
    {
        return mod_num;
    }

	 unsigned int CRessourceManager::GetImage2DCount()
    {
        return img2d_num;
    }
};
