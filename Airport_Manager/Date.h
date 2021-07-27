#ifndef __MYDATE__
#define __MYDATE__

#define MIN_YEAR 2020

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

int		checkDate(char* date, Date* pDate);

int		 equalDate(const Date* pDate1, const Date* pDate2);

char* createDateString(const Date* pDate);

void	getCorrectDate(Date* pDate);

void	printDate(const Date* pDate);



#endif
