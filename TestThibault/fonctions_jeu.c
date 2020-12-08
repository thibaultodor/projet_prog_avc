#include "fonctions_jeu.h"

void creerPatate(patate_t *patate,bool sens,int vitesse){
    patate->vie=true;
    patate->droit=sens;
    patate->type=0;
    patate->vitesse=vitesse;
    patate->patate_interval=false;
    if(sens == true){patate->DestR.x = 600-50;}
    else{patate->DestR.x = -50;}
	patate->DestR.y = rand() % 500;
	patate->DestR.w = 50;
	patate->DestR.h = 50;
    patate->RotationImage = 0;
}