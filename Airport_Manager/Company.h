#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"
#include "GeneralList.h"

typedef enum { eNone, eHour, eDate, eSorceCode, eDestCode, eNofSortOpt } eSortOption;
static const char* sortOptStr[eNofSortOpt];


typedef struct
{
	char* name;
	int			flightCount;
	Flight** flightArr;
	eSortOption	sortOpt;
	LIST		flighDateList;
}Company;

int		initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName);

int		isUniqueDate(const Company* pComp, int index);

int		addFlight(Company* pComp, const AirportManager* pManager);

int		saveCompanyToFile(const Company* pComp, const char* fileName);

int		loadCompanyFromFile(Company* pComp, const AirportManager* pManager, const char* fileName);

void	initCompany(Company* pComp, AirportManager* pManaer);

void	initDateList(Company* pComp);

void	printCompany(const Company* pComp);

void	printFlightsCount(const Company* pComp);

void getDateFromFlight(Company* pComp, FILE* f, int i, unsigned char* flightData);

void getFlightData(Company* pComp, FILE* f, int i);

void	sortFlight(Company* pComp);

void	findFlight(const Company* pComp);

void	freeCompany(Company* pComp);

void printSentence(char* word, ...);

eSortOption showSortMenu();
#endif

