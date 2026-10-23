#ifndef COLIS_H
#define COLIS_H

#define DESTINATION_MAX 50

typedef struct {
    int id;
    int poids;
    int valeur;
    int priorite;
    char destination[DESTINATION_MAX];
} Colis;

#endif
