#include "biblio.h"

void main(void){
    Livre l;
    strcpy(l.isbn, "2266111579");
    strcpy(l.titre, "Nostradamus");
    strcpy(l.auteur, "Joseph Conrad");
    l.anneeParution = 1903;
    strcpy(l.editeur, "Gallimard");
    modifierLivre(l);
}