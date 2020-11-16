#include <stdio.h>
#include <stdlib.h>
#define TAILLE_MAX 1000



void ecrireScore(int score, FILE* fichier){
    fichier=fopen("score.txt","a");
    if(fichier!=NULL){
        fprintf(fichier,"Voici ton score nulosse %d\n",score);
        fclose(fichier);
    }
}
int main(int argc, char *argv[])
{
    FILE* fichier = NULL;
    int terminer =0;
    int score= 0;

    while(!terminer){
        printf("Entrer le score : ");
        scanf("%d",&score);
        ecrireScore(score,fichier);
        printf("D'autre score a ajouter ? (0 oui 1 non)");
        scanf("%d",&terminer);
    }
    return 0;
}