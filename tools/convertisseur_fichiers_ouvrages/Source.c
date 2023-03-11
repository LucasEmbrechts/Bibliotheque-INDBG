#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#include "header.h"

int main() {
    FILE* pfichierTXT;
    FILE* pfichierDAT;
    Livre livre;
    int nbEnregistrements = 0;
    int nbLectures = 0;

    fopen_s(&pfichierTXT, "ouvrages.txt", "r");
    fopen_s(&pfichierDAT, "ouvrages.dat", "wb+");

    char ligne[100];
    char* token;
    char* next_token = "";
    char* pLigne;

    if (pfichierTXT == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1;
    }
    fgets(ligne, 100, pfichierTXT);
    pLigne = ligne + 3;
    while (!feof(pfichierTXT)) {
        token = strtok(pLigne, "|");
        strcpy_s(livre.code, 11, token);
        token = strtok(NULL, "|");
        strcpy_s(livre.titre, 100, token);
        token = strtok(NULL, "|");
        strcpy_s(livre.auteur, 100, token);
        token = strtok(NULL, "|");
        livre.date = atoi(token);
        token = strtok(NULL, "|");
        strcpy_s(livre.editeur, 50, token);
        printf("Enregistrement : %s %s %s %d %s", livre.code, livre.titre, livre.auteur, livre.date, livre.editeur);
        fwrite(&livre, sizeof(livre), 1, pfichierDAT);
        nbEnregistrements++;
        fgets(ligne, 100, pfichierTXT);
        pLigne = ligne;
    }

    fclose(pfichierTXT);
    fclose(pfichierDAT);

    fopen_s(&pfichierDAT, "ouvrages.dat", "rb");
    fread_s(&livre, sizeof(livre), sizeof(livre), 1, pfichierDAT);
    while (!feof(pfichierDAT)) {
        printf("Verification : %s %s %s %d %s", livre.code, livre.titre, livre.auteur, livre.date, livre.editeur);
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