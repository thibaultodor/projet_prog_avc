#include "fonctions_SDL.h"
#include <stdbool.h>
#ifndef FONCTIONS_GRAPHIQUE_H
#define FONCTIONS_GRAPHIQUE_H


//Structure gestion patate
typedef struct patate_s patate_t;
struct patate_s{
	bool vie;//en vie ou non
	int droit;
	int gauche;
	int type;//type de patate
	//SDL_Rect SrcR;
	SDL_Rect DestR;//Position de la patate
};


//Fonction servant à gérer les sprites

void positionImageSol(SDL_Rect *SrcR, SDL_Rect *DestR,int W, int H);
void positionImageCarre(SDL_Rect* DestRc);
void positionImageVie(SDL_Rect* DestRv);
void positionImageSons(SDL_Rect* DestRs);
void positionImagePatateArriveDroite(patate_t *pD);
void positionImagePatateArriveGauche(patate_t *pG);
void deplacementPatateArriveDroite(SDL_Rect *DestRp,int v);
void deplacementPatateArriveGauche(SDL_Rect *DestRp,int v);
void retourPatateArriveDroite(SDL_Rect *DestRp);
void retourPatateArriveGauche(SDL_Rect *DestRp);

#endif