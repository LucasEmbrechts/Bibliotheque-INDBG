#pragma once
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdint.h>

void extractFromYYYYMMDD(int date, int* year, int* month, int* day);
int calculateNbDays(int dateStartYYYYMMDD, int dateEndYYYYMMDD);

/*
* Entr�es :
*	- <dateStartYYYYMMDD> : un entier repr�sentant une date au format YYYYMMDD
*	- <dateEndYYYYMMDD> : un entier repr�sentant une date au format YYYYMMDD
* Sorties :
*	- un entier repr�sentant le nombre de jours entre les deux dates
*/
int calculateNbDays(int dateStartYYYYMMDD, int dateEndYYYYMMDD) {
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


	extractFromYYYYMMDD(dateStartYYYYMMDD, &extractedYear, &extractedMonth, &extractedDay);
	date1.tm_hour = 0;
	date1.tm_min = 0;
	date1.tm_sec = 0;
	date1.tm_mon = extractedMonth - 1;
	date1.tm_mday = extractedDay;
	date1.tm_year = extractedYear - 1900;


	extractFromYYYYMMDD(dateEndYYYYMMDD, &extractedYear, &extractedMonth, &extractedDay);
	date2.tm_hour = 0;
	date2.tm_min = 0;
	date2.tm_sec = 0;
	date2.tm_mon = extractedMonth - 1;
	date2.tm_mday = extractedDay;
	date2.tm_year = extractedYear - 1900;

	seconds = difftime(mktime(&date2), mktime(&date1));

	return seconds / 86400;
}


/*
* Entr�e: <date>, un entier repr�sentant une date au format YYYYMMDD (ex. 20230405)
* Sorties :
*	- <year>	: l'ann�e (ex. 2023)
*	- <month>	: le mois (ex. 4)
*	- <day>		: le jour (ex. 5)
*/
void extractFromYYYYMMDD(int date, int* year, int* month, int* day) {
	// �crire du code ici...
}