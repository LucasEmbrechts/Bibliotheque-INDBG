#pragma once

/*
* Gestion d'une bibliothèque
* Ce fichier contient les définitions de structures et de fonctions
* pour gérer les livres, les emprunts et les membres d'une bibliothèque.
* Il inclut des fonctions pour ouvrir des fichiers, obtenir des livres,
* insérer, supprimer et modifier des livres, emprunts et membres,
* ainsi que des fonctions utilitaires pour manipuler des dates.
* @author Lucas Embrechts
* @date 2025-04-13
* @note Les fichiers utilisés sont : emprunts.txt, membres.txt et ouvrages.txt.
* @note Le fichier ouvrages.txt est structuré de la manière suivante :
*       isbn|titre|auteur|anneeParution|editeur
* @note Le fichier emprunts.txt est structuré de la manière suivante :
*       isbn|numMembre|dateEmprunt
* @note Le fichier membres.txt est structuré de la manière suivante :
*       numMembre|nomPrenom|adresse|dateAdhesion
* @warning Les fichiers doivent être encodés au format UTF-8 sans BOM.
* @warning Le fichier ouvrages.txt doit terminer par une nouvelle ligne.
*/

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>


#define TAILLE_ISBN 11

#define NOM_FICHIER_EMPRUNTS "emprunts.txt"
#define NOM_FICHIER_MEMBRES "membres.txt"
#define NOM_FICHIER_LIVRES "ouvrages.txt"


// Insérer les structures ici


bool ouvertureFichiers(void);
int obtenirAnneeActuelle(void);
Livre obtenirLivre(char isbn[]);
bool insererLivre(Livre livreAjout);
bool supprimerLivre(char isbn[]);
bool modifierLivre(Livre livre);
Emprunt obtenirEmprunt(char isbn[], int numMembre);
bool insererEmprunt(Emprunt empruntAjout);
bool supprimerEmprunt(char isbn[], int numMembre);
bool modifierEmprunt(Emprunt emprunt);
Membre obtenirMembre(int numMembre);
bool insererMembre(Membre membreAjout);
bool supprimerMembre(int numMembre);
bool modifierMembre(Membre membre);

int calculerNbJours(int dateStartYYYYMMDD, int dateEndYYYYMMDD);
int extraireJourDDDepuisDateYYYYMMDD(int date);
int extraireMoisMMDepuisDateYYYYMMDD(int date);
int extraireAnneeYYYYDepuisDateYYYYMMDD(int date);


bool ouvertureFichiers(void) {


    FILE* pTabEmprunts =  fopen(NOM_FICHIER_EMPRUNTS, "r+");
    if (pTabEmprunts == NULL) {

        pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "a+");
        fclose(pTabEmprunts);
        pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "r+");
    }
    fclose(pTabEmprunts);

    FILE* pTabMembres = fopen(NOM_FICHIER_MEMBRES, "r+");
    if (pTabMembres == NULL) {
        pTabMembres = fopen(NOM_FICHIER_MEMBRES, "a+");
        fclose(pTabMembres);
        pTabMembres = fopen(NOM_FICHIER_EMPRUNTS, "r+");

    }
    fclose(pTabMembres);


    FILE* pTabLivres = fopen(NOM_FICHIER_LIVRES, "r+");
    if (pTabLivres == NULL) {
        printf("Fichier Introuvable");
        system("pause");
        return false;
    }
    fclose(pTabLivres);
    return true;
};

/**
 * Rempli un tableau de livres à partir du fichier des livres
 * @param livres un tableau de livres vide
 */
