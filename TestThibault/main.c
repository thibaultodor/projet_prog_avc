#include <stdbool.h>
#include "fonctions_score.h"
#include "fonctions_jeu.h"
#include "fonctions_graphique.h"
#include "audio.h"

int main()	{
	SDL_Window* fenetre; // Declaration de la fenetre
	SDL_Event evenements; // evenements lies e la fenetre
	bool terminer = false;
	//Structure gestion joueur
	typedef struct joueur_s joueur_t;
	struct joueur_s{
		int vie;
		int droit;
		int gauche;
	};
	//Initialistion du joueur
	joueur_t *joueur=malloc(sizeof(joueur_t));
	joueur->vie=3;
	joueur->droit=1;
	joueur->gauche=0;

	
	// Initialisation audio
	SDL_Init(SDL_INIT_AUDIO);
	initAudio();
	//Demarrage musique
	int audio = 8;
	playMusic("road.wav", audio);

	if(SDL_Init(SDL_INIT_VIDEO) < 0) // Initialisation de la SDL
	{
	printf("Erreur d'initialisation de la SDL: %s",SDL_GetError());
	SDL_Quit();
	return EXIT_FAILURE;
	}

	// Creer la fenetre
	fenetre = SDL_CreateWindow("JEU", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE);
	if(fenetre == NULL) // En cas d'erreur
	{
	printf("Erreur de la creation d'une fenetre: %s",SDL_GetError());
	SDL_Quit();
	return EXIT_FAILURE;
	}

	int best_score = 0;	//Bool du best score
	int score = 0; //Initialisation du score

	Uint8 r = 0, g = 255, b = 255;

	//Ouverture fichier score
	FILE * pFile;
	pFile=fopen ("score.txt","r");

	//Initialisation renderer
	SDL_Renderer *ecran = SDL_CreateRenderer(fenetre, -1, 0);

	//Image fond
	SDL_Texture* fond = charger_image("fond_frite.bmp",ecran);

    //Image sfond in game
	SDL_Texture* fond_ingame = charger_image("fond_cuisine_blanc.bmp",ecran);

	//Image sprite carre (joueur)
	/*
	SDL_Texture* sdcarre = charger_image("sprite_droite.bmp",ecran);
	SDL_Texture* sgcarre = charger_image("sprite_gauche.bmp",ecran);
	*/
	SDL_Texture* sdcarre = charger_image_transparente("PATATE/sprite_patate_nodegat.bmp",ecran,r,g,b);
	SDL_Texture* sgcarre = charger_image_transparente("PATATE/sprite_patate_nodegat.bmp",ecran,r,g,b);
	SDL_Rect DestRc;
	positionImageCarre(&DestRc);

	SDL_Texture* sprite_cuistot_menu = charger_image_transparente("spritecuistot/cuistot_menu.bmp",ecran,r,g,b);
	SDL_Rect DestRSprite_menu;
	positionSpriteCuistotMenu(&DestRSprite_menu);

	//////////////////////////////SPRITE CUISTOT///////////////////////////////
	SDL_Rect Position_cuistot_d_h,Position_cuistot_g_h,Position_cuistot_d_b,Position_cuistot_g_b;

	SDL_Texture* sprite_cuistot_droit_haut = charger_image_transparente("spritecuistot/Cuistot_droit_haut.bmp",ecran,r,g,b);
	SDL_Texture* sprite_cuistot_gauche_haut = charger_image_transparente("spritecuistot/Cuistot_gauche_haut.bmp",ecran,r,g,b);
	Position_cuistot_d_h.x=250;Position_cuistot_g_h.x=260;
	Position_cuistot_d_h.y=350;Position_cuistot_g_h.y=350;
	Position_cuistot_d_h.w=90;Position_cuistot_g_h.w=90;
	Position_cuistot_d_h.h=180;Position_cuistot_g_h.h=180;

	SDL_Texture* sprite_cuistot_droit_bas = charger_image_transparente("spritecuistot/Cuistot_droit_bas.bmp",ecran,r,g,b);
	SDL_Texture* sprite_cuistot_gauche_bas = charger_image_transparente("spritecuistot/Cuistot_gauche_bas.bmp",ecran,r,g,b);
	Position_cuistot_d_b.x=250;Position_cuistot_g_b.x=256;
	Position_cuistot_d_b.y=350;Position_cuistot_g_b.y=350;
	Position_cuistot_d_b.w=94;Position_cuistot_g_b.w=94;
	Position_cuistot_d_b.h=180;Position_cuistot_g_b.h=180;
	///////////////////////////////////////////////////////////////////////////

	//Sauvegarde des positions du sprite joueur patate venant de gauche
	int posx_patate_attack_gauche = DestRc.x + DestRc.w;
	//Sauvegarde des positions du sprite joueur patate venant de droite
	int posx_patate_attack_droite = 275;

	//Permet de gerer la hitbox en y du sprite du joueur
	int posy_patate_attack_bas = DestRc.y + DestRc.h;
	int posy_patate_attack_haut = DestRc.y;

	//Image sprite patate
	SDL_Texture* spatate_alive = charger_image_transparente("PATATE/sprite_patate_nodegat.bmp",ecran,r,g,b);
	SDL_Texture* spatate_ko = charger_image_transparente("PATATE/patateko.bmp",ecran,r,g,b);
	//Declaration patate plus positionnement
	SDL_Texture *simagepatate[4];
	simagepatate[0] = charger_image_transparente("PATATE/patate0.bmp",ecran,r,g,b);
	simagepatate[1] = charger_image_transparente("PATATE/patate1.bmp",ecran,r,g,b);
	simagepatate[2] = charger_image_transparente("PATATE/patate2.bmp",ecran,r,g,b);
	simagepatate[3] = charger_image_transparente("PATATE/patate3.bmp",ecran,r,g,b);

	SDL_Texture* spatate_fond = charger_image_transparente("PATATE/patate_fond.bmp",ecran,r,g,b);

	////////////////

	int nbPatate = 4;

	////////////////

	patate_t *patate[nbPatate];
	SDL_Texture *spatate[nbPatate];
	bool sens=true;
	for(int i=1;i<nbPatate+1;i++){
		patate[i-1]=malloc(sizeof(patate_t));
		if(i%2==0){sens=true;}
      	else{sens=false;}
      	creerPatate(patate[i-1],sens,i);
      	spatate[i-1] = charger_image_transparente("PATATE/patate0.bmp",ecran,r,g,b);
	}

	//Image Vie
	SDL_Texture* svie = charger_image_transparente("vie.bmp",ecran,r,g,b);
	SDL_Rect DestRv;
	positionImageVie(&DestRv);
	//Image Sons
	SDL_Texture* snosound = charger_image_transparente("soundsprite/musicno.bmp",ecran,r,g,b);
	SDL_Texture* ssound1 = charger_image_transparente("soundsprite/musicfull-2.bmp",ecran,r,g,b);
	SDL_Texture* ssound2 = charger_image_transparente("soundsprite/musicfull-1.bmp",ecran,r,g,b);
	SDL_Texture* ssound3 = charger_image_transparente("soundsprite/musicfull.bmp",ecran,r,g,b);
	SDL_Rect DestRs;
	positionImageSons(&DestRs);

	//Image retour
	SDL_Texture* back = charger_image("back.bmp",ecran);
	SDL_Rect DestBack;
	positionImageBack(&DestBack);

	//Gestion affichage texte_score_max pour score
	TTF_Init(); 
	TTF_Font *font = TTF_OpenFont("font/arial.ttf",25);
	TTF_Font *fontmenu = TTF_OpenFont("font/PressStart2P.ttf",80);
	TTF_Font *fontstart = TTF_OpenFont("font/secretsauce.ttf",90);
	TTF_Font *fontoption = TTF_OpenFont("font/secretsauce.ttf",60);
	TTF_Font *fontscore = TTF_OpenFont("font/secretsauce.ttf",60);
	TTF_Font *fontoption_sons = TTF_OpenFont("font/PressStart2P.ttf",60);
	TTF_Font *font_tuto = TTF_OpenFont("font/PressStart2P.ttf",40);
	SDL_Color color = {0,0,0,0};
	SDL_Color color_red = {250,0,0,0};
	int tick_color_red = 0;


	////////////////////////////Score dans interface du jeu (en haut a gauche)///////////////////////////
	int meilleur_score_fichier = lireHighScore(pFile);

	char msg_score_max[] = "Score max : xx";	//Gestion score max
	SDL_Texture* texte_score_max = charger_texte_score(meilleur_score_fichier,ecran,font,color);
	SDL_Surface * msg_score_max_surface = TTF_RenderText_Solid(font,msg_score_max,color);
	SDL_Rect text_posm; // Position du texte_score_max
	text_posm.x = 5;
	text_posm.y = 5;
	text_posm.w = msg_score_max_surface->w;// Largeur du texte_score_max en pixels (e recuperer)
	text_posm.h = msg_score_max_surface->h;// Hauteur du texte_score_max en pixels (e recuperer)
	SDL_FreeSurface(msg_score_max_surface);

	char msg_score[] = "Score : xx";	//Gestion score actuel
	SDL_Texture* texte_score = charger_texte_score_actu(0,ecran,font,color);
	SDL_Surface * msg_score_surface = TTF_RenderText_Solid(font,msg_score,color);
	SDL_Rect text_pos; // Position du texte_score_max
	text_pos.x = 5;
	text_pos.y = 35;
	text_pos.w = msg_score_surface->w;// Largeur du texte_score_max en pixels (e recuperer)
	text_pos.h = msg_score_surface->h;// Hauteur du texte_score_max en pixels (e recuperer)
	SDL_FreeSurface(msg_score_surface);
	////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////MENU///////////////////
	int swtch = 0;
	int swtch_entree = 0;
	int position_menu_touche = 1;
	int tick = 0;
	int tick_entree = 0;
	//Appuyez sur entree
	char msg_menu[] = "Appuyez sur entrée pour commencer";	//Gestion menu
	SDL_Texture* texte_menu = charger_texte(msg_menu,ecran,fontmenu,color);
	SDL_Rect text_pos_menu; // Position du msg
	text_pos_menu.x = 100;
	text_pos_menu.y = 500;
	text_pos_menu.w = 400;// Largeur du texte_score_max en pixels (a recuperer)
	text_pos_menu.h = 30;// Hauteur du texte_score_max en pixels (a recuperer)
	//Start
	char msg_start[] = "START";	//Gestion menu
	SDL_Texture* texte_start = charger_texte(msg_start,ecran,fontstart,color);
	SDL_Rect text_pos_start; // Position du msg
	text_pos_start.x = 150;
	text_pos_start.y = 200;
	text_pos_start.w = 300;// Largeur du texte_score_max en pixels (a recuperer)
	text_pos_start.h = 100;// Hauteur du texte_score_max en pixels (a recuperer)
	//Options
	char msg_option[] = "OPTIONS";	//Gestion menu
	SDL_Texture* texte_option = charger_texte(msg_option,ecran,fontoption,color);
	SDL_Rect text_pos_option; // Position du msg
	text_pos_option.x = 200;
	text_pos_option.y = 300;
	text_pos_option.w = 200;// Largeur du texte_score_max en pixels (a recuperer)
	text_pos_option.h = 50;// Hauteur du texte_score_max en pixels (a recuperer)
	//Score
	char msg_score_menu[] = "SCORES";	//Gestion menu
	SDL_Texture* texte_menu_score = charger_texte(msg_score_menu,ecran,fontscore,color);
	SDL_Rect text_pos_scores; // Position du msg
	text_pos_scores.x = 200;
	text_pos_scores.y = 350;
	text_pos_scores.w = 200;// Largeur du texte_score_max en pixels (a recuperer)
	text_pos_scores.h = 50;// Hauteur du texte_score_max en pixels (a recuperer)
	/*
	//Retour
	char msg_retour[] = "RETOUR";	//Gestion menu
	SDL_Texture* texte_retour = charger_texte(msg_retour,ecran,fontscore,color);
	SDL_Rect text_pos_retour; // Position du msg
	text_pos_retour.x = 130;
	text_pos_retour.y = 540;
	text_pos_retour.w = 80;// Largeur du texte_score_max en pixels (a recuperer)
	text_pos_retour.h = 30;// Hauteur du texte_score_max en pixels (a recuperer)
	*/
	///////////////////////////////////////////

	/////////////////Message choix difficulta////////////////////
	char msg_diff[] = "Quel niveau de difficulte ? (1, 2 ou 3)";	//Gestion menu
	SDL_Texture* texte_diff = charger_texte(msg_diff,ecran,fontmenu,color);
	SDL_Rect text_pos_diff; // Position du texte_score_max
	text_pos_diff.x = 50;
	text_pos_diff.y = 300;
	text_pos_diff.w = 500;// Largeur du texte_score_max en pixels (e recuperer)
	text_pos_diff.h = 40;// Hauteur du texte_score_max en pixels (e recuperer)
	////////////////////////////////////////////////////////////

	///////////////Opton du sons////////////////////////////////
	char msg_menu_sons[] = "Appuyez sur 'p' pour monter le sons et 'm' pour le diminuer";	//Gestion menu
	SDL_Texture* texte_menu_sons = charger_texte(msg_menu_sons,ecran,fontoption_sons,color);
	SDL_Rect text_pos_menu_sons; // Position du texte_score_max
	text_pos_menu_sons.x = 20;
	text_pos_menu_sons.y = 40;
	text_pos_menu_sons.w = 550;// Largeur du texte_score_max en pixels (e recuperer)
	text_pos_menu_sons.h = 30;// Hauteur du texte_score_max en pixels (e recuperer)
	char msg_menu_sons_delete[] = "Appuyez sur 'd' pour remmettre à zero vos scores.";	//Gestion menu
	SDL_Texture* texte_menu_sons_delete = charger_texte(msg_menu_sons_delete,ecran,fontoption_sons,color);
	SDL_Rect text_pos_menu_sons_delete; // Position du texte_score_max
	text_pos_menu_sons_delete.x = 40;
	text_pos_menu_sons_delete.y = 470;
	text_pos_menu_sons_delete.w = 500;// Largeur du texte_score_max en pixels (e recuperer)
	text_pos_menu_sons_delete.h = 30;// Hauteur du texte_score_max en pixels (e recuperer)
	////////////////////////////////////////////////////////////

	///////////////////////////////TUTO/////////////////////////////////
	char msg_tuto_p1_1[] = "Utilisez les touches directionnels";	//Tuto part1_1
	char msg_tuto_p1_2[] = "droite et gauche afin de positionner";	//Tuto part1_2
	char msg_tuto_p1_3[] = "le cuistot";	//Tuto part1_3
	SDL_Texture* text_msg_tuto_p1_1 = charger_texte(msg_tuto_p1_1,ecran,font_tuto,color);
	SDL_Texture* text_msg_tuto_p1_2 = charger_texte(msg_tuto_p1_2,ecran,font_tuto,color);
	SDL_Texture* text_msg_tuto_p1_3 = charger_texte(msg_tuto_p1_3,ecran,font_tuto,color);
	SDL_Rect pos_tuto_1_1,pos_tuto_1_2,pos_tuto_1_3;
	pos_tuto_1_1.x = 150;pos_tuto_1_2.x = 150;pos_tuto_1_3.x = 250;
	pos_tuto_1_1.y = 150;pos_tuto_1_2.y = 220;pos_tuto_1_3.y = 290;
	pos_tuto_1_1.w = 300;pos_tuto_1_2.w = 300;pos_tuto_1_3.w = 100;
	pos_tuto_1_1.h = 40;pos_tuto_1_2.h = 40;pos_tuto_1_3.h = 40;
	char msg_tuto_p2_1[] = "But : Faire le plus haut score";	//Tuto part1_1
	char msg_tuto_p2_2[] = "(votre score augmente en tuant une patate)";	//Tuto part1_2
	char msg_tuto_p2_3[] = "Appuyez sur R afin de recommencer";	//Tuto part1_3
	SDL_Texture* text_msg_tuto_p2_1 = charger_texte(msg_tuto_p2_1,ecran,font_tuto,color);
	SDL_Texture* text_msg_tuto_p2_2 = charger_texte(msg_tuto_p2_2,ecran,font_tuto,color);
	SDL_Texture* text_msg_tuto_p2_3 = charger_texte(msg_tuto_p2_3,ecran,font_tuto,color);
	SDL_Rect pos_tuto_2_1,pos_tuto_2_2,pos_tuto_2_3;
	pos_tuto_2_1.x = 150;pos_tuto_2_2.x = 130;pos_tuto_2_3.x = 150;
	pos_tuto_2_1.y = 150;pos_tuto_2_2.y = 220;pos_tuto_2_3.y = 320;
	pos_tuto_2_1.w = 300;pos_tuto_2_2.w = 340;pos_tuto_2_3.w = 300;
	pos_tuto_2_1.h = 40;pos_tuto_2_2.h = 40;pos_tuto_2_3.h = 40;
	////////////////////////////////////////////////////////////////////

	//////////////////////////SCOREBOARD/////////////////////////////////

	SDL_Texture* texte_scoreboard1er;
	SDL_Texture* texte_scoreboard2eme;
	SDL_Texture* texte_scoreboard3eme;
	SDL_Texture* texte_scoreboard4eme;
	SDL_Texture* texte_scoreboard5eme;

	SDL_Rect text_pos_scoreboard1,text_pos_scoreboard2,text_pos_scoreboard3,text_pos_scoreboard4,text_pos_scoreboard5; // Position du texte_score_max
	positionscoreboard(&text_pos_scoreboard1);positionscoreboard(&text_pos_scoreboard2);positionscoreboard(&text_pos_scoreboard3);positionscoreboard(&text_pos_scoreboard4);positionscoreboard(&text_pos_scoreboard5);
	text_pos_scoreboard2.y = text_pos_scoreboard2.y + 50;
	text_pos_scoreboard3.y = text_pos_scoreboard3.y + 100;
	text_pos_scoreboard4.y = text_pos_scoreboard4.y + 150;
	text_pos_scoreboard5.y = text_pos_scoreboard5.y + 200;

	/////////////////////////////////////////////////////////////////////

	SDL_Texture* spritecarree = sdcarre; //Texture tampon du du joueur
	SDL_Texture* spritepatate = charger_image_transparente("PATATE/patate0.bmp",ecran,r,g,b); //Texture tampon de la patate

	bool menu = true;
	bool difficulte = false;
	bool option_sons = false;
	bool scoreboard = false;
	bool choixdiff = false;
	bool firstlaunch = true;
	bool tuto = true;
	int time_tuto = 0;
	bool spritedroit = true;
	int tempo_coup = 0;
	bool trigger_patate_dead = false;


	// Boucle principale
	while(!terminer){
		if(menu){
			SDL_RenderClear(ecran);
			SDL_RenderCopy(ecran, fond, NULL, NULL);
			if(firstlaunch){
				nbPatate = 4;
				for(int i=0;i<nbPatate;i++){free(patate[i]);};

				bool sens=true;
				for(int i=1;i<nbPatate+1;i++){
					patate[i-1]=malloc(sizeof(patate_t));
					if(i%2==0){sens=true;}
      				else{sens=false;}
      				creerPatate(patate[i-1],sens,i);
      				spatate[i-1] = charger_image_transparente("PATATE/patate0.bmp",ecran,r,g,b);
				}
      			for(int i=0;i<nbPatate;i++){retourPatateMenu(patate[i]);spatate[i] = spritepatate;};

      			//////////////////////LECTURE DES SCORES////////////////////////////
      			int tab[nbLigne(pFile)];
				tabScore(pFile,tab);

				texte_scoreboard1er = charger_scoreboard(tab,0,ecran,fontmenu,color);
				texte_scoreboard2eme = charger_scoreboard(tab,1,ecran,fontmenu,color);
				texte_scoreboard3eme = charger_scoreboard(tab,2,ecran,fontmenu,color);
				texte_scoreboard4eme = charger_scoreboard(tab,3,ecran,fontmenu,color);
				texte_scoreboard5eme = charger_scoreboard(tab,4,ecran,fontmenu,color);
				////////////////////////////////////////////////////////////////////

      			firstlaunch = false;
			}

			if(position_menu_touche==1){
				clignotement_texte(&texte_start,&tick,msg_start,fontstart,ecran,&swtch);
			}
			else if(position_menu_touche==2){
				clignotement_texte(&texte_option,&tick,msg_option,fontoption,ecran,&swtch);
			}
			else if(position_menu_touche==3){
				clignotement_texte(&texte_menu_score,&tick,msg_score_menu,fontscore,ecran,&swtch);
			}

			SDL_Delay(10);
			for(int i=0;i<nbPatate;i++){

				//GESTION ROTATION IMAGE PATATE
				if (patate[i]->RotationImage == 20*(i+1)){spatate[i] = simagepatate[1];}
				else if (patate[i]->RotationImage == 40*(i+1)){spatate[i] = simagepatate[2];}
				else if (patate[i]->RotationImage == 80*(i+1)){spatate[i] = simagepatate[3];}
				else if (patate[i]->RotationImage == 100*(i+1)){spatate[i] = simagepatate[0];patate[i]->RotationImage=0;}

				SDL_RenderCopy(ecran, spatate[i], NULL, &(patate[i]->DestR));
	    		deplacementPatate(patate[i]);
				if(patate[i]->DestR.x<-50 && patate[i]->droit == true){patate[i]->DestR.x = 600;patate[i]->DestR.y = rand() % 500;patate[i]->RotationImage=0;}
				else if(patate[i]->DestR.x>650 && patate[i]->droit == false){patate[i]->DestR.x = -50;patate[i]->DestR.y = rand() % 500;patate[i]->RotationImage=0;}
			}

			///////Gere un clignotement simple du type affiche/affiche plus////////
			if(tick_entree==100){if(swtch_entree==0){swtch_entree=1;}else{swtch_entree=0;}tick_entree=0;}
			tick_entree++;
			if(swtch_entree==0){SDL_RenderCopy(ecran, texte_menu, NULL, &text_pos_menu);}
			///////////////////////////////////////////////////////////////////////

			SDL_RenderCopy(ecran, texte_start, NULL, &text_pos_start);
			SDL_RenderCopy(ecran, texte_option, NULL, &text_pos_option);
			SDL_RenderCopy(ecran, texte_menu_score, NULL, &text_pos_scores);
			SDL_RenderCopy(ecran,sprite_cuistot_menu,NULL, &DestRSprite_menu);
			SDL_RenderPresent(ecran);
			while( SDL_PollEvent( &evenements ) )
			switch(evenements.type){
				case SDL_QUIT:
				terminer = true; break;

			case SDL_KEYDOWN:
				switch(evenements.key.keysym.sym){
				case SDLK_ESCAPE:case SDLK_q:terminer = true; break;

				case SDLK_RETURN:
				swtch=1;tick=50;
				if(position_menu_touche==1){
					menu = false;
					difficulte = true;
				}
				else if(position_menu_touche==2){
					menu = false;
					option_sons = true;
				}
				else if(position_menu_touche==3){
					menu = false;
					scoreboard = true;
				}
				break;

				case SDLK_DOWN:
				if(position_menu_touche==1){
					remise_en_noir(&texte_start,&tick,msg_start,fontstart,ecran,&swtch);
					position_menu_touche=2;
				}
				else if(position_menu_touche==2){
					remise_en_noir(&texte_option,&tick,msg_option,fontoption,ecran,&swtch);
					position_menu_touche=3;
				};break;

				case SDLK_UP:
				if(position_menu_touche==3){
					remise_en_noir(&texte_menu_score,&tick,msg_score_menu,fontscore,ecran,&swtch);
					position_menu_touche=2;
				}else if(position_menu_touche==2){
					remise_en_noir(&texte_option,&tick,msg_option,fontoption,ecran,&swtch);
					position_menu_touche=1;
				};break;
				}
			}
		}
		//////////////////////////////////// GESTION DIFFICULTE ////////////////////////////////////
		else if(difficulte){
			SDL_RenderClear(ecran);
			SDL_RenderCopy(ecran, fond, NULL, NULL);
			SDL_RenderCopy(ecran, texte_diff, NULL, &text_pos_diff);

			SDL_RenderCopy(ecran, back,NULL,&DestBack);

			SDL_RenderPresent(ecran);
			while( SDL_PollEvent( &evenements ) )
			switch(evenements.type){
				case SDL_QUIT:
				terminer = true; break;

			case SDL_KEYDOWN:
				switch(evenements.key.keysym.sym){
				case SDLK_ESCAPE:case SDLK_q:terminer = true; break;
				case SDLK_1:case SDLK_KP_1: choixdiff = true;nbPatate = 2;break;
				case SDLK_2:case SDLK_KP_2: choixdiff = true;nbPatate = 3;break;
				case SDLK_3:case SDLK_KP_3: choixdiff = true;nbPatate = 4;break;
				case SDLK_BACKSPACE:menu = true;difficulte=false;break;
				}
			}

			if(choixdiff){
				for(int i=0;i<nbPatate;i++){free(patate[i]);};
				bool sens=true;
				for(int i=1;i<nbPatate+1;i++){
					patate[i-1]=malloc(sizeof(patate_t));
					if(i%2==0){sens=true;}
      				else{sens=false;}
      				creerPatate(patate[i-1],sens,i);
      				spatate[i-1] = charger_image_transparente("PATATE/patate0.bmp",ecran,r,g,b);
				}
      			for(int i=0;i<nbPatate;i++){retourPatate(patate[i]);spatate[i] = spritepatate;};
      			difficulte = false;
      			choixdiff = false;
      			tuto=true;
      			trigger_patate_dead=false;
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////
		else if(option_sons){
			SDL_RenderClear(ecran);
			SDL_RenderCopy(ecran, fond, NULL, NULL);
			SDL_RenderCopy(ecran, texte_menu_sons, NULL, &text_pos_menu_sons);

			SDL_RenderCopy(ecran, back,NULL,&DestBack);

			if (audio == 0){SDL_RenderCopy(ecran, snosound, NULL, &DestRs);}
			else if(audio > 0 && audio <= 5){SDL_RenderCopy(ecran, ssound1, NULL, &DestRs);}
			else if(audio > 5 && audio <= 10){SDL_RenderCopy(ecran, ssound2, NULL, &DestRs);}
			else{SDL_RenderCopy(ecran, ssound3, NULL, &DestRs);}

			SDL_RenderCopy(ecran, texte_menu_sons_delete, NULL, &text_pos_menu_sons_delete);

			SDL_RenderPresent(ecran);

			while( SDL_PollEvent( &evenements ) )
			switch(evenements.type){
			case SDL_QUIT:terminer = true; break;
			case SDL_KEYDOWN:
				switch(evenements.key.keysym.sym){
				case SDLK_ESCAPE:case SDLK_q:terminer = true; break;
				case SDLK_p: audio++;pauseAudio();SDL_Delay(10);playMusic("road.wav", audio);SDL_Delay(10);unpauseAudio();break;
				case SDLK_m: if(audio!=0){audio--;pauseAudio();SDL_Delay(10);playMusic("road.wav", audio);SDL_Delay(10);unpauseAudio();}break;
				case SDLK_BACKSPACE:menu = true;option_sons=false;break;
				case SDLK_d:resetScore(pFile);meilleur_score_fichier = 0;menu = true;option_sons=false;firstlaunch=true;break;
				}
			}
		}
		//////////////////////////// GESTION SCOREBOARD ///////////////////////////////////////
		else if(scoreboard){
			SDL_RenderClear(ecran);
			SDL_RenderCopy(ecran, fond, NULL, NULL);
			SDL_RenderCopy(ecran,spatate_fond,NULL,NULL);		
			SDL_RenderCopy(ecran, texte_scoreboard1er, NULL, &text_pos_scoreboard1);
			SDL_RenderCopy(ecran, texte_scoreboard2eme, NULL, &text_pos_scoreboard2);
			SDL_RenderCopy(ecran, texte_scoreboard3eme, NULL, &text_pos_scoreboard3);
			SDL_RenderCopy(ecran, texte_scoreboard4eme, NULL, &text_pos_scoreboard4);
			SDL_RenderCopy(ecran, texte_scoreboard5eme, NULL, &text_pos_scoreboard5);
			SDL_RenderCopy(ecran, back,NULL,&DestBack);
			SDL_RenderPresent(ecran);
			while( SDL_PollEvent( &evenements ) )
			switch(evenements.type){
			case SDL_QUIT:terminer = true; break;
			case SDL_KEYDOWN:
				switch(evenements.key.keysym.sym){
				case SDLK_ESCAPE:case SDLK_q:terminer = true; break;
				case SDLK_BACKSPACE:menu = true;scoreboard=false;break;
				}
			}
		}
		/////////////////////////////////////////////////////////////////////////////////
		else{
			if(tuto==false){
				//Affichage
				SDL_RenderClear(ecran);
				SDL_RenderCopy(ecran, fond_ingame, NULL, NULL);
		    	SDL_RenderCopy(ecran, spritecarree, NULL, &DestRc);

		    	for(int i=0;i<nbPatate;i++){SDL_RenderCopy(ecran, spatate[i], NULL, &(patate[i]->DestR));}
		    	SDL_RenderCopy(ecran, texte_score_max, NULL, &text_posm);
		    	SDL_RenderCopy(ecran, texte_score, NULL, &text_pos);
		    	for(int i = 0;i!=joueur->vie;i++){
		    		DestRv.x = DestRv.x + DestRv.w + 10;
		    		SDL_RenderCopy(ecran, svie, NULL, &DestRv);
		    	}
		    	DestRv.x = 400;

		    	//SDL_RenderCopy(ecran, sprites, spriterectsrc, spriterectdest);
		    	if(trigger_patate_dead){
		    		tempo_coup++;
		    		if(spritedroit){SDL_RenderCopy(ecran,sprite_cuistot_droit_bas,NULL,&Position_cuistot_d_b);}
		    		else{SDL_RenderCopy(ecran,sprite_cuistot_gauche_bas,NULL,&Position_cuistot_g_b);}
		    		if(tempo_coup >= 40){trigger_patate_dead=false;tempo_coup=0;}
		    	}
		    	else{
		    		tempo_coup=0;
		    		if(spritedroit){SDL_RenderCopy(ecran,sprite_cuistot_droit_haut,NULL,&Position_cuistot_d_h);}
		    		else{SDL_RenderCopy(ecran,sprite_cuistot_gauche_haut,NULL,&Position_cuistot_g_h);}
		    	}
				SDL_RenderPresent(ecran);

				while( SDL_PollEvent( &evenements ) )
				switch(evenements.type){
					case SDL_QUIT:
					terminer = true; break;

				case SDL_KEYDOWN:
					switch(evenements.key.keysym.sym){
					case SDLK_ESCAPE:case SDLK_q:terminer = true; break;
					case SDLK_LEFT: spritecarree = sgcarre;joueur->gauche=1;joueur->droit=0;spritedroit=false;trigger_patate_dead=false;break;
					case SDLK_RIGHT: spritecarree = sdcarre;joueur->gauche=0;joueur->droit=1;spritedroit=true;trigger_patate_dead=false;break;
					//Initialisation apres appui sur bouton delete 'd' (supprime tout les score et reset) A TRANSFORMER EN FONCTION
					//ICI ancien case d
					//Initialisation apres appui sur bouton reset 'r' (reset du score actuel) A TRANSFORMER EN FONCTION
					case SDLK_r:score=0;texte_score = charger_texte_score_actu(score,ecran,font,color);best_score=0;meilleur_score_fichier = lireHighScore(pFile);for(int i=0;i<nbPatate;i++){spatate[i] = spritepatate;retourPatate(patate[i]);joueur->vie=3;}break;
					}
				}
				SDL_Delay(10);
				for(int i=0;i<nbPatate;i++){
					deplacementPatate(patate[i]);
					if(patate[i]->DestR.x<-50 && patate[i]->droit == true){spatate[i] = spritepatate;retourPatate(patate[i]);} //Retour de la patate venant de droite apres avoir quitte l'ecran
					else if(patate[i]->DestR.x>650 && patate[i]->droit == false){spatate[i] = spritepatate;retourPatate(patate[i]);} //Retour de la patate venant de droite apres avoir quitte l'ecran


					if (patate[i]->DestR.x >= 600-3 && patate[i]->DestR.x <= 600+3 && patate[i]->droit == true){//Gestion score plus mort ou vie de la patate
						if(patate[i]->cri_arrive == false){playSound("Arrive.wav", audio+90);patate[i]->cri_arrive = true;}
					}
					else if (patate[i]->DestR.x >= -53 && patate[i]->DestR.x <= -47 && patate[i]->droit == false){//Gestion score plus mort ou vie de la patate
						if(patate[i]->cri_arrive == false){playSound("Arrive.wav", audio+90);patate[i]->cri_arrive = true;}
					}

					if (patate[i]->DestR.x >= posx_patate_attack_gauche-5 && patate[i]->DestR.x <= posx_patate_attack_gauche && patate[i]->droit == true){//Gestion score plus mort ou vie de la patate
						if(patate[i]->patate_interval == false){
							if(patate[i]->DestR.y > posy_patate_attack_haut && patate[i]->DestR.y < posy_patate_attack_bas){
								if (joueur->droit==1){spatate[i] = spatate_ko;score++;texte_score = charger_texte_score_actu(score,ecran,font,color);playSound("Mort.wav",audio+90);patate[i]->vie=false;trigger_patate_dead=true;}
								if (joueur->gauche==1){spatate[i] = spatate_alive;joueur->vie--;}
							}
							patate[i]->patate_interval = true;
						}
					}
					else if (patate[i]->DestR.x >= posx_patate_attack_droite-patate[i]->DestR.w && patate[i]->DestR.x <= posx_patate_attack_droite-patate[i]->DestR.w+5 && patate[i]->droit == false){//Gestion score plus mort ou vie de la patate
						if(patate[i]->patate_interval == false){	
							if(patate[i]->DestR.y > posy_patate_attack_haut && patate[i]->DestR.y < posy_patate_attack_bas){
								if (joueur->gauche==1){spatate[i] = spatate_ko;score++;texte_score = charger_texte_score_actu(score,ecran,font,color);playSound("Mort.wav",audio+90);patate[i]->vie=false;trigger_patate_dead=true;}
								if (joueur->droit==1){spatate[i] = spatate_alive;joueur->vie--;}
							}
							patate[i]->patate_interval = true;
						}
					}
				}

				if (joueur->vie <= 0){
					texte_score_max = charger_texte_score(score,ecran,font,color);
					ecrireScore(score,pFile);score=0;texte_score = charger_texte_score_actu(score,ecran,font,color);
					best_score=0;meilleur_score_fichier = lireHighScore(pFile);joueur->vie=3;menu = true;firstlaunch=true;
				}


				if (best_score == 0){
					if (meilleur_score_fichier == score){best_score = 1;tick_color_red = 0;}}//Verifie si le score actuel est superieur au score max
				else{
					if (meilleur_score_fichier == 0){tick_color_red = -1;}
					if (tick_color_red >= 0){tick_color_red++;}
					if (tick_color_red < 100 && tick_color_red >= 0){texte_score_max = charger_texte_score(score,ecran,font,color_red);}
					else{
						tick_color_red = -1;
						texte_score_max = charger_texte_score(score,ecran,font,color);
					}
				}
			}
			else{
				SDL_RenderClear(ecran);
				SDL_RenderCopy(ecran, fond_ingame, NULL, NULL);
				time_tuto++;

				if(time_tuto<=20000){
					SDL_RenderCopy(ecran,text_msg_tuto_p1_1, NULL,&pos_tuto_1_1);
					SDL_RenderCopy(ecran,text_msg_tuto_p1_2, NULL,&pos_tuto_1_2);
					SDL_RenderCopy(ecran,text_msg_tuto_p1_3, NULL,&pos_tuto_1_3);
				}

				if (time_tuto > 22000){
					SDL_RenderCopy(ecran,text_msg_tuto_p2_1, NULL,&pos_tuto_2_1);
					SDL_RenderCopy(ecran,text_msg_tuto_p2_2, NULL,&pos_tuto_2_2);
					SDL_RenderCopy(ecran,text_msg_tuto_p2_3, NULL,&pos_tuto_2_3);
					if(time_tuto==40000){tuto=false;time_tuto=0;}
				}
				SDL_RenderPresent(ecran);

				while( SDL_PollEvent( &evenements ) )
				switch(evenements.type){
					case SDL_QUIT:
					terminer = true; break;

				case SDL_KEYDOWN:
					switch(evenements.key.keysym.sym){
					case SDLK_ESCAPE:case SDLK_q:terminer = true; break;
					}
				}
			}
		}
	}

	//Termine tous les sons/Musiques actuel
	endAudio();

	// Quitter et nettoyer SDL
	SDL_DestroyTexture(fond);
	SDL_DestroyTexture(spritecarree);SDL_DestroyTexture(sdcarre);SDL_DestroyTexture(sgcarre);
	for(int i=0;i<nbPatate;i++){SDL_DestroyTexture(spatate[i]);free(patate[i]);}
	SDL_DestroyTexture(spritepatate);SDL_DestroyTexture(spatate_ko);SDL_DestroyTexture(spatate_alive);
	SDL_DestroyTexture(svie);
	SDL_DestroyTexture(texte_score_max);SDL_DestroyTexture(texte_score);SDL_DestroyTexture(texte_menu_sons);SDL_DestroyTexture(texte_menu);
	SDL_DestroyTexture(ssound1);SDL_DestroyTexture(ssound2);SDL_DestroyTexture(ssound3);SDL_DestroyTexture(snosound);
	SDL_DestroyRenderer(ecran);
	SDL_DestroyWindow(fenetre);
	TTF_CloseFont(font);TTF_CloseFont(fontmenu);TTF_CloseFont(fontoption);TTF_CloseFont(fontscore);TTF_CloseFont(fontstart);
	TTF_Quit();
	SDL_Quit();
	return 0;
	}
