#include "fonctions_score.h"
#define TAILLE_MAX 10

void ecrireScore(int score, FILE* fichier){
    fichier=fopen("score.txt","a");
    if(fichier!=NULL){
        fprintf(fichier,"%d\n",score);
        fclose(fichier);
    }
}

int lireHighScore(FILE* fichier){
    int max=0;
    char chaine [TAILLE_MAX];//Création chaine vide pour récupérer score
    fichier =fopen("score.txt","r");
    if(fichier!=NULL){
        while(fgets(chaine,TAILLE_MAX,fichier)){
            if(max<atoi(chaine)){
                max=atoi(chaine);
            }
        }
    }
    return max;
}

void resetScore(FILE* fichier){
    fichier=fopen("score.txt","w");
    if(fichier!=NULL){
        fputs("",fichier);
    }
}