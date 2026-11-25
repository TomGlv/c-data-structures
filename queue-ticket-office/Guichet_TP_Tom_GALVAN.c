#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>  // Utilisé pour Sleep sous Windows

#include "guichet.h"

int main() {
    srand(time(NULL));

    int nbGuichets, maxFile;
    printf("nombre de guichets : ");
    scanf("%d", &nbGuichets);
    printf("taille max des files d'attente : ");
    scanf("%d", &maxFile);

    // Allouer et initialiser les guichets (tous initialement fermés)
    t_guichet* guichets = allouerTabGuichets(nbGuichets);

    // Au début, on ouvre le premier guichet pour démarrer la simulation
    ouvrirGuichet(guichets, nbGuichets);

    int iteration = 0;
    int simulationFinished = 0;
    while (!simulationFinished) {
        printf("\n===== Tour de simulation %d =====\n", iteration);

        // 1. Arrivées : un nombre aléatoire (0 à 5) de personnes arrive
        int nbArrivals = rand() % 6;
        printf("Arrivees : %d\n", nbArrivals);
        for (int i = 0; i < nbArrivals; i++) {
            t_personne* personne = creerPersonne(iteration);
            // Ajoute la personne dans le guichet ouvert ayant la file la plus courte
            ajouterPersonne(guichets, nbGuichets, personne, maxFile);

            // Si tous les guichets ouverts sont saturés, tenter d'ouvrir un guichet fermé (s'il y en a)
            int allSaturated = 1;
            for (int j = 0; j < nbGuichets; j++) {
                if (guichets[j].ouvert && tailleF(&guichets[j].fileAttente) < maxFile) {
                    allSaturated = 0;
                    break;
                }
            }
            if (allSaturated) {
                for (int j = 0; j < nbGuichets; j++) {
                    if (!guichets[j].ouvert) {
                        ouvrirGuichet(guichets, nbGuichets);
                        break;
                    }
                }
            }
        }

        // 2. Départs : déterminer un nombre de départs aléatoire,
        // ne dépassant jamais le nombre total de personnes dans les files.
        int totalQueueCount = 0;
        for (int j = 0; j < nbGuichets; j++) {
            totalQueueCount += tailleF(&guichets[j].fileAttente);
        }
        int nbDepartures = 0;
        if (totalQueueCount > 0) {
            // Si le total est inférieur à 4, choisir de 0 à (total - 1) départs ;
            // sinon, choisir de 0 à 3 départs.
            if (totalQueueCount < 4)
                nbDepartures = rand() % totalQueueCount;
            else
                nbDepartures = rand() % 4;
        }
        printf("Depart de %d personnes si au guichet depuis au moins un tour, sinon rien\n", nbDepartures);
        for (int i = 0; i < nbDepartures; i++) {
            int departed = 0;
            // Parcourir les guichets ouverts et retirer une personne dès qu'une file n'est pas vide
            for (int j = 0; j < nbGuichets; j++) {
                if (guichets[j].ouvert && !fileVide(&guichets[j].fileAttente)) {
                    t_personne* premier = consulterPremier(&guichets[j].fileAttente);  // Regarder la première personne sans la retirer
                    if (premier->tourArrivee < iteration) {  // Vérifier qu'elle a attendu au moins un tour
                        t_personne* dep = retirerPersonne(&guichets[j]);  // Maintenant on peut la retirer
                        //printf("     Depart de %s (%d) du guichet %d\n", dep->nom, dep->anneeNaissance, guichets[j].numero);
                        free(dep);  // Libérer la mémoire
                    }
                    else {
                        printf("     Aucune personne eligible au depart dans le guichet %d.\n", guichets[j].numero);
                    }
                }
            }

            if (!departed) {
                //printf("Aucune personne a retirer pour ce depart.\n");
            }
        }

        // 3. Fermeture dynamique des guichets :
        // Fermer les guichets ouverts dont la file est vide, en ne gardant qu'au moins un ouvert.
        fermerGuichets(guichets, nbGuichets);

        // 4. Affichage de l'état des guichets et de leurs files d'attente
        afficherGuichets(guichets, nbGuichets);

        // 5. Condition d'arrêt : si toutes les files sont vides, la simulation s'arrête.
        int totalPersons = 0;
        for (int j = 0; j < nbGuichets; j++) {
            totalPersons += tailleF(&guichets[j].fileAttente);
        }
        printf("\nTotal des personnes dans les files : %d\n", totalPersons); // Débogage
        if (totalPersons == 0) {
            printf("Simulation terminee.\n\n");
            simulationFinished = 1;
        }
        iteration++;

        // Pause de 10 secondes avant le prochain tour de simulation
        printf("Pause de 10 secondes...\n");
        Sleep(10000); // Pause 10 secondes sous Windows
    }
    free(guichets);
    return 0;
}
