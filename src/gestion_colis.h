#ifndef GESTION_COLIS_H
#define GESTION_COLIS_H

#include <stddef.h>

#include "colis.h"

typedef enum {
    TRI_PAR_POIDS,
    TRI_PAR_VALEUR,
    TRI_PAR_PRIORITE
} CritereTri;

typedef struct {
    Colis *elements;
    int taille;
    int capacite;
} CollectionColis;

void collection_initialiser(CollectionColis *collection);
void collection_liberer(CollectionColis *collection);

/* Retourne 1 en cas de succès et 0 en cas d'erreur. */
int collection_ajouter(CollectionColis *collection, Colis colis);
void collection_afficher(const CollectionColis *collection);
void collection_afficher_triee(
    const CollectionColis *collection,
    CritereTri critere
);
const Colis *collection_trouver_par_id(const CollectionColis *collection, int id);

#endif