void obtenirListeLivres(Livre livres[]) {
    FILE* pTabLivres;
    Livre livreBD;
    char ligne[256];
    char* token;
    char* pLigne;
    int iLivre = 0;

    memset(&livreBD, 0, sizeof(Livre));
    strcpy(livreBD.isbn, "");
    pTabLivres = fopen(NOM_FICHIER_LIVRES, "r");
    if (pTabLivres == NULL) {
        return livreBD;
    }
    fgets(ligne, sizeof(ligne), pTabLivres);
    pLigne = ligne;
    while (!feof(pTabLivres)) {

        token = strtok(pLigne, "|");
        strcpy(livreBD.isbn, token);

        token = strtok(NULL, "|");
        strcpy(livreBD.titre, token);

        token = strtok(NULL, "|");
        strcpy(livreBD.auteur, token);

        token = strtok(NULL, "|");
        livreBD.anneeParution = atoi(token);

        token = strtok(NULL, "|");
        strcpy(livreBD.editeur, token);

        livres[iLivre] = livreBD;
        iLivre++;
        fgets(ligne, sizeof(ligne), pTabLivres);
        pLigne = ligne;
    }

    fclose(pTabLivres);

    memset(&livreBD, 0, sizeof(Livre));
    strcpy(livreBD.isbn, "");
    return livreBD;
}


/**
 * Obtient un emprunt à partir de l'ISBN et du numéro de membre
 * @param isbn L'ISBN du livre recherché
 * @return Le livre recherché
 */
Livre obtenirLivre(char isbnRecherche[]) {
    FILE* pTabLivres;
    Livre livreBD;
    char ligne[256];
    char* token;
    char* pLigne;

    memset(&livreBD, 0, sizeof(Livre));
    strcpy(livreBD.isbn, "");
    pTabLivres = fopen(NOM_FICHIER_LIVRES, "r");
    if (pTabLivres == NULL) {
        return livreBD;
    }
    fgets(ligne, sizeof(ligne), pTabLivres);
    pLigne = ligne;
    while (!feof(pTabLivres)) {

        token = strtok(pLigne, "|");
        strcpy(livreBD.isbn, token);

        token = strtok(NULL, "|");
        strcpy(livreBD.titre, token);

        token = strtok(NULL, "|");
        strcpy(livreBD.auteur, token);

        token = strtok(NULL, "|");
        livreBD.anneeParution = atoi(token);

        token = strtok(NULL, "|");
        strcpy(livreBD.editeur, token);

        if (strcmp(livreBD.isbn, isbnRecherche) == 0) {
            fclose(pTabLivres);
            return livreBD;
        }
        fgets(ligne, sizeof(ligne), pTabLivres);
        pLigne = ligne;
    }

    fclose(pTabLivres);

    memset(&livreBD, 0, sizeof(Livre));
    strcpy(livreBD.isbn, "");
    return livreBD;
}

/**
 * Obtient un emprunt à partir de l'ISBN et du numéro de membre
 * @param livreAjout Le livre à ajouter
 * @return true si l'insertion a réussi, false sinon
 */
bool insererLivre(Livre livreAjout) {
    FILE* pTabLivres = fopen(NOM_FICHIER_LIVRES, "a+");

    if (pTabLivres == NULL) {
        return false;
    }
    fprintf(pTabLivres, "%s|%s|%s|%d|%s\n",
        livreAjout.isbn,
        livreAjout.titre,
        livreAjout.auteur,
        livreAjout.anneeParution,
        livreAjout.editeur);
    fclose(pTabLivres);
    return true;
}

/**
 * Supprime un emprunt du fichier des emprunts
 * @param isbn L'ISBN du livre emprunté
 * @return true si la suppression a réussi, false sinon
 */
