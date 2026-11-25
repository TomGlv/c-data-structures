#include <stdlib.h>
#include "file.h"

void init_file(t_file* f) {
    f->tete = f->fin = NULL;
}

int fileVide(t_file* f) {
    return (f->tete == NULL);
}

int tailleF(t_file* f) {
    int n = 0;
    t_maillonF* temp = f->tete;
    while (temp) {
        n++;
        temp = temp->suivant;
    }
    return n;
}

void enfiler(t_file* f, void* data) {
    t_maillonF* nouveau = (t_maillonF*)malloc(sizeof(t_maillonF));
    if (!nouveau) return;
    nouveau->data = data;
    nouveau->suivant = NULL;
    if (f->tete == NULL)
        f->tete = f->fin = nouveau;
    else {
        f->fin->suivant = nouveau;
        f->fin = nouveau;
    }
}

void* defiler(t_file* f) {
    if (fileVide(f)) return NULL;
    t_maillonF* temp = f->tete;
    void* data = temp->data;
    f->tete = temp->suivant;
    if (!f->tete)
        f->fin = NULL;
    free(temp);
    return data;
}

void* tete(t_file* f) {
    return fileVide(f) ? NULL : f->tete->data;
}
