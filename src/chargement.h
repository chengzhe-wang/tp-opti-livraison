#ifndef CHARGEMENT_H
#define CHARGEMENT_H

#include "gestion_colis.h"

typedef struct {
    /* Exprimée en kilogrammes entiers pour la programmation dynamique. */
    int capacite_max;
} Camion;

typedef struct {
    Colis *colis_charges;
    int nombre_colis;
    int poids_total;
    int valeur_totale;
    int score_total;
} Chargement;

void chargement_initialiser(Chargement *chargement);
void chargement_liberer(Chargement *chargement);

/*
 * Cherche le chargement de valeur maximale sans dépasser la capacité du camion.
 * Retourne 1 en cas de succès et 0 en cas d'erreur.
 */
int camion_preparer_chargement_optimal(
    const CollectionColis *collection,
    const Camion *camion,
    Chargement *resultat
);

void chargement_afficher(const Chargement *chargement);

#endif
