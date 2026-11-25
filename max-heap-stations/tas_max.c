#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tas_max.h"
void init_tas(t_tas*t,int nv)
{
    if(nv<1) nv=1;//au moins 1 niveau
    t->tailleM=pow(2,nv)-1;//calcul du nombre de noeuds max en fonction du nombre de niveaux demandé
    t->tab=malloc(t->tailleM*sizeof(t_noeud));//allocation du tableau
    t->n=0;//au depart il n'y a aucun noeud
}
int tasVide(t_tas*t)
{
    if(t->n==0) return 1;
    else return 0;
}
void ajouter_au_tas(t_tas* t, t_noeud nouveau) {
    int i_noeud, i_parent;
    t_noeud temp;

    if (t->n == t->tailleM) {
        // Doubler la taille du tas
        int new_size = 2 * t->tailleM + 1;
        t_noeud* new_tab = realloc(t->tab, new_size * sizeof(t_noeud));
        if (new_tab == NULL) {
            printf("Erreur de réallocation mémoire.\n");
            return;
        }
        t->tab = new_tab;
        t->tailleM = new_size;
    }

    // Ajouter le noeud à la fin du tas
    t->tab[t->n] = nouveau;
    i_noeud = t->n;
    i_parent = (i_noeud - 1) / 2;
    t->n++;

    // Permutation ascendante avec le parent si nécessaire
    while (i_noeud > 0 && t->tab[i_noeud].cle > t->tab[i_parent].cle) {
        temp = t->tab[i_noeud];
        t->tab[i_noeud] = t->tab[i_parent];
        t->tab[i_parent] = temp;
        i_noeud = i_parent;
        i_parent = (i_noeud - 1) / 2;
    }
}

void* supprimer_du_tas(t_tas* t) {
    if (t->n == 0) {
        printf("Erreur : Le tas est vide.\n");
        return NULL;
    }

    t_noeud temp;
    void* data = t->tab[0].data;

    // Remplacer la racine par le dernier élément
    t->tab[0] = t->tab[t->n - 1];
    t->n--;

    int i = 0;
    int stop = 0;
    int imax;

    // Permutation descendante pour réorganiser le tas
    while (!stop && (2 * i + 1 < t->n)) {
        imax = i;
        if (t->tab[2 * i + 1].cle >= t->tab[i].cle) {
            imax = 2 * i + 1;
        }
        if ((2 * i + 2 < t->n) && (t->tab[2 * i + 2].cle > t->tab[imax].cle)) {
            imax = 2 * i + 2;
        }
        if (imax != i) {
            temp = t->tab[i];
            t->tab[i] = t->tab[imax];
            t->tab[imax] = temp;
            i = imax;
        }
        else {
            stop = 1;
        }
    }
    return data;
}


void* consulter(t_tas*t)
{
    if(t->n>0)
        return t->tab[0].data;
    else return NULL;
}


