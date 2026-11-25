#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PILES 6
#define MAX_CONTENEURS 10

typedef struct {
    char conteneurs[MAX_CONTENEURS];
    int taille;
} Pile;

Pile piles[MAX_PILES];

void charger_donnees() {
    FILE* fichier = fopen("stock_containers.txt", "r");
    if (fichier == NULL) {
        printf("Erreur de chargement du fichier.\n");
        return;
    }

    int nombre_piles;
    fscanf(fichier, "%d", &nombre_piles);

    for (int i = 0; i < nombre_piles; i++) {
        int taille;
        fscanf(fichier, "%d", &taille);
        piles[i].taille = taille;
        for (int j = 0; j < taille; j++) {
            fscanf(fichier, " %c", &piles[i].conteneurs[j]);
        }
    }
    fclose(fichier);
}

void afficher_piles() {
    printf("\nEtat des piles actuelles :\n");
    for (int i = 0; i < MAX_PILES; i++) {
        printf("Pile %d : ", i);
        for (int j = piles[i].taille - 1; j >= 0; j--) {
            printf("%c ", piles[i].conteneurs[j]);
        }
        printf("\n");
    }
}

char generer_identifiant_entreprise() {
    return 'A' + (rand() % 26);
}

void generer_fichier() {
    FILE* fichier = fopen("stock_containers.txt", "w");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }

    srand(time(NULL));

    int nombre_piles = MAX_PILES;
    fprintf(fichier, "%d\n", nombre_piles);

    for (int i = 0; i < nombre_piles; i++) {
        int taille_pile = rand() % (MAX_CONTENEURS + 1);
        fprintf(fichier, "%d ", taille_pile);

        for (int j = 0; j < taille_pile; j++) {
            char id_entreprise = generer_identifiant_entreprise();
            fprintf(fichier, "%c ", id_entreprise);
        }

        fprintf(fichier, "\n");
    }

    fclose(fichier);
    printf("Le fichier stock_containers.txt a ete genere avec succes.\n");
}

void livrer_conteneur(char id_entreprise) {
    int trouve = 0;
    for (int i = 0; i < MAX_PILES; i++) {
        for (int j = 0; j < piles[i].taille; j++) {
            if (piles[i].conteneurs[j] == id_entreprise) {
                printf("Conteneur %c livre de la pile %d.\n", id_entreprise, i);
                for (int k = j; k < piles[i].taille - 1; k++) {
                    piles[i].conteneurs[k] = piles[i].conteneurs[k + 1];
                }
                piles[i].taille--;
                trouve = 1;
                afficher_piles();
                return;
            }
        }
    }

    if (!trouve) {
        printf("Aucun conteneur de l'entreprise %c trouve.\n", id_entreprise);
    }
}

void sauvegarder_donnees() {
    FILE* fichier = fopen("stock_containers.txt", "w");
    if (fichier == NULL) {
        printf("Erreur de sauvegarde du fichier.\n");
        return;
    }

    fprintf(fichier, "%d\n", MAX_PILES);

    for (int i = 0; i < MAX_PILES; i++) {
        fprintf(fichier, "%d ", piles[i].taille);
        for (int j = 0; j < piles[i].taille; j++) {
            fprintf(fichier, "%c ", piles[i].conteneurs[j]);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
    printf("Les informations ont ete sauvegardees.\n");
}

int main() {
    charger_donnees();
    int choix;
    char entreprise;
    while (1) {
        printf("\nMenu :\n");
        printf("1. Livrer un conteneur\n");
        printf("2. Sauvegarder et quitter\n");
        printf("3. Generer un stock_containers.txt\n");
        printf("Choix : ");
        scanf("%d", &choix);

        if (choix == 1) {
            afficher_piles();
            printf("Entrer l'identifiant de l'entreprise : ");
            scanf(" %c", &entreprise);
            livrer_conteneur(entreprise);
        }
        else if (choix == 2) {
            sauvegarder_donnees();
            break;
        }
        else if (choix == 3) {
            generer_fichier();
        }
        else {
            printf("Choix invalide. Essayez encore.\n");
        }
    }

    return 0;
}
