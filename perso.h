#include <stdlib.h>
#include <stdio.h>
#define NB_VIE 3;

/**
 * \brief Fichier Header pour perso.h
 */

/**
 * \struct Personnage
 */
struct Perso{
    char *pseudo;/*!< Stockage nom joueur.*/
    int vie;/*!< Stockage nombre de vie du joueur*/
    int score;/*!< Stockage score du joueur*/
};
typedef struct Perso perso;

/**
 * \brief Gestion perte de point de vie
 * \param perso Personnage
 */
void perteVie(perso p);

/**
 * \brief Gestion adjout de point au score
 * \param perso Personnage
 */
void addPoint(perso p);

/**
 * \brief Récupère le nom du joueur pour le stocker
 * \param perso Personnage
 */
void nomJoueur(perso p);