bool supprimerLivre(char isbn[]) {
    FILE* pTabLivres = fopen(NOM_FICHIER_LIVRES, "r+");
    FILE* pTemp = fopen("temp.txt", "w");
    char ligne[256];
    bool livreSupprime = false;

    if (pTabLivres == NULL || pTemp == NULL) {
        if (pTabLivres) fclose(pTabLivres);
        if (pTemp) fclose(pTemp);
        return false;
    }

    while (fgets(ligne, sizeof(ligne), pTabLivres)) {
        char isbnLigne[TAILLE_ISBN];
        sscanf(ligne, "%[^|]", isbnLigne);

        if (strcmp(isbnLigne, isbn) != 0) {
            fputs(ligne, pTemp);
        } else {
            livreSupprime = true;
        }
    }

    fclose(pTabLivres);
    fclose(pTemp);

    if (livreSupprime) {
        if (remove(NOM_FICHIER_LIVRES) != 0 || rename("temp.txt", NOM_FICHIER_LIVRES) != 0) {
            return false;
        }
    } else {
        remove("temp.txt");
    }

    return livreSupprime;
}
/**
 * Modifie un emprunt dans le fichier des emprunts
 * @param livre le livre modifié
 * @return true si la modification a réussi, false sinon
 */
bool modifierLivre(Livre livre) {
    FILE* pTabLivres = fopen(NOM_FICHIER_LIVRES, "r+");
    FILE* pTemp = fopen("temp.txt", "w");
    char ligne[256];
    bool livreModifie = false;

    if (pTabLivres == NULL || pTemp == NULL) {
        if (pTabLivres) fclose(pTabLivres);
        if (pTemp) fclose(pTemp);
        return false;
    }

    while (fgets(ligne, sizeof(ligne), pTabLivres)) {
        char isbnLigne[TAILLE_ISBN];
        sscanf(ligne, "%[^|]", isbnLigne);

        if (strcmp(isbnLigne, livre.isbn) == 0) {
            fprintf(pTemp, "%s|%s|%s|%d|%s\n",
                    livre.isbn,
                    livre.titre,
                    livre.auteur,
                    livre.anneeParution,
                    livre.editeur);
            livreModifie = true;
        } else {
            fputs(ligne, pTemp);
        }
    }

    fclose(pTabLivres);
    fclose(pTemp);

    if (livreModifie) {
        if (remove(NOM_FICHIER_LIVRES) != 0 || rename("temp.txt", NOM_FICHIER_LIVRES) != 0) {
            return false;
        }
    } else {
        remove("temp.txt");
    }

    return livreModifie;
}

/**
 * Obtient un emprunt à partir de l'ISBN et du numéro de membre
 * @param isbn L'ISBN du livre emprunté
 * @param numMembre Le numéro du membre qui a emprunté le livre
 * @return L'emprunt trouvé ou un emprunt "invalide" si non trouvé
 */
Emprunt obtenirEmprunt(char isbnRecherche[], int numMembreRecherche) {
    FILE* pTabEmprunts;
    Emprunt empruntBD;
    char ligne[256];
    char* token;
    char* pLigne;

    memset(&empruntBD, 0, sizeof(Emprunt));
    strcpy(empruntBD.isbn, "");
    empruntBD.numMembre = -1;

    pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "r");
    if (pTabEmprunts == NULL) {
        return empruntBD;
    }

    fgets(ligne, sizeof(ligne), pTabEmprunts);
    pLigne = ligne;
    while (!feof(pTabEmprunts)) {
        token = strtok(pLigne, "|");
        strcpy(empruntBD.isbn, token);

        token = strtok(NULL, "|");
        empruntBD.numMembre = atoi(token);

        token = strtok(NULL, "|");
        empruntBD.dateEmprunt = atoi(token);

        if (strcmp(empruntBD.isbn, isbnRecherche) == 0 && empruntBD.numMembre == numMembreRecherche) {
            fclose(pTabEmprunts);
            return empruntBD;
        }

        fgets(ligne, sizeof(ligne), pTabEmprunts);
        pLigne = ligne;
    }

    fclose(pTabEmprunts);

    memset(&empruntBD, 0, sizeof(Emprunt));
    strcpy(empruntBD.isbn, "");
    empruntBD.numMembre = -1;
    return empruntBD;
}

