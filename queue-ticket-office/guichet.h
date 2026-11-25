#ifndef GUICHET_H
#define GUICHET_H

#include "file.h"

#define MAX_NOM 50

typedef struct {
    int anneeNaissance;
    char nom[MAX_NOM];
    int tourArrivee;
} t_personne;

typedef struct {
    int numero;
    int ouvert;
    t_file fileAttente;
} t_guichet;
t_guichet* allouerTabGuichets(int n);
void ouvrirGuichet(t_guichet* guichets, int nbGuichets);
void fermerGuichets(t_guichet* guichets, int nbGuichets);
void ajouterPersonne(t_guichet* guichets, int nbGuichets, t_personne* personne, int maxFile);
t_personne* retirerPersonne(t_guichet* guichet);
void afficherGuichets(t_guichet* guichets, int nbGuichets);
t_personne* creerPersonne(int tourACtuel);
t_personne* consulterPremier(t_file* file);


#endif
