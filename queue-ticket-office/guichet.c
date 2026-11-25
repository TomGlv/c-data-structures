#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "guichet.h"

t_personne* consulterPremier(t_file* file) {
    if (fileVide(file)) {
        return NULL;  // Retourne NULL si la file est vide
    }
    return (t_personne*)file->tete->data;  // Retourne un pointeur vers la première personne
}


// Allouer et initialiser un tableau de guichets
t_guichet* allouerTabGuichets(int n) {
    t_guichet* guichets = (t_guichet*)malloc(n * sizeof(t_guichet));  // Cast explicite ici
    for (int i = 0; i < n; i++) {
        guichets[i].numero = i + 1;
        guichets[i].ouvert = 0;
        init_file(&guichets[i].fileAttente);
    }
    return guichets;
}
void ouvrirGuichet(t_guichet* guichets, int nbGuichets) {
    for (int i = 0; i < nbGuichets; i++) {
        if (!guichets[i].ouvert) {
            guichets[i].ouvert = 1;
            printf("Guichet %d ouvert !\n", guichets[i].numero); // Debug
            return;
        }
    }
}



// Fermer les guichets avec une file vide, sauf un
void fermerGuichets(t_guichet* guichets, int nbGuichets) {
    int ouvertCount = 0;
    for (int i = 0; i < nbGuichets; i++) {
        if (guichets[i].ouvert) {
            ouvertCount++;
        }
    }
    for (int i = 0; i < nbGuichets; i++) {
        if (guichets[i].ouvert && fileVide(&guichets[i].fileAttente) && ouvertCount > 1) {
            guichets[i].ouvert = 0;
            ouvertCount--;
        }
    }
}

void ajouterPersonne(t_guichet* guichets, int nbGuichets, t_personne* personne, int maxFile) {
    int minFile = INT_MAX;
    int indiceMin = -1;

    for (int i = 0; i < nbGuichets; i++) {
        if (guichets[i].ouvert) {
            int taille = tailleF(&guichets[i].fileAttente);
            if (taille < minFile && taille < maxFile) { // Vérifie si la file n'est pas pleine
                minFile = taille;
                indiceMin = i;
            }
        }
    }

    if (indiceMin != -1) {
        enfiler(&guichets[indiceMin].fileAttente, personne);
        printf("     Ajout de %s (%d) au guichet %d\n", personne->nom, personne->anneeNaissance, indiceMin + 1);
    }
    else {
        //printf("Toutes les files sont pleines, impossible d'ajouter %s (%d)\n", personne->nom, personne->anneeNaissance);
    }
}


// Retirer la prochaine personne
t_personne* retirerPersonne(t_guichet* guichet) {
    return (t_personne*)defiler(&guichet->fileAttente);  // Retire une personne de la file
}

void afficherGuichets(t_guichet* guichets, int nbGuichets) {
    for (int i = 0; i < nbGuichets; i++) {
        printf("Guichet %d : ", guichets[i].numero);
        if (!guichets[i].ouvert) {
            printf("(FERME)\n");
            continue;
        }

        t_maillonF* temp = guichets[i].fileAttente.tete;
        if (!temp) {
            printf("(vide)\n");
            continue;
        }

        while (temp) {
            t_personne* p = (t_personne*)temp->data;
            printf("(%s, %d) ", p->nom, p->anneeNaissance);
            temp = temp->suivant;
        }
        printf("\n");
    }
}


t_personne* creerPersonne(int tourActuel) {
    t_personne* p = (t_personne*)malloc(sizeof(t_personne));
    if (p == NULL) {
        printf("Erreur d'allocation memoire\n");
        exit(EXIT_FAILURE);
    }

    // Générer un nom aléatoire (5 à 10 caractères)
    int longueurNom = 5 + rand() % 6; // Entre 5 et 10 caractères
    for (int i = 0; i < longueurNom - 2; i++) {
        p->nom[i] = 'A' + (rand() % 26); // Lettre majuscule aléatoire (A-Z)
    }
    //p->nom[longueurNom - 2] = '0' + (rand() % 10); // Chiffre
    //p->nom[longueurNom - 1] = '0' + (rand() % 10); // Chiffre
    p->nom[longueurNom-2] = '\0'; // Fin de chaîne

    // Générer une année de naissance aléatoire entre 1950 et 2024
    p->anneeNaissance = 1950 + rand() % 75;
    p->tourArrivee = tourActuel;  // On enregistre le tour d'arrivée
    return p;
}
