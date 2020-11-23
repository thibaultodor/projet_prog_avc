#include "fonctions_SDL.h"
#ifndef FONCTIONS_GRAPHIQUE_H
#define FONCTIONS_GRAPHIQUE_H

//Fonction servant à gérer les sprites

void positionImageSol();
void positionImageCarre(SDL_Rect* SrcRc,SDL_Rect* DestRc);
void positionImageVie(SDL_Rect* SrcRv,SDL_Rect* DestRv);
void positionImagePatateArriveDroite(SDL_Rect *SrcRp, SDL_Rect *DestRp);
void positionImagePatateArriveGauche(SDL_Rect *SrcRp, SDL_Rect *DestRp);
void deplacementPatateArriveDroite(SDL_Rect *DestRp,int v);
void deplacementPatateArriveGauche(SDL_Rect *DestRp,int v);
void retourPatateArriveDroite(SDL_Rect *DestRp);
void retourPatateArriveGauche(SDL_Rect *DestRp);

#endif