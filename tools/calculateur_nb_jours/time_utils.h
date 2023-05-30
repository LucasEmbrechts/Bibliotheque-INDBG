#pragma once
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdint.h>

void extraireDepuisDateYYYYMMDD(int date, int* year, int* month, int* day);
int calculerNbJours(int dateStartYYYYMMDD, int dateEndYYYYMMDD);
int obtenirDateYYYYMMDDDuJour(void);
/*
* Entrées :
*	- <dateStartYYYYMMDD> : un entier représentant une date au format YYYYMMDD
*	- <dateEndYYYYMMDD> : un entier représentant une date au format YYYYMMDD
* Sorties :
*	- un entier représentant le nombre de jours entre les deux dates
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


	extraireDepuisDateYYYYMMDD(dateStartYYYYMMDD, &extractedYear, &extractedMonth, &extractedDay);
	date1.tm_hour = 0;
	date1.tm_min = 0;
	date1.tm_sec = 0;
	date1.tm_mon = extractedMonth - 1;
	date1.tm_mday = extractedDay;
	date1.tm_year = extractedYear - 1900;


	extraireDepuisDateYYYYMMDD(dateEndYYYYMMDD, &extractedYear, &extractedMonth, &extractedDay);
	date2.tm_hour = 0;
	date2.tm_min = 0;
	date2.tm_sec = 0;
	date2.tm_mon = extractedMonth - 1;
	date2.tm_mday = extractedDay;
	date2.tm_year = extractedYear - 1900;

	seconds = difftime(mktime(&date2), mktime(&date1));

	return seconds / 86400;
}

int obtenirDateYYYYMMDDDuJour(void) {
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


/*
* Entrée: <date>, un entier représentant une date au format YYYYMMDD (ex. 20230405)
* Sorties :
*	- <year>	: l'année (ex. 2023)
*	- <month>	: le mois (ex. 4)
*	- <day>		: le jour (ex. 5)
*/
void extraireDepuisDateYYYYMMDD(int date, int* year, int* month, int* day) {
	// écrire du code ici...
}