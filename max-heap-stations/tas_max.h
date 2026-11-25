#ifndef TAS_MAX_H_INCLUDED
#define TAS_MAX_H_INCLUDED

//type pour les noeuds
typedef struct noeud{
    int cle;
    void*data;
}t_noeud;
//type pour le tas
typedef struct tas{
    t_noeud*tab;
    int tailleM;
    int n;
} t_tas;

//prototypes des sous-programmes
/*initialisation d'un tas vide
parametres : l'adresse du tas, son nombre de niveau de départ (>=1)*/
void init_tas(t_tas*t,int tM);

/*test si le tas est vide
parametres : l'adresse du tas
retour : 1 si vide, 0 sinon */
int tasVide(t_tas*t);

/*ajouter un element
parametres : l'adresse du tas, le noeud à ajouter (le sous-programme en fait une copie)*/
void ajouter_au_tas(t_tas*t, t_noeud nouveau);

/*supprimer la racine du tas (le noeud ayant la plus grande clé
parametres : l'adresse du tas
retour : l'adresse de la donnée du noeud supprimé*/
void* supprimer_du_tas(t_tas*t);

/*accéder à l'élément de plus haute priorité
parametres : l'adresse du tas
retour : l'adresse de la donnée du noeud racine*/
void* consulter(t_tas*t);

#endif // TAS_MAX_H_INCLUDED
