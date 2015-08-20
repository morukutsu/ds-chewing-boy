/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_PLATFORM_MAIN
#define H_PLATFORM_MAIN

#define IS_NDS
//#define IS_PSP
//#define IS_PC
//#define IS_WII

//Ce fichier demande le fichier à inclure en fonction de la plateforme cible
#if defined(IS_NDS)
#include "LowLevel/Platform.h"
#endif

#endif