/**
 * Insère un nouvel emprunt dans le fichier des emprunts
 * @param empruntAjout L'emprunt à ajouter
 * @return true si l'insertion a réussi, false sinon
 */
bool insererEmprunt(Emprunt empruntAjout) {
    FILE* pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "a");

    if (pTabEmprunts == NULL) {
        return false;
    }

    fprintf(pTabEmprunts, "%s|%d|%d\n",
            empruntAjout.isbn,
            empruntAjout.numMembre,
            empruntAjout.dateEmprunt);

    fclose(pTabEmprunts);
    return true;
}

/**
 * Supprime un emprunt du fichier des emprunts
 * @param isbn L'ISBN du livre emprunté
 * @param numMembre Le numéro du membre qui a emprunté le livre
 * @return true si la suppression a réussi, false sinon
 */
bool supprimerEmprunt(char isbn[], int numMembre) {
    FILE* pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "r");
    FILE* pTemp = fopen("temp.txt", "w");
    char ligne[256];
    bool empruntSupprime = false;

    if (pTabEmprunts == NULL || pTemp == NULL) {
        if (pTabEmprunts) fclose(pTabEmprunts);
        if (pTemp) fclose(pTemp);
        return false;
    }

    while (fgets(ligne, sizeof(ligne), pTabEmprunts)) {
        char isbnLigne[TAILLE_ISBN];
        int numMembreLigne;
        sscanf(ligne, "%[^|]|%d|", isbnLigne, &numMembreLigne);

        if (strcmp(isbnLigne, isbn) != 0 || numMembreLigne != numMembre) {
            fputs(ligne, pTemp);
        } else {
            empruntSupprime = true;
        }
    }

    fclose(pTabEmprunts);
    fclose(pTemp);

    if (empruntSupprime) {
        if (remove(NOM_FICHIER_EMPRUNTS) != 0 || rename("temp.txt", NOM_FICHIER_EMPRUNTS) != 0) {
            return false;
        }
    } else {
        remove("temp.txt");
    }

    return empruntSupprime;
}

/**
 * Modifie un emprunt dans le fichier des emprunts
 * @param emprunt L'emprunt modifié
 * @return true si la modification a réussi, false sinon
 */
bool modifierEmprunt(Emprunt emprunt) {
    FILE* pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "r");
    FILE* pTemp = fopen("temp.txt", "w");
    char ligne[256];
    bool empruntModifie = false;

    if (pTabEmprunts == NULL || pTemp == NULL) {
        if (pTabEmprunts) fclose(pTabEmprunts);
        if (pTemp) fclose(pTemp);
        return false;
    }

    while (fgets(ligne, sizeof(ligne), pTabEmprunts)) {
        char isbnLigne[TAILLE_ISBN];
        int numMembreLigne;
        sscanf(ligne, "%[^|]|%d|", isbnLigne, &numMembreLigne);

        if (strcmp(isbnLigne, emprunt.isbn) == 0 && numMembreLigne == emprunt.numMembre) {
            fprintf(pTemp, "%s|%d|%d\n",
                    emprunt.isbn,
                    emprunt.numMembre,
                    emprunt.dateEmprunt);
            empruntModifie = true;
        } else {
            fputs(ligne, pTemp);
        }
    }

    fclose(pTabEmprunts);
    fclose(pTemp);

    if (empruntModifie) {
        if (remove(NOM_FICHIER_EMPRUNTS) != 0 || rename("temp.txt", NOM_FICHIER_EMPRUNTS) != 0) {
            return false;
        }
    } else {
        remove("temp.txt");
    }

    return empruntModifie;
}

/**
 * Obtient un membre à partir de son numéro
 * @param numMembreRecherche Le numéro du membre à chercher
 * @return Le membre trouvé ou un membre "invalide" si non trouvé
 */
