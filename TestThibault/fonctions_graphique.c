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
	DestRc->h = 175;
}

void positionImageVie(SDL_Rect* DestRv){
	DestRv->x = 400;
	DestRv->y = 20;
	DestRv->w = 30;
	DestRv->h = 30;
}

void positionImageSons(SDL_Rect* DestRs){
	DestRs->x = 200;
	DestRs->y = 150;
	DestRs->w = 200;
	DestRs->h = 200;
}

void positionImageBack(SDL_Rect* DestRb){
	DestRb->x = 20;
	DestRb->y = 530;
	DestRb->w = 100;
	DestRb->h = 50;
}

void positionscoreboard(SDL_Rect* DestRscore){
	DestRscore->x = 160;
	DestRscore->y = 190;
	DestRscore->w = 300;
	DestRscore->h = 50;
}

void positionSpriteCuistotMenu(SDL_Rect* DestRSprCuistot){
	DestRSprCuistot->x = 20;
	DestRSprCuistot->y = 410;
	DestRSprCuistot->w = 80;
	DestRSprCuistot->h = 200;
}

void deplacementPatate(patate_t *P){
	if(P->droit == true){P->DestR.x = P->DestR.x - P->vitesse;}
	else{P->DestR.x = P->DestR.x + P->vitesse;}
	P->RotationImage++;
	if(P->vie == false){P->DestR.y = P->DestR.y+10;}
}

void retourPatateMenu(patate_t *P){
	if(P->droit == true){P->DestR.x = 600 + rand() % 400;}
	else{P->DestR.x = -50 - rand() % 400;}
	P->RotationImage=0;
	P->patate_interval=false;
	P->cri_arrive=false;
	P->cri_mort=false;
}


void retourPatate(patate_t *P){
	if(P->droit == true){P->DestR.x = 600 + rand() % 400;}
	else{P->DestR.x = -50 - rand() % 400;}
	P->DestR.y = 410-rand()%20;
	P->RotationImage=0;
	P->patate_interval=false;
	P->cri_arrive=false;
	P->cri_mort=false;
	P->vie=true;
}

void clignotement_texte (SDL_Texture** t,int* tick,char msg[],TTF_Font *fontmenu,SDL_Renderer* ecran,int* swtch){
	if(*tick == 50){
		if (*swtch==0){SDL_Color color = {250,0,0,0};*swtch=1;SDL_DestroyTexture(*t);*t = charger_texte(msg,ecran,fontmenu,color);}
		else{SDL_Color color = {0,0,0,0};*swtch=0;SDL_DestroyTexture(*t);*t = charger_texte(msg,ecran,fontmenu,color);}
		*tick = 0;
	}
	else{*tick = *tick+1;}
}

void remise_en_noir (SDL_Texture** t,int* tick,char msg[],TTF_Font *font,SDL_Renderer* ecran,int* swtch){
	SDL_Color color = {0,0,0,0};
	*swtch=0;
	SDL_DestroyTexture(*t);
	*t = charger_texte(msg,ecran,font,color);
	*tick=0;
}