#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "chargement.h"
#include "gestion_colis.h"

#define REPETITIONS_RECHERCHE 1000

static int lire_entier(const char *invite, int *valeur) {
    char ligne[128];
    char caractere_supplementaire;

    printf("%s", invite);
    if (fgets(ligne, sizeof(ligne), stdin) == NULL) {
        return 0;
    }

    return sscanf(ligne, " %d %c", valeur, &caractere_supplementaire) == 1;
}

static int lire_texte(const char *invite, char texte[], size_t taille) {
    printf("%s", invite);
    if (fgets(texte, taille, stdin) == NULL) {
        return 0;
    }

    texte[strcspn(texte, "\n")] = '\0';
    return texte[0] != '\0';
}

static void afficher_menu(void) {
    printf("\n===== GESTION DES LIVRAISONS =====\n");
    printf("1. Afficher les colis\n");
    printf("2. Ajouter un colis\n");
    printf("3. Rechercher un colis\n");
    printf("4. Organiser les colis\n");
    printf("5. Préparer un camion\n");
    printf("6. Afficher le dernier chargement\n");
    printf("7. Afficher les statistiques\n");
    printf("8. Exécuter les tests de performance\n");
    printf("0. Quitter\n");
}

static void charger_donnees_exemple(CollectionColis *collection) {
    const Colis exemples[] = {
        {101, 8, 20, 2, "Paris"},
        {102, 3, 10, 5, "Lille"},
        {103, 12, 40, 1, "Lyon"},
        {104, 5, 25, 4, "Paris"},
        {105, 2, 15, 3, "Lille"}
    };

    for (size_t i = 0; i < sizeof(exemples) / sizeof(exemples[0]); ++i) {
        collection_ajouter(collection, exemples[i]);
    }
}

static void ajouter_colis_console(CollectionColis *collection) {
    Colis colis;

    if (!lire_entier("Identifiant : ", &colis.id) ||
        !lire_entier("Poids (kg entier) : ", &colis.poids) ||
        !lire_entier("Valeur : ", &colis.valeur) ||
        !lire_entier("Priorité : ", &colis.priorite) ||
        !lire_texte("Destination : ", colis.destination, DESTINATION_MAX)) {
        printf("Saisie invalide.\n");
        return;
    }

    if (colis.id <= 0 || colis.poids < 0 || colis.valeur < 0 || colis.priorite < 0) {
        printf("Les valeurs numériques doivent être positives ou nulles.\n");
        return;
    }

    if (collection_ajouter(collection, colis)) {
        printf("Colis ajouté.\n");
    } else {
        printf("Ajout impossible : identifiant déjà utilisé ou manque de mémoire.\n");
    }
}

static void rechercher_colis_console(const CollectionColis *collection) {
    int id;
    if (!lire_entier("Identifiant recherché : ", &id)) {
        printf("Saisie invalide.\n");
        return;
    }

    const Colis *colis = collection_trouver_par_id(collection, id);
    if (colis == NULL) {
        printf("Aucun colis avec l'identifiant %d.\n", id);
        return;
    }

    printf("Colis %d : %d kg, valeur %d, priorité %d, destination %s\n",
           colis->id, colis->poids, colis->valeur, colis->priorite,
           colis->destination);
}

static void organiser_colis_console(const CollectionColis *collection) {
    int choix;
    if (!lire_entier("1. Poids  2. Valeur  3. Priorité : ", &choix)) {
        printf("Saisie invalide.\n");
        return;
    }

    if (choix == 1) {
        collection_afficher_triee(collection, TRI_PAR_POIDS);
    } else if (choix == 2) {
        collection_afficher_triee(collection, TRI_PAR_VALEUR);
    } else if (choix == 3) {
        collection_afficher_triee(collection, TRI_PAR_PRIORITE);
    } else {
        printf("Choix inconnu.\n");
    }
}

static int preparer_camion_console(
    const CollectionColis *collection,
    Chargement *chargement
) {
    Camion camion;
    if (!lire_entier("Capacité maximale du camion (kg) : ", &camion.capacite_max) ||
        camion.capacite_max < 0) {
        printf("Capacité invalide.\n");
        return 0;
    }

    if (!camion_preparer_chargement_optimal(collection, &camion, chargement)) {
        printf("Impossible de préparer le chargement.\n");
        return 0;
    }

    printf("Chargement calculé avec la règle valeur + 10 × priorité.\n");
    chargement_afficher(chargement);
    return 1;
}