Membre obtenirMembre(int numMembreRecherche) {
    FILE* pTabMembres;
    Membre membreBD;
    char ligne[256];
    char* token;
    char* pLigne;

    memset(&membreBD, 0, sizeof(Membre));
    membreBD.numMembre = -1;
    pTabMembres = fopen(NOM_FICHIER_MEMBRES, "r");
    if (pTabMembres == NULL) {
        return membreBD;
    }

    fgets(ligne, sizeof(ligne), pTabMembres);
    pLigne = ligne;
    while (!feof(pTabMembres)) {
        token = strtok(pLigne, "|");
        membreBD.numMembre = atoi(token);

        token = strtok(NULL, "|");
        strcpy(membreBD.nomPrenom, token);

        token = strtok(NULL, "|");
        strcpy(membreBD.adresse, token);

        token = strtok(NULL, "|");
        membreBD.dateAdhesion = atoi(token);

        if (membreBD.numMembre == numMembreRecherche) {
            fclose(pTabMembres);
            return membreBD;
        }

        fgets(ligne, sizeof(ligne), pTabMembres);
        pLigne = ligne;
    }

    fclose(pTabMembres);

    memset(&membreBD, 0, sizeof(Membre));
    membreBD.numMembre = -1;
    return membreBD;
}

/**
 * Insère un nouveau membre dans le fichier des membres
 * @param membreAjout Le membre à ajouter
 * @return true si l'insertion a réussi, false sinon
 */
bool insererMembre(Membre membreAjout) {
    FILE* pTabMembres = fopen(NOM_FICHIER_MEMBRES, "a+");

    if (pTabMembres == NULL) {
        return false;
    }

    fprintf(pTabMembres, "%d|%s|%s|%d\n",
            membreAjout.numMembre,
            membreAjout.nomPrenom,
            membreAjout.adresse,
            membreAjout.dateAdhesion);

    fclose(pTabMembres);
    return true;
}

/**
 * Supprime un membre du fichier des membres
 * @param numMembre Le numéro du membre à supprimer
 * @return true si la suppression a réussi, false sinon
 */
bool supprimerMembre(int numMembre) {
    FILE* pTabMembres = fopen(NOM_FICHIER_MEMBRES, "r");
    FILE* pTemp = fopen("temp.txt", "w");
    char ligne[256];
    bool membreSupprime = false;

    if (pTabMembres == NULL || pTemp == NULL) {
        if (pTabMembres) fclose(pTabMembres);
        if (pTemp) fclose(pTemp);
        return false;
    }

    while (fgets(ligne, sizeof(ligne), pTabMembres)) {
        int numMembreLigne;
        sscanf(ligne, "%d|", &numMembreLigne);

        if (numMembreLigne != numMembre) {
            fputs(ligne, pTemp);
        } else {
            membreSupprime = true;
        }
    }

    fclose(pTabMembres);
    fclose(pTemp);

    if (membreSupprime) {
        if (remove(NOM_FICHIER_MEMBRES) != 0 || rename("temp.txt", NOM_FICHIER_MEMBRES) != 0) {
            return false;
        }
    } else {
        remove("temp.txt");
    }

    return membreSupprime;
}

/**
 * Modifie un membre dans le fichier des membres
 * @param membre Le membre modifié (identifié par numMembre)
 * @return true si la modification a réussi, false sinon
 */
bool modifierMembre(Membre membre) {
    FILE* pTabMembres = fopen(NOM_FICHIER_MEMBRES, "r");
    FILE* pTemp = fopen("temp.txt", "w");
    char ligne[256];
    bool membreModifie = false;

    if (pTabMembres == NULL || pTemp == NULL) {
        if (pTabMembres) fclose(pTabMembres);
        if (pTemp) fclose(pTemp);
        return false;
    }

    while (fgets(ligne, sizeof(ligne), pTabMembres)) {
        int numMembreLigne;
        sscanf(ligne, "%d|", &numMembreLigne);

        if (numMembreLigne == membre.numMembre) {
            fprintf(pTemp, "%d|%s|%s|%d\n",
                    membre.numMembre,
                    membre.nomPrenom,
                    membre.adresse,
                    membre.dateAdhesion);
            membreModifie = true;
        } else {
            fputs(ligne, pTemp);
        }
    }

    fclose(pTabMembres);
    fclose(pTemp);

    if (membreModifie) {
        if (remove(NOM_FICHIER_MEMBRES) != 0 || rename("temp.txt", NOM_FICHIER_MEMBRES) != 0) {
            return false;
        }
    } else {
        remove("temp.txt");
    }

    return membreModifie;
}




