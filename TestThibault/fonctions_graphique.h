#include "fonctions_SDL.h"
#ifndef FONCTIONS_GRAPHIQUE_H
#define FONCTIONS_GRAPHIQUE_H

//Fonction servant à gérer les sprites

void positionImageSol();
void positionImageCarre(SDL_Rect* SrcRc,SDL_Rect* DestRc);
void positionImagePatate(SDL_Rect *SrcRp, SDL_Rect *DestRp);

#endif