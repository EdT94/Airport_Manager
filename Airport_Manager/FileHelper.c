#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileHelper.h"
#include "General.h"
#include "myMacros.h"


int	 writeStringToFile(char* str, FILE* fp, const char* msg)
{
	int length = strlen(str);
	length++;
	CHECK_0_MSG_COLSE_FILE(writeIntToFile(length, fp, msg), msg, fp);

	CHECK_0_MSG_COLSE_FILE(writeCharsToFile(str, length, fp, msg), msg, fp);

	return 1;
}

int	 writeCharsToFile(char* arr, int size, FILE* fp, const char* msg)
{
	if (fwrite(arr, sizeof(char), size, fp) != size)
	{
		MSG_CLOSE_RETURN_0(msg, fp);

	}
	return 1;

}

int	 writeIntToFile(int val, FILE* fp, const char* msg)
{
	CHECK_0_MSG_COLSE_FILE(fwrite(&val, sizeof(int), 1, fp), msg, fp);

	return 1;
}

char* readStringFromFile(FILE* fp, const char* msg)
{
	char* str;
	int length;
	CHECK_NULL__MSG_COLSE_FILE(readIntFromFile(&length, fp, msg), msg, fp);

	str = (char*)malloc((length) * sizeof(char));
	CHECK_NULL__MSG_COLSE_FILE(str, msg, fp);

	if (fread(str, sizeof(char), length, fp) != length)
	{

		puts(msg);
		fclose(fp);
		return NULL;
	}
	return str;

}

int	 readCharsFromFile(char* arr, int length, FILE* fp, const char* msg)
{
	if (fread(arr, sizeof(char), length, fp) != length)
	{
		MSG_CLOSE_RETURN_0(msg, fp);
	}
	return 1;
}

int	  readIntFromFile(int* pVal, FILE* fp, const char* msg)
{
	if (fread(pVal, sizeof(int), 1, fp) != 1)
	{
		MSG_CLOSE_RETURN_0(msg, fp);
	}
	return 1;

}