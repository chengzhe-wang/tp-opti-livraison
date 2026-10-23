#include "chargement.h"
#include <stdio.h>
#include <stdlib.h>

void chargement_initialiser(Chargement *chargement) {
    if (chargement == NULL) {
        return;
    }

    chargement->colis_charges = NULL;
    chargement->nombre_colis = 0;
    chargement->poids_total = 0;
    chargement->valeur_totale = 0;
}

void chargement_liberer(Chargement *chargement) {
    if(chargement != NULL){
        free(chargement->colis_charges);
        chargement->colis_charges = NULL;
        chargement->nombre_colis = 0;
        chargement->poids_total = 0;
        chargement->valeur_totale = 0;
    }
}

int camion_preparer_chargement_optimal(const CollectionColis *collection, const Camion *camion, Chargement *resultat) {
    if (collection == NULL || camion == NULL || resultat == NULL) {
        return 0;
    }

    if (collection->taille < 0 || camion->capacite_max < 0) {
        return 0;
    }

    if (collection->taille > 0 && collection->elements == NULL) {
        return 0;
    }

    for (int i = 0; i < collection->taille; ++i) {
        if (collection->elements[i].poids < 0) {
            return 0;
        }
    }

    chargement_initialiser(resultat);

    int lignes = collection->taille + 1;
    int colonnes = camion->capacite_max + 1;
    int *table = calloc(lignes * colonnes, sizeof(*table));
    if (table == NULL) {
        return 0;
    }
    for (int i = 1; i < lignes; i++) {
        const Colis *colis = &collection->elements[i - 1];
        for (int j = 0; j < colonnes; j++) {
            if(colis->poids > j){
                table[i * colonnes + j] = table[(i - 1) * colonnes + j];
            }
            else{
                int sans_colis = table[(i - 1) * colonnes + j];
                int avec_colis = colis->valeur + table[(i - 1) * colonnes + (j - colis->poids)];
                if (avec_colis > sans_colis) {
                    table[i * colonnes + j] = avec_colis;
                } else {
                    table[i * colonnes + j] = sans_colis;
                }
            }
            
        }
    }
    resultat->valeur_totale = table[(lignes - 1) * colonnes + (colonnes - 1)];

    if (collection->taille > 0) {
        resultat->colis_charges = malloc(
            (size_t)collection->taille * sizeof(*resultat->colis_charges)
        );
        if (resultat->colis_charges == NULL) {
            free(table);
            return 0;
        }
    }

    int capacite_restante = camion->capacite_max;
    for (int i = collection->taille; i > 0; --i) {
        int valeur_actuelle = table[i * colonnes + capacite_restante];
        int valeur_sans_colis = table[(i - 1) * colonnes + capacite_restante];

        if (valeur_actuelle != valeur_sans_colis) {
            const Colis *colis = &collection->elements[i - 1];
            resultat->colis_charges[resultat->nombre_colis++] = *colis;
            resultat->poids_total += colis->poids;
            capacite_restante -= colis->poids;
        }
    }

    /* La remontée est faite du dernier colis vers le premier : on inverse l'ordre. */
    for (int gauche = 0, droite = resultat->nombre_colis - 1;
         gauche < droite;
         ++gauche, --droite) {
        Colis temporaire = resultat->colis_charges[gauche];
        resultat->colis_charges[gauche] = resultat->colis_charges[droite];
        resultat->colis_charges[droite] = temporaire;
    }

    free(table);
    return 1;
}

void chargement_afficher(const Chargement *chargement) {
    if (chargement == NULL) {
        return;
    }

    printf("Poids total : %d kg\n", chargement->poids_total);
    printf("Valeur totale : %d\n", chargement->valeur_totale);
    printf("Colis sélectionnés :\n");

    for (int i = 0; i < chargement->nombre_colis; ++i) {
        const Colis *colis = &chargement->colis_charges[i];
        printf("- Colis %d : %d kg, valeur %d\n",
               colis->id, colis->poids, colis->valeur);
    }
}
