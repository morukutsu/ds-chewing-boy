/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#include "Model.h"
#include "../System/RessourceManager.h"

namespace LowLevel
{
    vec3_t anorms_table[162] =
    {
#include "anorms.h"
    };

    bool Model::isInited = false;
    //v16  Model::anormtable16[162][3];

    Model::Model()
    {
        //On initialise le modèle
        /*if (isInited == false)
        {
            //Chargement des anorms
            for (int a = 0; a < 162; a++)
            {
                anormtable16[a][0] = floattov10(anorms_table[a][0]);
                anormtable16[a][1] = floattov10(anorms_table[a][1]);
                anormtable16[a][2] = floattov10(anorms_table[a][2]);
            }
            isInited = true;
        }*/

        centerX = centerY = 0;
        posX = posY = 0;
        scaleX = scaleY = 0;
        angle = 0;
        mirrorX = mirrorY = false;
        mirrorXVal = mirrorYVal = 0;
        alpha = 31;
        polygroup = 0;
		isTransition = false;
		transition = 0;
		transTimer = -1;
		mapMode = false;
		color = 32767;
        frameCounter = interFrameCounter = currentFrame = 0;
        animNbr = 0;
        defaultAnim = 0;
        currentAnim = -1;
        isStarted = isPaused = isLoop = isInterpolation = false;
        isTexture = false;
        model = new MD2Entity;
        isOutline = false;
		onceMode = ONCE_STAYLAST;
    }

    Model::~Model()
    {
        /*int N;
         for(N = 0; N < model->header.num_frames; ++N)
         	free(model->frms[N].verts);

         free(model->frms);
         free(model->uvs);
         free(model->tris);*/
		//delete model;
		 //delete image;


    }

