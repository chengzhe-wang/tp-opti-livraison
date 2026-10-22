#include <stdio.h>
#include <stdlib.h>

#include "gestion_colis.h"

int main(void) {
    CollectionColis collection;
    collection_initialiser(&collection);

    printf("Après initialisation : taille = %d, capacité = %d\n",
           collection.taille, collection.capacite);

    /* Temporaire : l'agrandissement sera codé plus tard dans collection_ajouter. */
    collection.capacite = 3;
    collection.elements = malloc(collection.capacite * sizeof(Colis));
    if (collection.elements == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return 1;
    }

    Colis colis1 = {101, 8.0, 20.0, 2, "Paris"};
    Colis colis2 = {102, 3.0, 10.0, 5, "Lille"};
    Colis colis3 = {103, 12.0, 40.0, 1, "Lyon"};

    printf("Ajout du colis 101 : %d\n", collection_ajouter(&collection, colis1));
    printf("Ajout du colis 102 : %d\n", collection_ajouter(&collection, colis2));
    printf("Ajout du colis 103 : %d\n", collection_ajouter(&collection, colis3));

    printf("\nColis enregistrés :\n");
    collection_afficher(&collection);

    printf("\nTri par poids :\n");
    collection_afficher_triee(&collection, TRI_PAR_POIDS);

    printf("\nTri par valeur :\n");
    collection_afficher_triee(&collection, TRI_PAR_VALEUR);

    printf("\nTri par priorité :\n");
    collection_afficher_triee(&collection, TRI_PAR_PRIORITE);

    collection_liberer(&collection);
    return 0;
}
