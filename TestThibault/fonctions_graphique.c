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

void positionImageCarre(SDL_Rect* DestRc){
	DestRc->x = 275;
	DestRc->y = 600-250;
	DestRc->w = 50;
	DestRc->h = 200;
}

void positionImageVie(SDL_Rect* DestRv){
	DestRv->x = 400;
	DestRv->y = 10;
	DestRv->w = 30;
	DestRv->h = 30;
}

void positionImageSons(SDL_Rect* DestRs){
	DestRs->x = 500;
	DestRs->y = 20;
	DestRs->w = 50;
	DestRs->h = 50;
}

void positionImagePatateArriveDroite(patate_t *pD){
	pD->DestR.x = 600-50;
	pD->DestR.y = 600-200;
	pD->DestR.w = 50;
	pD->DestR.h = 50;
}

void positionImagePatateArriveGauche(patate_t *pG){
	pG->DestR.x = -50;
	pG->DestR.y = 600-200;
	pG->DestR.w = 50;
	pG->DestR.h = 50;
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