#include <stdio.h>
#include <string.h>
typedef struct salle{
	int numero;
	int nbPlaces;
} Salle;

typedef struct film {
    char nomFilm[200];
    char genres[200];
    int dureeFilm;
    int ageMin;
}Film;