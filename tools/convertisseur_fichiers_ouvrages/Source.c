#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#include "header.h"

int main() {
    FILE* pfichierTXT;
    FILE* pfichierDAT;
    Film livre;
    int nbEnregistrements = 0;
    int nbLectures = 0;

    fopen_s(&pfichierTXT, "films.txt", "r");
    fopen_s(&pfichierDAT, "films.dat", "wb+");

    char ligne[100];
    char* token;
    char* next_token = "";
    char* pLigne;

    if (pfichierTXT == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1;
    }
    fgets(ligne, 100, pfichierTXT);
    pLigne = ligne;
    while (!feof(pfichierTXT)) {
        token = strtok(pLigne, "|");
        strcpy_s(livre.nomFilm, 200, token);
        token = strtok(NULL, "|");
        strcpy_s(livre.genres, 200, token);
        token = strtok(NULL, "|");
        livre.dureeFilm = atoi(token);
        token = strtok(NULL, "|");
        livre.ageMin = atoi(token);
        printf("Enregistrement : %s %s %d %d", livre.nomFilm, livre.genres, livre.dureeFilm, livre.ageMin);
        fwrite(&livre, sizeof(livre), 1, pfichierDAT);
        nbEnregistrements++;
        fgets(ligne, 100, pfichierTXT);
        pLigne = ligne;
    }

    fclose(pfichierTXT);
    fclose(pfichierDAT);

    fopen_s(&pfichierDAT, "films.dat", "rb");
    fread_s(&livre, sizeof(livre), sizeof(livre), 1, pfichierDAT);
    while (!feof(pfichierDAT)) {
        printf("Enregistrement : %s %s %d %d", livre.nomFilm, livre.genres, livre.dureeFilm, livre.ageMin);
        fread_s(&livre, sizeof(livre), sizeof(livre), 1, pfichierDAT);
        nbLectures++;
    }
    
    if (nbEnregistrements == nbLectures) {
        printf("Tous les enregistrements ont ete verifies\n");
    }
    else {
        printf("%d enregistrements n'ont pas ete verifies\n", nbEnregistrements - nbLectures);
    }

}