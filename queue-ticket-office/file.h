#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
typedef struct maillonF {
    void* data;
    struct maillonF* suivant;
} t_maillonF;

typedef struct {
    t_maillonF* tete;
    t_maillonF* fin;
} t_file;

void init_file(t_file* f);
int fileVide(t_file* f);
int tailleF(t_file* f);
void enfiler(t_file* f, void* data);
void* defiler(t_file* f);
void* tete(t_file* f);

#endif