    void Model::Draw()
    {
        //Test d'affichage
        int n = currentFrame;
        if (n < 0)
            n = 0;

        //Déclaration d'un pointeur vers un vertex
        nds_vertex_t* nextvert;
        if(n == animations[currentAnim].endFrame)
            nextvert = model->frms[animations[currentAnim].startFrame].verts;
        else
            nextvert = model->frms[n+1].verts;

		nds_vertex_t* vert;
		if(isTransition == true)
			vert = model->frms[transition].verts;
		else
			vert = model->frms[n].verts;
		//glLoadIdentity();

        //Démarrage du dessin
        //ulVertexColor(32767);
        glEnable(GL_ANTIALIAS);
        ulSetAlpha(UL_FX_ALPHA, alpha, polygroup);
        //Outline
        if (isOutline == true)
        {
            glPushMatrix();
            //On affiche une première fois le modèle en avec la couleur de l'outline (consomme 2x plus de poly)
            ulVertexBegin(GL_TRIANGLES);

            //Translation du modèle à sa position et mise en rotation du modèle dans le bon sens
            glTranslate3f32(posX-2, posY+2, (int)ulGetDepth() );

            //Rotation du modèle
			//glRotatef32i(int angle, int32 x, int32 y, int32 z);
            glRotateZi(angle);
            glRotateYi(mirrorXVal);
            glRotateXi(mirrorYVal);
            //glScalef32(4096);

            ulVertexColor(outlineColor);

            if (!isInterpolation)
            {
                //Dessin des vertex
                for (int i = 0; i < model->header.num_tris; ++i)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        int stnum = model->tris[i].st[j];
                        int vertnum = model->tris[i].vertex[j];

                        glVertex3v16(vert[vertnum].v[0], vert[vertnum].v[1], vert[vertnum].v[2]);
                        /*GFX_VERTEX10 = f32tov10_2(vert[vertnum].v[0]);
                        GFX_VERTEX10 = f32tov10_2(vert[vertnum].v[1]);
                        GFX_VERTEX10 = f32tov10_2(vert[vertnum].v[2]);*/
                    }
                }
            }
            else
            {
                //Dessin des vertex
                for (int i = 0; i < model->header.num_tris; ++i)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        int stnum = model->tris[i].st[j];
                        int vertnum = model->tris[i].vertex[j];

                        //On calcule le pourcentage
                        v16 interpol = animations[currentAnim].pas*frameCounter;
                        v16 vx = fxtoi(Mulfx(interpol, itofx(nextvert[vertnum].v[0] - vert[vertnum].v[0]) ) );
                        v16 vy = fxtoi(Mulfx(interpol, itofx(nextvert[vertnum].v[1] - vert[vertnum].v[1]) ) );
                        glVertex3v16(vert[vertnum].v[0] + vx, vert[vertnum].v[1] + vy, vert[vertnum].v[2]);
                        /*GFX_VERTEX10 = f32tov10_2(vx + vert[vertnum].v[0]);
                        GFX_VERTEX10 = f32tov10_2(vy + vert[vertnum].v[1]);
                        GFX_VERTEX10 = f32tov10_2(vert[vertnum].v[2]);*/
                    }
                }
            }

            ulVertexEnd();
            glPopMatrix(1);
            ulVertexHandleDepth();
        }
		ulSetAlpha(UL_FX_DEFAULT, 0, 0);

        glPushMatrix();
		
		ulVertexColor(32767);
		ulSetAlpha(UL_FX_ALPHA, alpha, polygroup+1);
        //On désactive la texture courante (juste pour le debug)
        ulVertexBegin(GL_TRIANGLES);
        //Translation du modèle à sa position et mise en rotation du modèle dans le bon sens
        glTranslate3f32(posX, posY, (int)ulGetDepth() );

        //Rotation du modèle
        glRotateZi(angle);
        glRotateYi(mirrorXVal);
        glRotateXi(mirrorYVal);

        //Tex
        if (!isTexture)
        {
            ulVertexColor(color);
            ulDisableTexture();
        }
        else
        {
            ulSetTexture(image->img);
			ulVertexColor(color);
        }

        if (!isInterpolation)
        {
            //Dessin des vertex
            for (int i = 0; i < model->header.num_tris; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    int stnum = model->tris[i].st[j];
                    int vertnum = model->tris[i].vertex[j];

                    glTexCoord2t16(model->uvs[stnum].s, model->uvs[stnum].t);
                    glVertex3v16(vert[vertnum].v[0], vert[vertnum].v[1], vert[vertnum].v[2]);
                }
            }
        }
        else
        {
            //Dessin des vertex
            for (int i = 0; i < model->header.num_tris; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    int stnum = model->tris[i].st[j];
                    int vertnum = model->tris[i].vertex[j];

                    glTexCoord2t16(model->uvs[stnum].s, model->uvs[stnum].t);

                    //On calcule le pourcentage
                    v16 interpol = animations[currentAnim].pas*frameCounter;
                    v16 vx = fxtoi(Mulfx(interpol, itofx(nextvert[vertnum].v[0] - vert[vertnum].v[0]) ) );
                    v16 vy = fxtoi(Mulfx(interpol, itofx(nextvert[vertnum].v[1] - vert[vertnum].v[1]) ) );
                    glVertex3v16(vert[vertnum].v[0] + vx, vert[vertnum].v[1] + vy, vert[vertnum].v[2] );
                }
            }
        }
        ulVertexEnd();
        ulSetAlpha(UL_FX_DEFAULT, 0, 0);
        ulVertexHandleDepth();
        ulDisableTexture();
        glPopMatrix(1);
    }

    void Model::Play()
    {
        if (!isPaused && isStarted)
        {
            //On update le framecounter
			frameCounter++;
			
			//On update le counter des transition
			if(transTimer >= 0)
				transTimer++;
			if(transTimer >= 10)
			{
				transTimer = -1;
				isTransition = false;
			}
            //On vérifie l'état du framecounter
            if (frameCounter > animations[currentAnim].fps)
            {
                frameCounter = 0;
                currentFrame++;
            }

            //On vérifie l'état de la frame courante (cas loop)
            if (isLoop && currentFrame > animations[currentAnim].endFrame)
                currentFrame = animations[currentAnim].startFrame;

            //On vérifie si l'animation est finie
            if (!isLoop)
            {
                if (currentFrame >= animations[currentAnim].endFrame)
                {
                    this->Stop();
                    if (onceMode == ONCE_STAYFIRST)
                    {
                        currentFrame = animations[currentAnim].startFrame;
                    }
                    if (onceMode == ONCE_STAYLAST)
                    {
                        currentFrame = animations[currentAnim].endFrame;
                    }
                }
            }
        }
    }

    void Model::LoadModel(char* filename, int widthheight, int scale)
    {
        //On charge un modèle depuis le fichier
        bool texturereverse = true;
        FILE *fp = fopen(filename,"rb");
        if (fp == NULL)
        {
            ulDebug("wasnt found");
            return;
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

        ulDebug("vert:%d\n", model->header.num_vertices);
        ulDebug("tris:%d\n", model->header.num_tris);
        ulDebug("frames:%d\n", model->header.num_frames);
    }

    void Model::AssignModel(MD2Entity* Model)
    {
        model = Model;
    }

    void Model::Interpolate(bool trigger)
    {
        isInterpolation = trigger;
    }

    int Model::AddAnim(u8 sta, u8 end, u8 fps)
    {
        //Déclaration d'une structure anim temporaire
        sModelAnim tmpAnim = {sta, end, fps, 256/fps};
        //Ajout au vector
        animations.push_back(tmpAnim);
        animNbr++;
        return animNbr;
    }

    void Model::PlayAnim(int mode, int anim, bool isTrans)
    {
        //Reset de l'ancienne animation
        switch (mode)
        {
        case ANIM_LOOP:
            //Reset de l'ancienne animation
            if (currentAnim != anim)
            {
                frameCounter = 0;
				transition = currentFrame;
				isTransition = isTrans;
				transTimer = 0;
                currentFrame = animations[anim].startFrame;
                //Démarrage d'une animation
                isStarted = true;
                isLoop = true;
                //Settings
                currentAnim = anim;
            }
            break;

        case ANIM_ONCE:
            if (currentAnim != anim)
            {
                frameCounter = 0;
				transition = currentFrame;
				isTransition = isTrans;
				transTimer = 0;
                currentFrame = animations[anim].startFrame;
                //Démarrage d'une animation
                isStarted = true;
                isLoop = false;
                //Settings
                currentAnim = anim;
            }
            break;
        }
    }

    void Model::SetAnimationSpeed(u8 animation, int speed)
    {
        animations[animation].fps = speed;
        animations[animation].pas = 256/speed;
    }

    void Model::Pause()
    {
        isPaused = true;
    }

    void Model::Restart()
    {
        isPaused = false;
    }

    void Model::Stop()
    {
        frameCounter = 0;
        currentFrame--;
        isStarted = false;
    }

    void Model::Tint(int col)
    {
        color = col;
    }

    void Model::Outline(int color, bool trigger)
    {
        isOutline = trigger;
        outlineColor = color;
    }

    void Model::SetOnceMonde(int mode)
    {
        onceMode = mode;
    }
}
