#include "fonctions_graphique.h"

void positionImageSol(SDL_Rect *SrcR, SDL_Rect *DestR,int W, int H){
	SrcR->x = W/4;
	SrcR->y = H-50;
	SrcR->w = W/2;
	SrcR->h = 30;
	DestR->x = 0;
	DestR->y = 550;
	DestR->w = 600;
	DestR->h = 50;
}

void positionImageCarre(SDL_Rect* SrcRc,SDL_Rect* DestRc){
	SrcRc->x = 0;
	SrcRc->y = 0;
	SrcRc->w = 500;
	SrcRc->h = 2000;
	DestRc->x = 275;
	DestRc->y = 600-250;
	DestRc->w = 50;
	DestRc->h = 200;
}

void positionImagePatateArriveDroite(SDL_Rect *SrcRp, SDL_Rect *DestRp){
	SrcRp->x = 0;
	SrcRp->y = 0;
	SrcRp->w = 500;
	SrcRp->h = 500;
	DestRp->x = 600-50;
	DestRp->y = 600-200;
	DestRp->w = 50;
	DestRp->h = 50;
}

void positionImagePatateArriveGauche(SDL_Rect *SrcRp, SDL_Rect *DestRp){
	SrcRp->x = 0;
	SrcRp->y = 0;
	SrcRp->w = 500;
	SrcRp->h = 500;
	DestRp->x = -50;
	DestRp->y = 600-200;
	DestRp->w = 50;
	DestRp->h = 50;
}

void deplacementPatateArriveDroite(SDL_Rect *DestRp,int v){
	DestRp->x = DestRp->x - v;
}

void deplacementPatateArriveGauche(SDL_Rect *DestRp,int v){
	DestRp->x = DestRp->x + v;
}

void retourPatateArriveDroite(SDL_Rect *DestRp){
	DestRp->x = 600;
}

void retourPatateArriveGauche(SDL_Rect *DestRp){
	DestRp->x = -50;
}