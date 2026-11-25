#define _CRT_SECURE_NO_WARNINGS  
#include "tas_max.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* nom;
    char* montagne;
    int pistes[4]; // 0: vertes, 1: bleues, 2: rouges, 3: noires
} Station;

Station* chargerStations(const char* filename, int* n) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        *n = 0;
        return NULL;
    }
    fscanf(file, "%d\n", n);
    Station* tab = malloc(*n * sizeof(Station));
    for (int i = 0; i < *n; i++) {
        char ligne[256], * token;
        fgets(ligne, sizeof(ligne), file);
        ligne[strcspn(ligne, "\n")] = 0;

        token = strtok(ligne, ";");
        tab[i].nom = malloc(strlen(token) + 1);
        strcpy(tab[i].nom, token);

        token = strtok(NULL, ";");
        tab[i].montagne = malloc(strlen(token) + 1);
        strcpy(tab[i].montagne, token);

        for (int j = 0; j < 4; j++) {
            token = strtok(NULL, ";");
            tab[i].pistes[j] = strtol(token, NULL, 10);
        }
    }
    fclose(file);
    return tab;
}

void afficherStation(const Station* s) {
    if (s) {
        printf("Station: %s\nMontagne: %s\nPistes: V:%d B:%d R:%d N:%d\n",
            s->nom, s->montagne, s->pistes[0], s->pistes[1], s->pistes[2], s->pistes[3]);
    }
}

void afficherStations(const Station* tab, int n) {
    for (int i = 0; i < n; i++) {
        afficherStation(&tab[i]);
    }
}

Station* rechercherStation(Station* tab, int n, const char* nom) {
    for (int i = 0; i < n; i++) {
        if (strcmp(tab[i].nom, nom) == 0) {
            return &tab[i];
        }
    }
    return NULL;
}

Station* trierStations(Station* tab, int n, int type) {
    printf("Début du tri par type de piste %d...\n", type);

    // Initialisation du tas
    t_tas tas;
    init_tas(&tas, n);

    // Ajout des stations dans le tas
    for (int i = 0; i < n; i++) {
        t_noeud noeud = { tab[i].pistes[type], &tab[i] };
        printf("Ajout au tas: station %s, piste %d, valeur %d\n", tab[i].nom, type, tab[i].pistes[type]);
        ajouter_au_tas(&tas, noeud);
    }

    // Allocation mémoire pour le tableau trié
    Station* trie = malloc(n * sizeof(Station));
    if (trie == NULL) {
        printf("Erreur d'allocation mémoire pour les stations triées.\n");
        return NULL;
    }

    // Extraction des éléments triés du tas
    for (int i = 0; i < n; i++) {
        Station* temp = (Station*)supprimer_du_tas(&tas);
        if (temp) {
            trie[i] = *temp;  // Copie de la station triée
            printf("Station extraite: %s, Pistes: %d\n", trie[i].nom, trie[i].pistes[type]);
        }
        else {
            printf("Erreur lors de la suppression du tas.\n");
            free(trie);  // Libération de la mémoire allouée si erreur
            return NULL;
        }
    }
    return trie;
}



void libererStations(Station* tab, int n) {
    if (!tab) return;
    for (int i = 0; i < n; i++) {
        free(tab[i].nom);
        free(tab[i].montagne);
    }
    free(tab);
}

int main() {
    int n;
    Station* stations = chargerStations("stations.txt", &n);
    if (!stations) return 1;

    int choix;
    do {
        printf("\nMenu:\n1. Afficher toutes les stations\n2. Rechercher une station\n3. Trier par type de piste\n4. Quitter\nChoix: ");
        scanf("%d", &choix);
        while (getchar() != '\n'); // Évite les problèmes avec fgets()

        if (choix == 1) {
            afficherStations(stations, n);
        }
        else if (choix == 2) {
            char nom[100];
            printf("Nom de la station: ");
            fgets(nom, 100, stdin);
            nom[strcspn(nom, "\n")] = 0;
            Station* s = rechercherStation(stations, n, nom);
            if (s) afficherStation(s);
            else printf("Cette station n'existe pas !\n");
        }
        else if (choix == 3) {
            int type;
            printf("Trier par piste (0: vertes, 1: bleues, 2: rouges, 3: noires): ");
            scanf("%d", &type);
            while (getchar() != '\n'); // Évite les problèmes avec fgets()
            Station* trie = trierStations(stations, n, type);
            afficherStations(trie, n);
            free(trie);
        }
    } while (choix != 4);

    libererStations(stations, n);
    return 0;
}