#ifndef __AIR_MANAGER__
#define __AIR_MANAGER__

#include "Airport.h"
#include "GeneralList.h"

#define ERROR 0
#define FROM_FILE 1
#define FROM_USER 2

typedef struct
{
	LIST		airportList;
	int			count;
}AirportManager;

int		initManager(AirportManager* pManager, const char* fileName);

int		 addAirport(AirportManager* pManager);

int		checkUniqeCode(const char* code, const AirportManager* pManager);

int		saveManagerToFile(const AirportManager* pManager, const char* fileName);

int		loadManagerFromFile(AirportManager* pManager, const char* fileName);

void	insertPortToListSorted(LIST* airportList, Airport* pPort);

void	printAirports(const AirportManager* pManager);

void	freeManager(AirportManager* pManager);

Airport* createAirport(AirportManager* pManager);

Airport* findAirportByCode(const AirportManager* pManager, const char* code);


#endif