/**
 * Calcule le nombre de jours entre deux dates au format YYYYMMDD
 * @param dateStartYYYYMMDD La date de début au format YYYYMMDD
 * @param dateEndYYYYMMDD La date de fin au format YYYYMMDD
 * @return Le nombre de jours entre les deux dates
 */
int calculerNbJours(int dateStartYYYYMMDD, int dateEndYYYYMMDD) {
	time_t now;
	struct tm date1;
	struct tm date2;
	double seconds;
	int extractedDay;
	int extractedMonth;
	int extractedYear;

	time(&now);

	date1 = *localtime(&now);
	date2 = *localtime(&now);


    extractedYear = extraireAnneeYYYYDepuisDateYYYYMMDD(dateStartYYYYMMDD);
    extractedMonth = extraireMoisMMDepuisDateYYYYMMDD(dateStartYYYYMMDD);
    extractedDay = extraireJourDDDepuisDateYYYYMMDD(dateStartYYYYMMDD);
	date1.tm_hour = 0;
	date1.tm_min = 0;
	date1.tm_sec = 0;
	date1.tm_mon = extractedMonth - 1;
	date1.tm_mday = extractedDay;
	date1.tm_year = extractedYear - 1900;


    extractedYear = extraireAnneeYYYYDepuisDateYYYYMMDD(dateEndYYYYMMDD);
    extractedMonth = extraireMoisMMDepuisDateYYYYMMDD(dateEndYYYYMMDD);
    extractedDay = extraireJourDDDepuisDateYYYYMMDD(dateEndYYYYMMDD);
	date2.tm_hour = 0;
	date2.tm_min = 0;
	date2.tm_sec = 0;
	date2.tm_mon = extractedMonth - 1;
	date2.tm_mday = extractedDay;
	date2.tm_year = extractedYear - 1900;

	seconds = difftime(mktime(&date2), mktime(&date1));

	return seconds / 86400;
}

/**
 * Donne la date actuelle du système au format YYYYMMDD
 * @return La date actuelle au format YYYYMMDD
 */
int obtenirDateYYYYMMDDDuSysteme(void) {
	time_t now;
	struct tm *current_date;
	char buffer[9];
	int dateYYYYMMDD;

	time(&now);
	current_date = localtime(&now);
	strftime(buffer, 9, "%Y%m%d", current_date);
	dateYYYYMMDD = atoi(buffer);
	return dateYYYYMMDD;
}


/**
 * Extrait l'année d'une date au format YYYYMMDD
 * @param date La date au format YYYYMMDD
 * @return L'année extraite
 */
int extraireAnneeYYYYDepuisDateYYYYMMDD(int date) {
	return date / 10000;
}

/**
 * Extrait le mois d'une date au format YYYYMMDD
 * @param date La date au format YYYYMMDD
 * @return Le mois extrait
 */
int extraireMoisMMDepuisDateYYYYMMDD(int date) {
	int year = date / 10000;
	return (date - (year * 10000)) / 100;

}

/**
 * Extrait le jour d'une date au format YYYYMMDD
 * @param date La date au format YYYYMMDD
 * @return Le jour extrait
 */
int extraireJourDDDepuisDateYYYYMMDD(int date) {
	int year = date / 10000;
	int month = (date - (year * 10000)) / 100;
	return (date - (year * 10000) - month * 100);
}