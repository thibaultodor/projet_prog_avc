#include <stdio.h>
#include <stdlib.h>
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

int main(int argc, char *argv[])
{
    //<---TEST DES FONCTION POUR GESTION SCORE
    /*
    FILE* fichier = NULL;
    int terminer =0;
    int score= 0;

    //TestEcriture
    while(!terminer){
        printf("Entrer le score : ");
        scanf("%d",&score);
        ecrireScore(score,fichier);
        printf("D'autre score a ajouter ? (0 oui 1 non)");
        scanf("%d",&terminer);
    }

    //Test lecture HigHScore
    int highScore = lireHighScore(fichier);
    printf("Le score max est : %d\n",highScore);

    //Test reset score
    resetScore(fichier);*/

    return 0;
}