#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Company.h"
#include "Airport.h"
#include "General.h"
#include "myMacros.h"
#include "FileHelper.h"

#define DETAIL_PRINT

static const char* sortOptStr[eNofSortOpt] = {
	"None","Hour", "Date", "Airport takeoff code", "Airport landing code" };


int	initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName)
{
	L_init(&pComp->flighDateList);
	if (loadCompanyFromFile(pComp, pManaer, fileName))
	{
		initDateList(pComp);
		return 1;
	}
	return 0;
}


void	initCompany(Company* pComp, AirportManager* pManaer)
{
	printf("-----------  Init Airline Company\n");
	L_init(&pComp->flighDateList);
	pComp->name = getStrExactName("Enter company name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
}

void	initDateList(Company* pComp)
{
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isUniqueDate(pComp, i))
		{
			char* sDate = createDateString(&pComp->flightArr[i]->date);
			L_insert(&(pComp->flighDateList.head), sDate);
		}
	}
}

int		isUniqueDate(const Company* pComp, int index)
{
	Date* pCheck = &pComp->flightArr[index]->date;
	for (int i = 0; i < index; i++)
	{
		if (equalDate(&pComp->flightArr[i]->date, pCheck))
			return 0;
	}
	return 1;
}

int		addFlight(Company* pComp, const AirportManager* pManager)
{

	if (pManager->count < 2)
	{
		printf("There are not enoght airport to set a flight\n");
		return 0;
	}
	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	CHECK_RETURN_0(pComp->flightArr);

	pComp->flightArr[pComp->flightCount] = (Flight*)calloc(1, sizeof(Flight));
	if (!pComp->flightArr[pComp->flightCount])
		return 0;
	initFlight(pComp->flightArr[pComp->flightCount], pManager);
	if (isUniqueDate(pComp, pComp->flightCount))
	{
		char* sDate = createDateString(&pComp->flightArr[pComp->flightCount]->date);
		L_insert(&(pComp->flighDateList.head), sDate);
	}
	pComp->flightCount++;
	return 1;
}

void	printCompany(const Company* pComp)
{
	printf("\n");
#ifdef DETAIL_PRINT
	printf("%s", pComp->name);
	printSentence("Hachi", "Babit", "Ba", "Olam", NULL);
	printf("Has %d flights\n", pComp->flightCount);
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight**), printFlightV);
	printf("\nFlight Date List:");
	L_print(&pComp->flighDateList, printStr);
#else
	printf("%s Has %d flights\n", pComp->name, pComp->flightCount);
#endif
}

void printSentence(char* word, ...) {
	va_list allWords;
	char* currentWord;
	va_start(allWords, word);
	currentWord = word;
	while (currentWord != NULL) {
		printf("_%s", currentWord);
		currentWord = va_arg(allWords, char*);
	}
	printf("\n");
	va_end(allWords);
}

void	printFlightsCount(const Company* pComp)
{
	char codeOrigin[CODE_LENGTH + 1];
	char codeDestination[CODE_LENGTH + 1];

	if (pComp->flightCount == 0)
	{
		printf("No flight to search\n");
		return;
	}

	printf("Origin Airport\n");
	getAirportCode(codeOrigin);
	printf("Destination Airport\n");
	getAirportCode(codeDestination);

	int count = countFlightsInRoute(pComp->flightArr, pComp->flightCount, codeOrigin, codeDestination);
	if (count != 0)
		printf("There are %d flights ", count);
	else
		printf("There are No flights ");

	printf("from %s to %s\n", codeOrigin, codeDestination);
}


int		saveCompanyToFile(const Company* pComp, const char* fileName)
{
	FILE* f = fopen(fileName, "wb");
	if (!f) {
		return 0;
	}
	int flightCount = pComp->flightCount;
	int nameLength = strlen(pComp->name) + 1;
	int sorts = pComp->sortOpt;
	unsigned char* companyData = (char*)calloc(2, sizeof(char));
	companyData[0] = nameLength;
	companyData[0] |= sorts << 4;
	if (flightCount == 0) {
		companyData[0] |= 0;                              //companyData[0]: MSB=0 no flights
		companyData[1] = 0;                               //0 flights in companyData[1]
	}
	else {
		companyData[0] |= 128;                            //companyData[0] :MSB=1 there is flights ,128=10000000
		companyData[1] = flightCount;                     //num of flight in companyData[1]
	}
	fwrite(&companyData[0], sizeof(char), 1, f);
	fwrite(&companyData[1], sizeof(char), 1, f);
	fwrite(pComp->name, sizeof(char), nameLength, f);
	if (flightCount != 0) {
		for (int i = 0; i < pComp->flightCount; i++)
		{
			unsigned char* flightData = (char*)calloc(4, sizeof(char));
			Date date = pComp->flightArr[i]->date;
			flightData[0] = pComp->flightArr[i]->hour;
			flightData[0] |= date.day << 5;
			flightData[1] = date.day >> 3;
			flightData[1] |= date.month << 2;
			flightData[1] |= date.year << 6;
			flightData[2] = date.year >> 2;
			flightData[3] = date.year >> 10;
			fwrite(&flightData[0], sizeof(char), 1, f);
			fwrite(&flightData[1], sizeof(char), 1, f);
			fwrite(&flightData[2], sizeof(char), 1, f);
			fwrite(&flightData[3], sizeof(char), 1, f);
			fwrite(&pComp->flightArr[i]->originCode, sizeof(char), 4, f);
			fwrite(&pComp->flightArr[i]->destCode, sizeof(char), 4, f);
			free(flightData);
		}
	}
	free(companyData);
	fclose(f);
}

