#include <stdbool.h>
#include "fonctions_score.h"
#include "fonctions_graphique.h"

int main()	{
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

	int best_score = 0;	//Bool du best score
	int score = 0; //Initialisation du score

	//Ouverture fichier score
	FILE * pFile;
	pFile=fopen ("score.txt","r");

	//Initialisation renderer
	SDL_Renderer * ecran = SDL_CreateRenderer(fenetre, -1, 0);

	//Image fond
	SDL_Texture* fond = charger_image("green.bmp",ecran);

    //Image sol
	SDL_Texture* sol = charger_image("fond.bmp",ecran);
	SDL_Surface * surface_obj = SDL_LoadBMP("fond.bmp");
	SDL_FreeSurface(surface_obj);
	int tailleW = surface_obj->w;
	int tailleH = surface_obj->h;
	SDL_Rect SrcR,DestR;
	positionImageSol(&SrcR,&DestR,tailleW,tailleH);

	//Image sprite carré (joueur)
	SDL_Texture* sdcarre = charger_image("sprite_droite.bmp",ecran);
	SDL_Texture* sgcarre = charger_image("sprite_gauche.bmp",ecran);
	SDL_Rect SrcRc,DestRc;
	positionImageCarre(&SrcRc,&DestRc);

	//Sauvegarde des positions du sprite joueur
	int posx_patate_attack_gauche = DestRc.x + DestRc.w;
	int posy_patate_attack_gauche_bas = DestRc.y + DestRc.h;
	int posy_patate_attack_gauche_haut = DestRc.y;

	//Image sprite patate
	SDL_Texture* spatate = charger_image("sprite_patate.bmp",ecran);
	SDL_Texture* spatate_alive = charger_image("sprite_patate_nodegat.bmp",ecran);
	SDL_Texture* spatate_ko = charger_image("sprite_patate_degat.bmp",ecran);
	SDL_Rect SrcRp,DestRp;
	positionImagePatate(&SrcRp,&DestRp);

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

	//Gestion affichage texte pour score
	TTF_Init(); 
	TTF_Font *font = TTF_OpenFont("./arial.ttf",25);
	SDL_Color color = {0,0,0,0};
	SDL_Color color_red = {250,0,0,0};
	int tick_color_red = 0;

	int meilleur_score_fichier = lireHighScore(pFile);

	char msg_score_max[] = "Score max : xx";	//Gestion score max
	SDL_Texture* texte = charger_texte_score(meilleur_score_fichier,ecran,font,color);
	SDL_Surface * msg_score_max_surface = TTF_RenderText_Solid(font,msg_score_max,color);
	SDL_Rect text_posm; // Position du texte
	text_posm.x = 0;
	text_posm.y = 10;
	text_posm.w = msg_score_max_surface->w;// Largeur du texte en pixels (à récupérer)
	text_posm.h = msg_score_max_surface->h;// Hauteur du texte en pixels (à récupérer)
	SDL_FreeSurface(msg_score_max_surface);

	char msg_score[] = "Score : xx";	//Gestion score actuel
	SDL_Texture* texte_score = charger_texte_score_actu(0,ecran,font,color);
	SDL_Surface * msg_score_surface = TTF_RenderText_Solid(font,msg_score,color);
	SDL_Rect text_pos; // Position du texte
	text_pos.x = 0;
	text_pos.y = 40;
	text_pos.w = msg_score_surface->w;// Largeur du texte en pixels (à récupérer)
	text_pos.h = msg_score_surface->h;// Hauteur du texte en pixels (à récupérer)
	SDL_FreeSurface(msg_score_surface);


	SDL_Texture* spritecarree = sdcarre; //Texture tampon du du joueur
	SDL_Texture* spritepatate = spatate; //Texture tampon de la patate
	int temp_position = DestRp.x+50; //Postition de base de la patate

	// Boucle principale
	while(!terminer){
		//Affichage
		SDL_RenderClear(ecran);
		SDL_RenderCopy(ecran, fond, NULL, NULL);
    	SDL_RenderCopy(ecran, sol, &SrcR, &DestR);
    	SDL_RenderCopy(ecran, spritecarree, &SrcRc, &DestRc);
    	SDL_RenderCopy(ecran, spritepatate, &SrcRp, &DestRp);
    	SDL_RenderCopy(ecran, texte, NULL, &text_posm);
    	SDL_RenderCopy(ecran, texte_score, NULL, &text_pos);
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
			//Initialisation aprés appui sur bouton delete 'd' (supprime tout les score et reset) A TRANSFORMER EN FONCTION
			case SDLK_d:resetScore(pFile);texte = charger_texte_score(0,ecran,font,color);best_score = 1;score=0;texte_score = charger_texte_score_actu(score,ecran,font,color);meilleur_score_fichier = 0;tick_color_red = -1;break;
			//Initialisation aprés appui sur bouton reset 'r' (reset du score actuel) A TRANSFORMER EN FONCTION
			case SDLK_r:ecrireScore(score,pFile);score=0;texte_score = charger_texte_score_actu(score,ecran,font,color);best_score=0;meilleur_score_fichier = lireHighScore(pFile);break;
			}
		}
		SDL_Delay(10);
		DestRp.x--;	//Deplacement patate
		if(DestRp.x<-50){spritepatate = spatate;DestRp.x = temp_position;} //Retour de la patate aprés avoir quitté l'écran

		if (DestRp.x == posx_patate_attack_gauche){//Gestion score plus mort ou vie de la patate
			if(DestRp.y > posy_patate_attack_gauche_haut && DestRp.y < posy_patate_attack_gauche_bas){
				if (spritecarree == sdcarre){spritepatate = spatate_ko;score++;texte_score = charger_texte_score_actu(score,ecran,font,color);}
				if (spritecarree == sgcarre){spritepatate = spatate_alive;ecrireScore(score,pFile);score=0;texte_score = charger_texte_score_actu(score,ecran,font,color);best_score=0;meilleur_score_fichier = lireHighScore(pFile);}
			}

		}

		if (best_score == 0){
			if (meilleur_score_fichier == score){best_score = 1;tick_color_red = 0;}}//Verifie si le score actuel est supérieur au score max
		else{
			if (meilleur_score_fichier == 0){tick_color_red = -1;}
			if (tick_color_red >= 0){tick_color_red++;}
			if (tick_color_red < 100 && tick_color_red >= 0){texte = charger_texte_score(score,ecran,font,color_red);}
			else{
				tick_color_red = -1;
				texte = charger_texte_score(score,ecran,font,color);
			}
		}//Affiche le score actuel en tant que score max
	}
	ecrireScore(score,pFile);

	// Quitter et nettoyer SDL
	SDL_DestroyTexture(fond);
	SDL_DestroyTexture(sol);
	SDL_DestroyTexture(spritecarree);
	//SDL_DestroyTexture(sprites);
	SDL_DestroyRenderer(ecran);
	SDL_DestroyWindow(fenetre);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	return 0;
	}
