#include "fonctions_SDL.h"
#include <stdbool.h>
#ifndef FONCTIONS_GRAPHIQUE_H
#define FONCTIONS_GRAPHIQUE_H


//Structure gestion patate
typedef struct patate_s patate_t;
struct patate_s{
	bool vie;//en vie ou non
	bool droit;//vrai = arrive de droite faux = arrive de gauche
	int type;//type de patate
	int vitesse;//Par defaut vitesse = 1
	int RotationImage;
	bool patate_interval;
	bool cri_arrive;
	bool cri_mort;
	SDL_Rect DestR;//Position de la patate
};


//Fonction servant à gérer les sprites

void positionImageSol(SDL_Rect *SrcR, SDL_Rect *DestR,int W, int H);
void positionImageCarre(SDL_Rect* DestRc);
void positionImageVie(SDL_Rect* DestRv);
void positionImageSons(SDL_Rect* DestRs);
void deplacementPatate(patate_t *P);
void retourPatate(patate_t *P);
void clignotement_texte (SDL_Texture** t,int* tick,char msg[],TTF_Font *font,SDL_Renderer* renderer,int* swtch);
void remise_en_noir (SDL_Texture** t,int* tick,char msg[],TTF_Font *font,SDL_Renderer* ecran,int* swtch);

#endif