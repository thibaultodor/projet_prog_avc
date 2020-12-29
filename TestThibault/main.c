#include <stdbool.h>
#include "fonctions_score.h"
#include "fonctions_jeu.h"
#include "fonctions_graphique.h"
#include "audio.h"

int main()	{
	SDL_Window* fenetre; // D?claration de la fen?tre
	SDL_Event evenements; // ?v?nements li?s ? la fen?tre
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

	// Cr?er la fen?tre
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

	//Ouverture fichier score
	FILE * pFile;
	pFile=fopen ("score.txt","r");

	//Initialisation renderer
	SDL_Renderer *ecran = SDL_CreateRenderer(fenetre, -1, 0);

	//Image fond
	SDL_Texture* fond = charger_image("fond_cuisine.bmp",ecran);

    //Image sol
	SDL_Texture* sol = charger_image("fond.bmp",ecran);
	SDL_Surface * surface_obj = SDL_LoadBMP("fond.bmp");
	SDL_FreeSurface(surface_obj);
	int tailleW = surface_obj->w;
	int tailleH = surface_obj->h;
	SDL_Rect SrcR,DestR;
	//positionImageSol(&SrcR,&DestR,tailleW,tailleH);

	//Image sprite carr? (joueur)
	SDL_Texture* sdcarre = charger_image("cuisto_droite.bmp",ecran);
	SDL_Texture* sgcarre = charger_image("cuisto_gauche.bmp",ecran);
	SDL_Rect DestRc;
	positionImageCarre(&DestRc);

	//Sauvegarde des positions du sprite joueur patate venant de gauche
	int posx_patate_attack_gauche = DestRc.x + DestRc.w;
	//Sauvegarde des positions du sprite joueur patate venant de droite
	int posx_patate_attack_droite = 275;

	//Permet de g?rer la hitbox en y du sprite du joueur
	int posy_patate_attack_bas = DestRc.y + DestRc.h;
	int posy_patate_attack_haut = DestRc.y;

	Uint8 r = 0, g = 255, b = 255;

	//Image sprite patate
	SDL_Texture* spatate_alive = charger_image("sprite_patate_nodegat.bmp",ecran);
	SDL_Texture* spatate_ko = charger_image_transparente("sprite_patate_degat.bmp",ecran,r,g,b);
	//D?claration patate plus positionnement
	SDL_Texture *simagepatate[4];
	simagepatate[0] = charger_image_transparente("PATATE/patate0.bmp",ecran,r,g,b);
	simagepatate[1] = charger_image_transparente("PATATE/patate1.bmp",ecran,r,g,b);
	simagepatate[2] = charger_image_transparente("PATATE/patate2.bmp",ecran,r,g,b);
	simagepatate[3] = charger_image_transparente("PATATE/patate3.bmp",ecran,r,g,b);


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


	//Gestion affichage texte_score_max pour score
	TTF_Init(); 
	TTF_Font *font = TTF_OpenFont("./arial.ttf",25);
	TTF_Font *fontmenu = TTF_OpenFont("./arial.ttf",80);
	TTF_Font *fontstart = TTF_OpenFont("./arial.ttf",60);
	TTF_Font *fontoption = TTF_OpenFont("./arial.ttf",60);
	TTF_Font *fontscore = TTF_OpenFont("./arial.ttf",60);
	SDL_Color color = {0,0,0,0};
	SDL_Color color_red = {250,0,0,0};
	int tick_color_red = 0;

	int meilleur_score_fichier = lireHighScore(pFile);

	char msg_score_max[] = "Score max : xx";	//Gestion score max
	SDL_Texture* texte_score_max = charger_texte_score(meilleur_score_fichier,ecran,font,color);
	SDL_Surface * msg_score_max_surface = TTF_RenderText_Solid(font,msg_score_max,color);
	SDL_Rect text_posm; // Position du texte_score_max
	text_posm.x = 5;
	text_posm.y = 10;
	text_posm.w = msg_score_max_surface->w;// Largeur du texte_score_max en pixels (? r?cup?rer)
	text_posm.h = msg_score_max_surface->h;// Hauteur du texte_score_max en pixels (? r?cup?rer)
	SDL_FreeSurface(msg_score_max_surface);

	char msg_score[] = "Score : xx";	//Gestion score actuel
	SDL_Texture* texte_score = charger_texte_score_actu(0,ecran,font,color);
	SDL_Surface * msg_score_surface = TTF_RenderText_Solid(font,msg_score,color);
	SDL_Rect text_pos; // Position du texte_score_max
	text_pos.x = 5;
	text_pos.y = 40;
	text_pos.w = msg_score_surface->w;// Largeur du texte_score_max en pixels (? r?cup?rer)
	text_pos.h = msg_score_surface->h;// Hauteur du texte_score_max en pixels (? r?cup?rer)
	SDL_FreeSurface(msg_score_surface);


	////////////////////MENU///////////////////
	int swtch = 0;
	int position_menu_touche = 1;
	int tick = 0;
	//Appuyez sur entree
	char msg_menu[] = "Appuyez sur entr�e pour commencer";	//Gestion menu
	SDL_Texture* texte_menu = charger_texte(msg_menu,ecran,fontmenu,color);
	SDL_Surface * msg_menu_surface = TTF_RenderText_Solid(fontmenu,msg_menu,color);
	SDL_Rect text_pos_menu; // Position du msg
	text_pos_menu.x = 100;
	text_pos_menu.y = 250;
	text_pos_menu.w = 400;// Largeur du texte_score_max en pixels (� r?cup?rer)
	text_pos_menu.h = 50;// Hauteur du texte_score_max en pixels (� r?cup?rer)
	SDL_FreeSurface(msg_menu_surface);
	//Start
	char msg_start[] = "START";	//Gestion menu
	SDL_Texture* texte_start = charger_texte(msg_start,ecran,fontstart,color);
	SDL_Surface * msg_menu_start = TTF_RenderText_Solid(fontstart,msg_start,color);
	SDL_Rect text_pos_start; // Position du msg
	text_pos_start.x = 100;
	text_pos_start.y = 350;
	text_pos_start.w = 200;// Largeur du texte_score_max en pixels (� r?cup?rer)
	text_pos_start.h = 50;// Hauteur du texte_score_max en pixels (� r?cup?rer)
	SDL_FreeSurface(msg_menu_start);
	//Options
	char msg_option[] = "OPTIONS";	//Gestion menu
	SDL_Texture* texte_option = charger_texte(msg_option,ecran,fontoption,color);
	SDL_Surface * msg_menu_option = TTF_RenderText_Solid(fontoption,msg_option,color);
	SDL_Rect text_pos_option; // Position du msg
	text_pos_option.x = 100;
	text_pos_option.y = 400;
	text_pos_option.w = 200;// Largeur du texte_score_max en pixels (� r?cup?rer)
	text_pos_option.h = 50;// Hauteur du texte_score_max en pixels (� r?cup?rer)
	SDL_FreeSurface(msg_menu_option);
	//Score
	char msg_score_menu[] = "SCORES";	//Gestion menu
	SDL_Texture* texte_menu_score = charger_texte(msg_score_menu,ecran,fontscore,color);
	SDL_Surface * msg_menu_score = TTF_RenderText_Solid(fontscore,msg_score_menu,color);
	SDL_Rect text_pos_scores; // Position du msg
	text_pos_scores.x = 100;
	text_pos_scores.y = 450;
	text_pos_scores.w = 200;// Largeur du texte_score_max en pixels (� r?cup?rer)
	text_pos_scores.h = 50;// Hauteur du texte_score_max en pixels (� r?cup?rer)
	SDL_FreeSurface(msg_menu_score);
	///////////////////////////////////////////

	char msg_diff[] = "Quel niveau de difficulte ? (f , m ou d)";	//Gestion menu
	SDL_Texture* texte_diff = charger_texte(msg_diff,ecran,fontmenu,color);
	SDL_Surface * msg_menu_diff = TTF_RenderText_Solid(fontmenu,msg_diff,color);
	SDL_Rect text_pos_diff; // Position du texte_score_max
	text_pos_diff.x = 100;
	text_pos_diff.y = 300;
	text_pos_diff.w = 400;// Largeur du texte_score_max en pixels (? r?cup?rer)
	text_pos_diff.h = 50;// Hauteur du texte_score_max en pixels (? r?cup?rer)
	SDL_FreeSurface(msg_menu_diff);

	char msg_menu_sons[] = "Appuyez sur 'p' pour monter le sons et 'm' pour le diminuer";	//Gestion menu
	SDL_Texture* texte_menu_sons = charger_texte(msg_menu_sons,ecran,font,color);
	SDL_Surface * msg_menu_sons_surface = TTF_RenderText_Solid(font,msg_menu_sons,color);
	SDL_Rect text_pos_menu_sons; // Position du texte_score_max
	text_pos_menu_sons.x = 5;
	text_pos_menu_sons.y = 40;
	text_pos_menu_sons.w = 400;// Largeur du texte_score_max en pixels (? r?cup?rer)
	text_pos_menu_sons.h = msg_menu_sons_surface->h;// Hauteur du texte_score_max en pixels (? r?cup?rer)
	SDL_FreeSurface(msg_menu_sons_surface);


	SDL_Texture* spritecarree = sdcarre; //Texture tampon du du joueur
	SDL_Texture* spritepatate = charger_image_transparente("PATATE/patate0.bmp",ecran,r,g,b); //Texture tampon de la patate

	bool menu = true;
	bool difficulte = false;
	bool option_sons = false;
	bool choixdiff = false;
	bool firstlaunch = true;


	// Boucle principale
	while(!terminer){
		if(menu){
			SDL_RenderClear(ecran);
			SDL_RenderCopy(ecran, fond, NULL, NULL);
			SDL_RenderCopy(ecran, texte_menu, NULL, &text_pos_menu);
			SDL_RenderCopy(ecran, texte_start, NULL, &text_pos_start);
			SDL_RenderCopy(ecran, texte_option, NULL, &text_pos_option);
			SDL_RenderCopy(ecran, texte_menu_score, NULL, &text_pos_scores);

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
      			for(int i=0;i<nbPatate;i++){retourPatate(patate[i]);patate[i]->DestR.y = 600-200;spatate[i] = spritepatate;};
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
		else if(difficulte){
			SDL_RenderClear(ecran);
			SDL_RenderCopy(ecran, fond, NULL, NULL);
			SDL_RenderCopy(ecran, texte_diff, NULL, &text_pos_diff);
			SDL_RenderPresent(ecran);
			while( SDL_PollEvent( &evenements ) )
			switch(evenements.type){
				case SDL_QUIT:
				terminer = true; break;

			case SDL_KEYDOWN:
				switch(evenements.key.keysym.sym){
				case SDLK_ESCAPE:case SDLK_q:terminer = true; break;
				case SDLK_f: choixdiff = true;nbPatate = 2;break;
				case SDLK_m: choixdiff = true;nbPatate = 3;break;
				case SDLK_d: choixdiff = true;nbPatate = 4;break;
				case SDLK_BACKSPACE:menu = true;difficulte=false;break;
				}
			}

			if(choixdiff){
				for(int i=0;i<nbPatate;i++){free(patate[i]);};

				if (audio == 0){SDL_RenderCopy(ecran, snosound, NULL, &DestRs);}
				else if(audio > 0 && audio <= 5){SDL_RenderCopy(ecran, ssound1, NULL, &DestRs);}
				else if(audio > 5 && audio <= 10){SDL_RenderCopy(ecran, ssound2, NULL, &DestRs);}
				else{SDL_RenderCopy(ecran, ssound3, NULL, &DestRs);}
				SDL_RenderPresent(ecran);

				bool sens=true;
				for(int i=1;i<nbPatate+1;i++){
					patate[i-1]=malloc(sizeof(patate_t));
					if(i%2==0){sens=true;}
      				else{sens=false;}
      				creerPatate(patate[i-1],sens,i);
      				spatate[i-1] = charger_image_transparente("PATATE/patate0.bmp",ecran,r,g,b);
				}
      			for(int i=0;i<nbPatate;i++){retourPatate(patate[i]);patate[i]->DestR.y = 600-200;spatate[i] = spritepatate;};
      			difficulte = false;
      			choixdiff = false;
			}
		}
		else if(option_sons){
			SDL_RenderClear(ecran);
			SDL_RenderCopy(ecran, fond, NULL, NULL);
			SDL_RenderCopy(ecran, texte_menu_sons, NULL, &text_pos_menu_sons);

			if (audio == 0){SDL_RenderCopy(ecran, snosound, NULL, &DestRs);}
			else if(audio > 0 && audio <= 5){SDL_RenderCopy(ecran, ssound1, NULL, &DestRs);}
			else if(audio > 5 && audio <= 10){SDL_RenderCopy(ecran, ssound2, NULL, &DestRs);}
			else{SDL_RenderCopy(ecran, ssound3, NULL, &DestRs);}

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
				}
			}
		}
		else{
			//Affichage
			SDL_RenderClear(ecran);
			SDL_RenderCopy(ecran, fond, NULL, NULL);
	    	SDL_RenderCopy(ecran, sol, &SrcR, &DestR);
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
			SDL_RenderPresent(ecran);

			while( SDL_PollEvent( &evenements ) )
			switch(evenements.type){
				case SDL_QUIT:
				terminer = true; break;

			case SDL_KEYDOWN:
				switch(evenements.key.keysym.sym){
				case SDLK_ESCAPE:case SDLK_q:terminer = true; break;
				case SDLK_LEFT: spritecarree = sgcarre;joueur->gauche=1;joueur->droit=0;break;
				case SDLK_RIGHT: spritecarree = sdcarre;joueur->gauche=0;joueur->droit=1;break;
				//Initialisation apr?s appui sur bouton delete 'd' (supprime tout les score et reset) A TRANSFORMER EN FONCTION
				case SDLK_d:resetScore(pFile);texte_score_max = charger_texte_score(0,ecran,font,color);best_score = 1;score=0;texte_score = charger_texte_score_actu(score,ecran,font,color);meilleur_score_fichier = 0;tick_color_red = -1;break;
				//Initialisation apr?s appui sur bouton reset 'r' (reset du score actuel) A TRANSFORMER EN FONCTION
				case SDLK_r:ecrireScore(score,pFile);score=0;texte_score = charger_texte_score_actu(score,ecran,font,color);best_score=0;meilleur_score_fichier = lireHighScore(pFile);break;
				}
			}
			SDL_Delay(10);
			for(int i=0;i<nbPatate;i++){
				deplacementPatate(patate[i]);
				if(patate[i]->DestR.x<-50 && patate[i]->droit == true){spatate[i] = spritepatate;retourPatate(patate[i]);} //Retour de la patate venant de droite apr?s avoir quitt? l'?cran
				else if(patate[i]->DestR.x>650 && patate[i]->droit == false){spatate[i] = spritepatate;retourPatate(patate[i]);} //Retour de la patate venant de droite apr?s avoir quitt? l'?cran


				if (patate[i]->DestR.x >= 600-3 && patate[i]->DestR.x <= 600+3 && patate[i]->droit == true){//Gestion score plus mort ou vie de la patate
					if(patate[i]->cri_arrive == false){playSound("Arrive.wav", audio+90);patate[i]->cri_arrive = true;}
				}
				else if (patate[i]->DestR.x >= -53 && patate[i]->DestR.x <= -47 && patate[i]->droit == false){//Gestion score plus mort ou vie de la patate
					if(patate[i]->cri_arrive == false){playSound("Arrive.wav", audio+90);patate[i]->cri_arrive = true;}
				}

				if (patate[i]->DestR.x >= posx_patate_attack_gauche-5 && patate[i]->DestR.x <= posx_patate_attack_gauche && patate[i]->droit == true){//Gestion score plus mort ou vie de la patate
					if(patate[i]->patate_interval == false){
						if(patate[i]->DestR.y > posy_patate_attack_haut && patate[i]->DestR.y < posy_patate_attack_bas){
							if (joueur->droit==1){spatate[i] = spatate_ko;score++;texte_score = charger_texte_score_actu(score,ecran,font,color);playSound("Mort.wav",audio+90);}
							if (joueur->gauche==1){spatate[i] = spatate_alive;joueur->vie--;}
						}
						patate[i]->patate_interval = true;
					}
				}
				else if (patate[i]->DestR.x >= posx_patate_attack_droite-patate[i]->DestR.w && patate[i]->DestR.x <= posx_patate_attack_droite-patate[i]->DestR.w+5 && patate[i]->droit == false){//Gestion score plus mort ou vie de la patate
					if(patate[i]->patate_interval == false){	
						if(patate[i]->DestR.y > posy_patate_attack_haut && patate[i]->DestR.y < posy_patate_attack_bas){
							if (joueur->gauche==1){spatate[i] = spatate_ko;score++;texte_score = charger_texte_score_actu(score,ecran,font,color);playSound("Mort.wav",audio+90);}
							if (joueur->droit==1){spatate[i] = spatate_alive;joueur->vie--;}
						}
						patate[i]->patate_interval = true;
					}
				}
			}

			if (joueur->vie <= 0){
				ecrireScore(score,pFile);score=0;texte_score = charger_texte_score_actu(score,ecran,font,color);
				best_score=0;meilleur_score_fichier = lireHighScore(pFile);joueur->vie=3;menu = true;firstlaunch=true;
				for(int i=0;i<nbPatate;i++){spatate[i] = spritepatate;retourPatate(patate[i]);patate[i]->DestR.y = rand() % 500;}
			}


			if (best_score == 0){
				if (meilleur_score_fichier == score){best_score = 1;tick_color_red = 0;}}//Verifie si le score actuel est sup?rieur au score max
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
	}
	ecrireScore(score,pFile);

	//Termine tous les sons/Musiques actuel
	endAudio();

	// Quitter et nettoyer SDL
	SDL_DestroyTexture(fond);SDL_DestroyTexture(sol);
	SDL_DestroyTexture(spritecarree);SDL_DestroyTexture(sdcarre);SDL_DestroyTexture(sgcarre);
	for(int i=0;i<nbPatate;i++){SDL_DestroyTexture(spatate[i]);}
	SDL_DestroyTexture(spritepatate);SDL_DestroyTexture(spatate_ko);SDL_DestroyTexture(spatate_alive);
	SDL_DestroyTexture(svie);
	SDL_DestroyTexture(texte_score_max);SDL_DestroyTexture(texte_score);SDL_DestroyTexture(texte_menu_sons);SDL_DestroyTexture(texte_menu);
	SDL_DestroyTexture(ssound1);SDL_DestroyTexture(ssound2);SDL_DestroyTexture(ssound3);SDL_DestroyTexture(snosound);
	//SDL_DestroyTexture(sprites);
	SDL_DestroyRenderer(ecran);
	SDL_DestroyWindow(fenetre);
	TTF_CloseFont(font);
	TTF_CloseFont(fontmenu);
	TTF_Quit();
	SDL_Quit();
	return 0;
	}
