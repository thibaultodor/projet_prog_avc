#include "fonctions_SDL.h"

SDL_Texture* charger_image(const char* nomfichier, SDL_Renderer* renderer){
	SDL_Surface * image = SDL_LoadBMP(nomfichier);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
	return texture;
}

SDL_Texture* charger_image_transparente(const char* nomfichier,SDL_Renderer* renderer,Uint8 r, Uint8 g, Uint8 b){
	SDL_Surface * obj = SDL_LoadBMP(nomfichier);
	SDL_SetColorKey(obj,SDL_TRUE,SDL_MapRGB(obj->format, r, g, b));
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, obj);
	return texture;
}

SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer,TTF_Font *font, SDL_Color color){
	SDL_Surface * txt = TTF_RenderText_Solid(font,message,color);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, txt);
	return texture;
}
