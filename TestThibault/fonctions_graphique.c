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
	DestRs->x = 540;
	DestRs->y = 540;
	DestRs->w = 50;
	DestRs->h = 50;
}

void deplacementPatate(patate_t *P){
	if(P->droit == true){P->DestR.x = P->DestR.x - P->vitesse;}
	else{P->DestR.x = P->DestR.x + P->vitesse;}	
	P->RotationImage++;
}


void retourPatate(patate_t *P){
	if(P->droit == true){P->DestR.x = 600;}
	else{P->DestR.x = -50;}
	P->RotationImage=0;
}