static void afficher_statistiques(const CollectionColis *collection) {
    long poids_total = 0;
    long valeur_totale = 0;

    for (int i = 0; i < collection->taille; ++i) {
        poids_total += collection->elements[i].poids;
        valeur_totale += collection->elements[i].valeur;
    }

    printf("Nombre de colis : %d\n", collection->taille);
    printf("Capacité interne du tableau : %d\n", collection->capacite);
    printf("Poids cumulé : %ld kg\n", poids_total);
    printf("Valeur cumulée : %ld\n", valeur_totale);
}

static unsigned int nombre_pseudo_aleatoire(unsigned int *etat) {
    *etat = *etat * 1103515245u + 12345u;
    return *etat;
}

static int generer_collection_test(CollectionColis *collection, int taille) {
    unsigned int etat = 12345u;
    collection_initialiser(collection);

    collection->elements = malloc((size_t)taille * sizeof(*collection->elements));
    if (collection->elements == NULL) {
        return 0;
    }

    for (int i = 0; i < taille; ++i) {
        Colis colis = {
            i + 1,
            (int)(nombre_pseudo_aleatoire(&etat) % 20u) + 1,
            (int)(nombre_pseudo_aleatoire(&etat) % 100u) + 1,
            (int)(nombre_pseudo_aleatoire(&etat) % 5u) + 1,
            "Test"
        };

        collection->elements[i] = colis;
    }

    collection->taille = taille;
    collection->capacite = taille;
    return 1;
}

static void mesurer_performances(int taille) {
    CollectionColis collection;
    if (!generer_collection_test(&collection, taille)) {
        fprintf(stderr, "Impossible de générer %d colis.\n", taille);
        return;
    }

    clock_t debut_recherche = clock();
    int somme_ids = 0;
    for (int i = 0; i < REPETITIONS_RECHERCHE; ++i) {
        const Colis *colis = collection_trouver_par_id(&collection, taille);
        if (colis != NULL) {
            somme_ids += colis->id;
        }
    }
    clock_t fin_recherche = clock();

    Camion camion = {50};
    Chargement resultat;
    chargement_initialiser(&resultat);
    clock_t debut_chargement = clock();
    int succes = camion_preparer_chargement_optimal(&collection, &camion, &resultat);
    clock_t fin_chargement = clock();

    double temps_recherche = (double)(fin_recherche - debut_recherche) / CLOCKS_PER_SEC;
    double temps_chargement = (double)(fin_chargement - debut_chargement) / CLOCKS_PER_SEC;

    if (succes) {
        printf("%6d | %12.6f s | %12.6f s | %d\n",
               taille, temps_recherche, temps_chargement, somme_ids);
    } else {
        fprintf(stderr, "Erreur lors du chargement pour %d colis.\n", taille);
    }

    chargement_liberer(&resultat);
    collection_liberer(&collection);
}

static void executer_tests_performance(void) {
    const int tailles[] = {100, 1000, 10000, 100000};

    printf("\nTests de performance\n");
    printf("Colis  | Recherche ID | Chargement   | Contrôle\n");
    printf("------------------------------------------------\n");

    for (size_t i = 0; i < sizeof(tailles) / sizeof(tailles[0]); ++i) {
        mesurer_performances(tailles[i]);
    }
}

int main(void) {
    CollectionColis collection;
    Chargement dernier_chargement;
    int chargement_disponible = 0;
    int choix;

    collection_initialiser(&collection);
    chargement_initialiser(&dernier_chargement);
    charger_donnees_exemple(&collection);

    printf("Données d'exemple chargées : 5 colis.\n");

    do {
        afficher_menu();
        if (!lire_entier("Choix : ", &choix)) {
            printf("Saisie invalide.\n");
            continue;
        }

        switch (choix) {
            case 1:
                collection_afficher(&collection);
                break;
            case 2:
                ajouter_colis_console(&collection);
                break;
            case 3:
                rechercher_colis_console(&collection);
                break;
            case 4:
                organiser_colis_console(&collection);
                break;
            case 5:
                chargement_disponible = preparer_camion_console(
                    &collection,
                    &dernier_chargement
                );
                break;
            case 6:
                if (chargement_disponible) {
                    chargement_afficher(&dernier_chargement);
                } else {
                    printf("Aucun chargement n'a encore été préparé.\n");
                }
                break;
            case 7:
                afficher_statistiques(&collection);
                break;
            case 8:
                executer_tests_performance();
                break;
            case 0:
                break;
            default:
                printf("Choix inconnu.\n");
                break;
        }
    } while (choix != 0);

    chargement_liberer(&dernier_chargement);
    collection_liberer(&collection);
    printf("Au revoir.\n");
    return 0;
}