int loadCompanyFromFile(Company* pComp, const AirportManager* pManager, const char* fileName) {
	FILE* f = fopen(fileName, "rb");
	CHECK_MSG_RETURN_0(f, Error open company file);
	/*if (!f) {
		return 0;
	}*/
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	pComp->sortOpt = 0;
	L_init(&pComp->flighDateList);
	unsigned char* companyData = (char*)calloc(2, sizeof(char));
	unsigned char* nameLength = 0, sort = 0, temp = 7, flightCount, ifFlightCount;
	fread(&companyData[0], sizeof(char), 1, f);
	fread(&companyData[1], sizeof(char), 1, f);
	nameLength = companyData[0] << 4;
	nameLength = (char)nameLength >> 4;
	pComp->name = malloc(sizeof(char) * (char)nameLength);
	fread(pComp->name, sizeof(char), nameLength, f);
	sort = companyData[0] >> 4;
	sort = sort & temp;
	ifFlightCount = companyData[0] << 7;
	ifFlightCount = ifFlightCount >> 7;
	pComp->sortOpt = sort;
	if (ifFlightCount != 0) {         //there is flights
		flightCount = companyData[1];
		pComp->flightCount = flightCount;
		pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
		CHECK_NULL__MSG_COLSE_FILE(pComp->flightArr, Alocation error, f);
		for (int i = 0; i < pComp->flightCount; i++) {
			getFlightData(pComp, f, i);
		}
		initDateList(pComp);
		free(companyData);
	}
	fclose(f);
}

void getFlightData(Company* pComp, FILE* f, int i) {
	char codeOrigin[CODE_LENGTH + 1];
	char codeDest[CODE_LENGTH + 1];
	unsigned char* hour, temp = 0;
	unsigned char* flightData = (char*)calloc(4, sizeof(char));
	if (!flightData) {
		return 0;
	}
	pComp->flightArr[i] = (Flight*)calloc(1, sizeof(Flight));
	if (!pComp->flightArr[i])
	{
		return 0;
	}
	fread(&flightData[0], sizeof(char), 1, f);
	fread(&flightData[1], sizeof(char), 1, f);
	fread(&flightData[2], sizeof(char), 1, f);
	fread(&flightData[3], sizeof(char), 1, f);
	fread(&codeOrigin, sizeof(char), 4, f);
	fread(&codeDest, sizeof(char), 4, f);
	strcpy(pComp->flightArr[i]->originCode, codeOrigin);
	strcpy(pComp->flightArr[i]->destCode, codeDest);
	temp = flightData[0] << 3;
	hour = temp >> 3;
	pComp->flightArr[i]->hour = hour;
	getDateFromFlight(pComp, f, i, flightData);
	free(flightData);
}

void getDateFromFlight(Company* pComp, FILE* f, int i, unsigned char* flightData) {
	unsigned char* day, month, temp1, temp2;
	int year = 0, temp3, temp4;
	pComp->flightArr[i]->date;
	pComp->flightArr[i] = pComp->flightArr[i];
	temp1 = flightData[0] >> 5;
	temp2 = flightData[1] << 6;
	temp2 = temp2 >> 6;
	day = temp1 | (temp2 << 3);
	pComp->flightArr[i]->date.day = day;
	temp1 = 0;
	temp2 = 0;
	month = flightData[1] << 2;
	month = month >> 4;
	pComp->flightArr[i]->date.month = month;
	year = flightData[1] >> 6;
	temp3 = flightData[2] << 2;
	year = year | temp3;
	temp3 = 0;
	temp4 = flightData[3];
	temp4 = temp4 << 10;
	year |= temp4;
	pComp->flightArr[i]->date.year = year;
	temp4 = 0;
	temp3 = 0;
}
void	sortFlight(Company* pComp)
{
	pComp->sortOpt = showSortMenu();
	int(*compare)(const void* air1, const void* air2) = NULL;

	switch (pComp->sortOpt)
	{
	case eHour:
		compare = compareByHour;
		break;
	case eDate:
		compare = compareByDate;
		break;
	case eSorceCode:
		compare = compareByCodeOrig;
		break;
	case eDestCode:
		compare = compareByCodeDest;
		break;

	}

	if (compare != NULL)
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);

}

void	findFlight(const Company* pComp)
{
	int(*compare)(const void* air1, const void* air2) = NULL;
	Flight f = { 0 };
	Flight* pFlight = &f;


	switch (pComp->sortOpt)
	{
	case eHour:
		f.hour = getFlightHour();
		compare = compareByHour;
		break;
	case eDate:
		getchar();
		getCorrectDate(&f.date);
		compare = compareByDate;
		break;
	case eSorceCode:
		getchar();
		getAirportCode(f.originCode);
		compare = compareByCodeOrig;
		break;
	case eDestCode:
		getchar();
		getAirportCode(f.destCode);
		compare = compareByCodeDest;
		break;
	}

	if (compare != NULL)
	{
		Flight** pF = bsearch(&pFlight, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
		if (pF == NULL)
			printf("Flight was not found\n");
		else {
			printf("Flight found, ");
			printFlight(*pF);
		}
	}
	else {
		printf("The search cannot be performed, array not sorted\n");
	}

}

eSortOption showSortMenu()
{
	int opt;
	printf("Base on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt >eNofSortOpt);

	return (eSortOption)opt;
}

void	freeCompany(Company* pComp)
{
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight**), freeFlight);
	free(pComp->flightArr);
	free(pComp->name);
	L_free(&pComp->flighDateList, freePtr);
}
