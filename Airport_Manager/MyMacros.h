#ifndef _MYMACROS_H_
#define _MYMACROS_H_


//AirportManager.c:addAirport,checkUniqeCode     Company.c:addFlight
#define CHECK_RETURN_0(x) ((x)==NULL)? 0:1                                               


//AirportManager.c:createAirport      
#define CHECK_RETURN_NULL(x) ((x)==NULL)?  NULL:1                                             


//AirportManager.c:saveManagerToFile,loadManagerFromFile   Company.c:loadCompanyFromFile
#define	CHECK_MSG_RETURN_0(x,msg)  if((x)==NULL) {printf(#msg "\n");return 0; }          


//fileHelper.c:writeStringToFile,writeIntToFile
#define CHECK_0_MSG_COLSE_FILE(x,msg,f) if((x)==0){printf(#msg "\n");fclose(f);return 0;} 


//fileHelper.c:readStringFromFile     Company.c:loadCompanyFromFile
#define CHECK_NULL__MSG_COLSE_FILE(x,msg,f)   if((x)==NULL){printf(#msg "\n");fclose(f);return 0;} 


//fileHelper.c:writeCharsToFile,readCharsFromFile,readIntFromFile
#define MSG_CLOSE_RETURN_0(msg,f) {printf(#msg "\n");fclose(f);return 0;}


#endif _MYMACROS_H_
