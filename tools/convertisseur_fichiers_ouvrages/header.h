#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct livre {
    char code[11];
    char titre[100];
    char auteur[100];
    int date;
    char editeur[50];
}Livre;
