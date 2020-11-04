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
	SDL_Texture* fond = charger_image("grey.bmp",ecran);

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

	//Image perso
	Uint8 r = 0, g = 255, b = 255;
	SDL_Texture* sprites = charger_image_transparente("sprites.bmp",ecran,r,g,b);

	SDL_Surface * surface_sprite = SDL_LoadBMP("sprites.bmp");
	int tailleWs = surface_sprite->w/3;
	int tailleHs = surface_sprite->h/2;
	SDL_FreeSurface(surface_sprite);

	SDL_Rect SrcR_sprite,DestR_sprite;
	SrcR_sprite.x = 0;
	SrcR_sprite.y = 0;
	SrcR_sprite.w = tailleWs; // Largeur du sprite
	SrcR_sprite.h = tailleHs; // Hauteur du sprite
	DestR_sprite.x = tailleW/2 - tailleWs/2;
	DestR_sprite.y = 550 - tailleHs;
	DestR_sprite.w = tailleWs; // Largeur du sprite
	DestR_sprite.h = tailleHs; // Hauteur du sprite


	SDL_RenderClear(ecran);
	SDL_RenderCopy(ecran, fond, NULL, NULL);
    SDL_RenderCopy(ecran, sol, &SrcR, &DestR);
    SDL_RenderCopy(ecran, sprites, &SrcR_sprite, &DestR_sprite);
	SDL_RenderPresent(ecran);

	// Boucle principale
	while(!terminer){
		while( SDL_PollEvent( &evenements ) )
		switch(evenements.type){
			case SDL_QUIT:
			terminer = true; break;

		case SDL_KEYDOWN:
			switch(evenements.key.keysym.sym){
			case SDLK_ESCAPE:
			case SDLK_q:
			terminer = true; break;
			}
		}
	}

	// Quitter SDL
	SDL_DestroyTexture(fond);
	SDL_DestroyTexture(sol);
	SDL_DestroyTexture(sprites);
	SDL_DestroyRenderer(ecran);
	SDL_Quit();
	return 0;
	}
