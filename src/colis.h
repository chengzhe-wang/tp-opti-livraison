#ifndef COLIS_H
#define COLIS_H

#define DESTINATION_MAX 50

typedef struct {
    int id;
    double poids;
    double valeur;
    int priorite;
    char destination[DESTINATION_MAX];
} Colis;

#endif
