#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fonctions_SDL.h"

int main(int argc, char *argv[])	{
	SDL_Window* fenetre; // Déclaration de la fenêtre
	SDL_Event evenements; // Événements liés à la fenêtre
	bool terminer = false;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) // Initialisation de la SDL
	{
	printf("Erreur d’initialisation de la SDL: %s",SDL_GetError());
	SDL_Quit();
	return EXIT_FAILURE;
	}

	// Créer la fenêtre
	fenetre = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE);
	if(fenetre == NULL) // En cas d’erreur
	{
	printf("Erreur de la creation d’une fenetre: %s",SDL_GetError());
	SDL_Quit();
	return EXIT_FAILURE;
	}

	SDL_Renderer * ecran = SDL_CreateRenderer(fenetre, -1, 0);

	//Image fond
	SDL_Texture* fond = charger_image("green.bmp",ecran);

    //Image sol
	SDL_Texture* sol = charger_image("fond.bmp",ecran);
	SDL_Surface * surface_obj = SDL_LoadBMP("fond.bmp");
	int tailleW = surface_obj->w;
	int tailleH = surface_obj->h;
	SDL_FreeSurface(surface_obj);
	SDL_Rect SrcR,DestR;
	SrcR.x = tailleW/4;
	SrcR.y = tailleH-50;
	SrcR.w = tailleW/2;
	SrcR.h = 30;
	DestR.x = 0;
	DestR.y = 550;
	DestR.w = 600;
	DestR.h = 50;

	//Image sprite carré
	SDL_Texture* sdcarre = charger_image("sprite_droite.bmp",ecran);
	SDL_Texture* sgcarre = charger_image("sprite_gauche.bmp",ecran);
	SDL_Rect SrcRc,DestRc;
	SrcRc.x = 0;
	SrcRc.y = 0;
	SrcRc.w = 500;
	SrcRc.h = 500;
	DestRc.x = 275;
	DestRc.y = 600-100;
	DestRc.w = 50;
	DestRc.h = 50;

	//Image sprite patate
	SDL_Texture* spatate = charger_image("sprite_patate.bmp",ecran);
	SDL_Rect SrcRp,DestRp;
	SrcRp.x = 0;
	SrcRp.y = 0;
	SrcRp.w = 500;
	SrcRp.h = 500;
	DestRp.x = 600-50;
	DestRp.y = 600-200;
	DestRp.w = 50;
	DestRp.h = 50;

	//Image sprite chat
	/*Uint8 r = 0, g = 255, b = 255;
	SDL_Texture* sprites = charger_image_transparente("sprites.bmp",ecran,r,g,b);

	SDL_Surface * surface_sprite = SDL_LoadBMP("sprites.bmp");
	int tailleWs = surface_sprite->w/3;
	int tailleHs = surface_sprite->h/2;
	SDL_FreeSurface(surface_sprite);

	SDL_Rect SrcR_sprite[6];
	for(int i=0; i<6; i++){
		SrcR_sprite[i].x = i > 2 ? tailleWs*(i-3) : tailleWs*(i);
		SrcR_sprite[i].y = i > 2 ? tailleHs*(1) : tailleHs*(0);
		SrcR_sprite[i].w = tailleWs; // Largeur du sprite
		SrcR_sprite[i].h = tailleHs; // Hauteur du sprite
	}

	SDL_Rect DestR_sprite[6];
	for(int i=0; i<6; i++){
		DestR_sprite[i].x = 300-tailleWs/2;
		DestR_sprite[i].y = 440;
		DestR_sprite[i].w = tailleWs; // Largeur du sprite
		DestR_sprite[i].h = tailleHs; // Hauteur du sprite
	}

	SDL_Rect* spriterectsrc = &SrcR_sprite[1];
	SDL_Rect* spriterectdest = &DestR_sprite[1];
	*/

	SDL_Texture* spritecarree = sdcarre;
	int temp_position = DestRp.x--;

	// Boucle principale
	while(!terminer){
		//Affichage
		SDL_RenderClear(ecran);
		SDL_RenderCopy(ecran, fond, NULL, NULL);
    	SDL_RenderCopy(ecran, sol, &SrcR, &DestR);
    	SDL_RenderCopy(ecran, spritecarree, &SrcRc, &DestRc);
    	SDL_RenderCopy(ecran, spatate, &SrcRp, &DestRp);
    	//SDL_RenderCopy(ecran, sprites, spriterectsrc, spriterectdest);
		SDL_RenderPresent(ecran);

		while( SDL_PollEvent( &evenements ) )
		switch(evenements.type){
			case SDL_QUIT:
			terminer = true; break;

		case SDL_KEYDOWN:
			switch(evenements.key.keysym.sym){
			case SDLK_ESCAPE:case SDLK_q:terminer = true; break;
			case SDLK_LEFT: spritecarree = sgcarre;break;
			case SDLK_RIGHT: spritecarree = sdcarre;break;
			}
		}
		SDL_Delay(10);
		DestRp.x--;
		if(DestRp.x<-50){DestRp.x = temp_position+50;}
	}

	// Quitter SDL
	SDL_DestroyTexture(fond);
	SDL_DestroyTexture(sol);
	SDL_DestroyTexture(spritecarree);
	//SDL_DestroyTexture(sprites);
	SDL_DestroyRenderer(ecran);
	SDL_Quit();
	return 0;
	}
