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

SDL_Texture* charger_texte_score(int score, SDL_Renderer* renderer,TTF_Font *font, SDL_Color color){
  	char score_str[5]; 
  	if (score < 10){sprintf(score_str, "0%d", score);}
  	else{sprintf(score_str, "%d", score);}
	char message[25] = "Score max : ";
	for (long unsigned int i = 0; i < strlen(score_str); i++)
    {message[12+i] = score_str[i];}
	SDL_Surface * txt = TTF_RenderText_Solid(font,message,color);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, txt);
	return texture;
}

SDL_Texture* charger_texte_score_actu(int score, SDL_Renderer* renderer,TTF_Font *font, SDL_Color color){
  	char score_str[5];
  	if (score < 10){sprintf(score_str, "0%d", score);}
  	else{sprintf(score_str, "%d", score);}
	char message[25] = "Score : ";
	for (long unsigned int i = 0; i < strlen(score_str); i++)
    {message[8+i] = score_str[i];}
	SDL_Surface * txt = TTF_RenderText_Solid(font,message,color);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, txt);
	return texture;
}

SDL_Texture* charger_scoreboard(int * tab,int position,SDL_Renderer* renderer,TTF_Font *font, SDL_Color color){
	char message[25];
	if (position == 0){strcpy(message,"1er -> xx POINTS");}
	else{
		strcpy(message,"xeme -> xx POINTS");
		char rang[2];
		int pos_temp = position+1;
		sprintf(rang, "%d", pos_temp);
		message[0] = rang[0];
	}
	if(tab[position] == 0){strcpy(message," ");}
	else{
		char score_str[5];
  		if (tab[position] < 10){sprintf(score_str, "0%d", tab[position]);}
  		else{sprintf(score_str, "%d", tab[position]);}
  		for (long unsigned int i = 0; i < strlen(score_str); i++){
        	if(position == 0){message[7+i] = score_str[i];}
        	else{message[8+i] = score_str[i];}
    	}
	}
  	SDL_Surface * txt = TTF_RenderText_Solid(font,message,color);
  	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, txt);
	return texture;
}