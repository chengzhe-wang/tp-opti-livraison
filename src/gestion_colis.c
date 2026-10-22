#include "gestion_colis.h"
#include <stdlib.h>
#include <stdio.h>

void collection_initialiser(CollectionColis *collection) {
    collection->elements = NULL;
    collection->taille = 0;
    collection->capacite = 0;
}

void collection_liberer(CollectionColis *collection) {

    if(collection != NULL){
        free(collection->elements);
        collection->elements = NULL;
        collection->taille = 0;
        collection->capacite = 0;
    }
}

int collection_ajouter(CollectionColis *collection, Colis colis) {
    if(collection == NULL){
        return 0;
    }

    if(collection->taille < collection->capacite){
        collection->elements[collection->taille] = colis;
        collection->taille ++;
        return 1;
    }
    return 0;
}

void collection_afficher(const CollectionColis *collection) {
    if(collection == NULL){
        printf("Aucun colis enregistré \n");
    }
    else{
        for(int i = 0; i < collection->taille; i++){
        printf("Colis numéro %d : %.2f kilos, valeur de %.2f, priorité de %d \n", 
            collection->elements[i].id, 
            collection->elements[i].poids,
            collection->elements[i].valeur, 
            collection->elements[i].priorite);
        }
    }
}

const Colis *collection_trouver_par_id(const CollectionColis *collection, int id) {
    if(collection == NULL){
        return NULL;
    }
    for(int i = 0; i < collection->taille; i++){
        if(collection->elements[i].id == id){
            return &collection->elements[i];
        }
    }
    return NULL;
}

/* Retourne une valeur négative si gauche doit apparaître avant droite. */
static int comparer_colis(
    const Colis *gauche,
    const Colis *droite,
    CritereTri critere
) {
    switch (critere) {
        case TRI_PAR_POIDS:
            if (gauche->poids < droite->poids) return -1;
            if (gauche->poids > droite->poids) return 1;
            return 0;

        case TRI_PAR_VALEUR:
            if (gauche->valeur > droite->valeur) return -1;
            if (gauche->valeur < droite->valeur) return 1;
            return 0;

        case TRI_PAR_PRIORITE:
            if (gauche->priorite > droite->priorite) return -1;
            if (gauche->priorite < droite->priorite) return 1;
            return 0;
    }

    return 0;
}

static void echanger_colis(Colis *gauche, Colis *droite) {
    Colis temporaire = *gauche;
    *gauche = *droite;
    *droite = temporaire;
}

/* Place le pivot à sa position finale et renvoie son indice. */
static int partitionner(
    Colis elements[],
    int debut,
    int fin,
    CritereTri critere
) {
    /* Un pivot au milieu limite le risque d'un mauvais cas sur des données triées. */
    int indice_pivot = debut + (fin - debut) / 2;
    echanger_colis(&elements[indice_pivot], &elements[fin]);

    Colis pivot = elements[fin];
    int i = debut - 1;

    for (int j = debut; j < fin; ++j) {
        if (comparer_colis(&elements[j], &pivot, critere) <= 0) {
            ++i;
            echanger_colis(&elements[i], &elements[j]);
        }
    }

    echanger_colis(&elements[i + 1], &elements[fin]);
    return i + 1;
}

static void tri_rapide(
    Colis elements[],
    int debut,
    int fin,
    CritereTri critere
) {
    if (debut >= fin) {
        return;
    }

    int position_pivot = partitionner(elements, debut, fin, critere);
    tri_rapide(elements, debut, position_pivot - 1, critere);
    tri_rapide(elements, position_pivot + 1, fin, critere);
}

void collection_afficher_triee(const CollectionColis *collection, CritereTri critere) {
    if (collection == NULL) {
        printf("Aucun colis enregistré\n");
        return;
    }

    if (collection->taille == 0) {
        printf("La collection est vide\n");
        return;
    }

    Colis *copie = malloc((size_t)collection->taille * sizeof(*copie));
    if (copie == NULL) {
        printf("Erreur d'allocation mémoire pour le tri\n");
        return;
    }

    for (int i = 0; i < collection->taille; ++i) {
        copie[i] = collection->elements[i];
    }

    tri_rapide(copie, 0, collection->taille - 1, critere);

    CollectionColis collection_triee = {copie, collection->taille, collection->taille};
    collection_afficher(&collection_triee);

    free(copie);
}


