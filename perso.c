#include "perso.h"

void perteVie(perso p){
    if(p.vie>0){
        p.vie--;
    }
}

void addPoint(perso p){
    p.score++;
}

void nomJoueur(perso p){

    char nom[3];
    printf("Entrez votre nom a 3 caractère\n");
    for(int i=0;i<3;i++){
        char a;
        scanf("%c"&a);
        nom = nom + a;
    }
    p.pseudo=nom;   
}