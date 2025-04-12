//#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define TAILLE_ISBN 11
#define TAILLE_TITRE 100
#define TAILLE_AUTEUR 100
#define TAILLE_EDITEUR 50
#define TAILLE_NOM 100
#define TAILLE_PRENOM 100
#define TAILLE_ADRESSE 100

#define NOM_FICHIER_EMPRUNTS "emprunts.txt"
#define NOM_FICHIER_MEMBRES "membres.txt"
#define NOM_FICHIER_LIVRES "ouvrages.txt"




struct livre {
    char isbn[TAILLE_ISBN];
    char titre[TAILLE_TITRE];
    char auteur[TAILLE_AUTEUR];
    int anneeParution;
    char editeur[TAILLE_EDITEUR];
};

typedef struct livre Livre;

struct emprunt {
    int dateEmprunt;
    int numMembre;
    char isbn[TAILLE_ISBN];
};

typedef struct emprunt Emprunt;

struct membre {
    int numMembre;
    char nom[TAILLE_NOM];
    char prenom[TAILLE_PRENOM];
    char adresse[TAILLE_ADRESSE];
    int dateAdhesion;
};
typedef struct membre Membre;

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

/**
 * Obtient l'année courante
 * @return L'année actuelle du système
 */
int obtenirAnneeActuelle(void) {
    time_t now;
    struct tm current_time;
    time(&now);
    current_time = *localtime(&now);

    return current_time.tm_year + 1900;
}

bool ouvertureFichiers(void) {


    // Fichier Emprunts
    FILE* pTabEmprunts =  fopen(NOM_FICHIER_EMPRUNTS, "r+");
    if (pTabEmprunts == NULL) {

        pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "a+");
        fclose(pTabEmprunts);
        pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "r+");
    }
    fclose(pTabEmprunts);

    // Fichiers Membres
    FILE* pTabMembres = fopen(NOM_FICHIER_MEMBRES, "r+");
    if (pTabMembres == NULL) {
        pTabMembres = fopen(NOM_FICHIER_MEMBRES, "a+");
        fclose(pTabMembres);
        pTabMembres = fopen(NOM_FICHIER_EMPRUNTS, "r+");

    }
    fclose(pTabMembres);


    // Fichiers Livres
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

    // Livre invalide par défaut
    memset(&livreBD, 0, sizeof(Livre));
    strcpy(livreBD.isbn, "");
    pTabLivres = fopen(NOM_FICHIER_LIVRES, "r");
    if (pTabLivres == NULL) {
        return livreBD;
    }
    fgets(ligne, sizeof(ligne), pTabLivres);
    pLigne = ligne;
    while (!feof(pTabLivres)) {

        // Séparer les champs
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

        // Comparaison de l'ISBN recherché
        if (strcmp(livreBD.isbn, isbnRecherche) == 0) {
            fclose(pTabLivres);
            return livreBD;
        }
        fgets(ligne, sizeof(ligne), pTabLivres);
        pLigne = ligne;
    }

    fclose(pTabLivres);

    // Si non trouvé
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

    // Lire chaque ligne du fichier original
    while (fgets(ligne, sizeof(ligne), pTabLivres)) {
        char isbnLigne[TAILLE_ISBN];
        sscanf(ligne, "%[^|]", isbnLigne); // Extraire l'ISBN de la ligne

        // Si l'ISBN ne correspond pas, écrire la ligne dans le fichier temporaire
        if (strcmp(isbnLigne, isbn) != 0) {
            fputs(ligne, pTemp);
        } else {
            livreSupprime = true; // Marquer que le livre a été supprimé
        }
    }

    fclose(pTabLivres);
    fclose(pTemp);

    // Remplacer le fichier original par le fichier temporaire
    if (livreSupprime) {
        if (remove(NOM_FICHIER_LIVRES) != 0 || rename("temp.txt", NOM_FICHIER_LIVRES) != 0) {
            return false;
        }
    } else {
        // Si aucun livre n'a été supprimé, supprimer le fichier temporaire
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

    // Lire chaque ligne du fichier original
    while (fgets(ligne, sizeof(ligne), pTabLivres)) {
        char isbnLigne[TAILLE_ISBN];
        sscanf(ligne, "%[^|]", isbnLigne); // Extraire l'ISBN de la ligne

        // Si l'ISBN correspond, écrire les nouvelles données
        if (strcmp(isbnLigne, livre.isbn) == 0) {
            fprintf(pTemp, "%s|%s|%s|%d|%s\n",
                    livre.isbn,
                    livre.titre,
                    livre.auteur,
                    livre.anneeParution,
                    livre.editeur);
            livreModifie = true;
        } else {
            // Sinon, copier la ligne telle quelle
            fputs(ligne, pTemp);
        }
    }

    fclose(pTabLivres);
    fclose(pTemp);

    // Remplacer le fichier original par le fichier temporaire
    if (livreModifie) {
        if (remove(NOM_FICHIER_LIVRES) != 0 || rename("temp.txt", NOM_FICHIER_LIVRES) != 0) {
            return false;
        }
    } else {
        // Si aucun livre n'a été modifié, supprimer le fichier temporaire
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
Emprunt obtenirEmprunt(char isbn[], int numMembre) {
    FILE* pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "r+");
    Emprunt empruntBD;

    // Initialisation d'un emprunt "invalide"
    memset(&empruntBD, 0, sizeof(empruntBD));
    strcpy(empruntBD.isbn, ""); // ISBN vide pour indiquer une erreur
    empruntBD.numMembre = 0;

    if (pTabEmprunts == NULL) {
        return empruntBD;
    }

    while (fread(&empruntBD, sizeof(empruntBD), 1, pTabEmprunts) != 0) {
        if (strcmp(isbn, empruntBD.isbn) == 0 && numMembre == empruntBD.numMembre) {
            fclose(pTabEmprunts);
            return empruntBD;
        }
    }

    // Réinitialiser à un emprunt "invalide"
    memset(&empruntBD, 0, sizeof(empruntBD));
    strcpy(empruntBD.isbn, "");
    empruntBD.numMembre = 0;

    fclose(pTabEmprunts);
    return empruntBD;
}

/**
 * Insère un nouvel emprunt dans le fichier des emprunts
 * @param empruntAjout L'emprunt à ajouter
 * @return true si l'insertion a réussi, false sinon
 */
bool insererEmprunt(Emprunt empruntAjout) {
    FILE* pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "a+");

    if (pTabEmprunts == NULL) {
        return false;
    }

    fwrite(&empruntAjout, sizeof(empruntAjout), 1, pTabEmprunts);
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
    FILE* pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "r+");
    Emprunt empruntBD;

    if (pTabEmprunts == NULL) {
        return false;
    }

    while (fread(&empruntBD, sizeof(empruntBD), 1, pTabEmprunts) != 0) {
        if (strcmp(isbn, empruntBD.isbn) == 0 && numMembre == empruntBD.numMembre) {
            break;
        }
    }

    if (feof(pTabEmprunts)) {
        fclose(pTabEmprunts);
        return false;
    }

    // Marquer l'emprunt comme supprimé
    strcpy(empruntBD.isbn, "***");
    empruntBD.numMembre = -1;

    fseek(pTabEmprunts, -1 * (long)sizeof(empruntBD), SEEK_CUR);
    fwrite(&empruntBD, sizeof(empruntBD), 1, pTabEmprunts);
    fclose(pTabEmprunts);
    return true;
}

