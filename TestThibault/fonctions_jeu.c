#include "fonctions_jeu.h"

void creerPatate(patate_t *patate,int droit, int gauche,int type){
    patate->vie=true;
    patate->droit=droit;
    patate->gauche=gauche;
    patate->type=type;
}