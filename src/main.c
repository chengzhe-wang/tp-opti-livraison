#include <stdio.h>
#include <stdlib.h>

#include "chargement.h"
#include "gestion_colis.h"

int main(void) {
    CollectionColis collection;
    collection_initialiser(&collection);

    printf("Après initialisation : taille = %d, capacité = %d\n",
           collection.taille, collection.capacite);

    collection.capacite = 5;
    collection.elements = malloc(collection.capacite * sizeof(Colis));
    if (collection.elements == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return 1;
    }

    Colis colis1 = {101, 8, 20, 2, "Paris"};
    Colis colis2 = {102, 3, 10, 5, "Lille"};
    Colis colis3 = {103, 12, 40, 1, "Lyon"};
    Colis colis4 = {104, 5, 25, 4, "Paris"};
    Colis colis5 = {105, 2, 15, 3, "Lille"};

    printf("Ajout du colis 101 : %d\n", collection_ajouter(&collection, colis1));
    printf("Ajout du colis 102 : %d\n", collection_ajouter(&collection, colis2));
    printf("Ajout du colis 103 : %d\n", collection_ajouter(&collection, colis3));
    printf("Ajout du colis 104 : %d\n", collection_ajouter(&collection, colis4));
    printf("Ajout du colis 105 : %d\n", collection_ajouter(&collection, colis5));

    printf("\nColis enregistrés :\n");
    collection_afficher(&collection);

    printf("\nTri par poids :\n");
    collection_afficher_triee(&collection, TRI_PAR_POIDS);

    printf("\nTri par valeur :\n");
    collection_afficher_triee(&collection, TRI_PAR_VALEUR);

    printf("\nTri par priorité :\n");
    collection_afficher_triee(&collection, TRI_PAR_PRIORITE);

    //Modifier la valeur pour tester différentes capacités maximales
    Camion camion = {20};
    Chargement resultat;
    chargement_initialiser(&resultat);

    if (camion_preparer_chargement_optimal(&collection, &camion, &resultat)) {
        printf("\nChargement optimal pour un camion de %d kg : valeur %d\n",
               camion.capacite_max, resultat.valeur_totale);
        printf("Valeur attendue : 110\n");
        printf("Poids total attendu : 30 kg (sur 50 kg maximum)\n");
        chargement_afficher(&resultat);
    } else {
        fprintf(stderr, "Erreur pendant la préparation du chargement.\n");
    }

    chargement_liberer(&resultat);
    collection_liberer(&collection);
    return 0;
}