/**
 * Modifie un emprunt dans le fichier des emprunts
 * @param emprunt L'emprunt modifié
 * @return true si la modification a réussi, false sinon
 */
bool modifierEmprunt(Emprunt emprunt) {
    FILE* pTabEmprunts = fopen(NOM_FICHIER_EMPRUNTS, "r+");
    Emprunt empruntBD;

    if ( pTabEmprunts == NULL) {
        return false;
    }

    while (fread(&empruntBD, sizeof(empruntBD), 1, pTabEmprunts) != 0) {
        if (strcmp(emprunt.isbn, empruntBD.isbn) == 0 && emprunt.numMembre == empruntBD.numMembre) {
            break;
        }
    }

    if (feof(pTabEmprunts)) {
        fclose(pTabEmprunts);
        return false;
    }

    fseek(pTabEmprunts, -1 * (long)sizeof(empruntBD), SEEK_CUR);
    fwrite(&emprunt, sizeof(emprunt), 1, pTabEmprunts);
    fclose(pTabEmprunts);
    return true;
}


/**
 * Obtient un membre à partir de son numéro
 * @param numMembre Le numéro du membre à chercher
 * @return Le membre trouvé ou un membre "invalide" si non trouvé
 */
Membre obtenirMembre(int numMembre) {
    FILE* pTabMembres = fopen(NOM_FICHIER_MEMBRES, "r+");
    Membre membreBD;

    // Initialisation d'un membre "invalide"
    memset(&membreBD, 0, sizeof(membreBD));
    membreBD.numMembre = 0;
    strcpy(membreBD.nom, "");

    if (pTabMembres != NULL) {
        return membreBD;
    }

    while (fread(&membreBD, sizeof(membreBD), 1, pTabMembres) != 0 && membreBD.numMembre != numMembre);

    if (feof(pTabMembres)) {
        fclose(pTabMembres);
        // Réinitialiser à un membre "invalide"
        memset(&membreBD, 0, sizeof(membreBD));
        membreBD.numMembre = 0;
        strcpy(membreBD.nom, "");
        return membreBD;
    }

    fclose(pTabMembres);
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

    fwrite(&membreAjout, sizeof(membreAjout), 1, pTabMembres);
    fclose(pTabMembres);
    return true;
}

/**
 * Supprime un membre du fichier des membres
 * @param numMembre Le numéro du membre à supprimer
 * @return true si la suppression a réussi, false sinon
 */
bool supprimerMembre(int numMembre) {
    FILE* pTabMembres = fopen(NOM_FICHIER_MEMBRES, "r+");
    Membre membreBD;

    if (pTabMembres == NULL) {
        return false;
    }

    while (fread(&membreBD, sizeof(membreBD), 1, pTabMembres) != 0 && membreBD.numMembre != numMembre);

    if (feof(pTabMembres)) {
        fclose(pTabMembres);
        return false;
    }

    strcpy(membreBD.nom, "***");
    fseek(pTabMembres, -1 * (long)sizeof(membreBD), SEEK_CUR);
    fwrite(&membreBD, sizeof(membreBD), 1, pTabMembres);
    fclose(pTabMembres);
    return true;
}

/**
 * Modifie un membre dans le fichier des membres
 * @param membre Le membre modifié (identifié par numMembre)
 * @return true si la modification a réussi, false sinon
 */
bool modifierMembre(Membre membre) {
    FILE* pTabMembres = fopen(NOM_FICHIER_MEMBRES, "r+");
    Membre membreBD;

    if (pTabMembres == NULL) {
        return false;
    }

    while (fread(&membreBD, sizeof(membreBD), 1, pTabMembres) != 0 && membreBD.numMembre != membre.numMembre);

    if (feof(pTabMembres)) {
        fclose(pTabMembres);
        return false;
    }

    fseek(pTabMembres, -1 * (long)sizeof(membreBD), SEEK_CUR);
    fwrite(&membre, sizeof(membre), 1, pTabMembres);
    fclose(pTabMembres);
    return true;
}