/*
Author:		Anastasia Vasilzhenko && Avi Karmi
Date:		19.07.2015
Description: 
*/

#include <stdio.h>
#include <string.h>
#include "Assembler.h"


int main(int argc, char * argv[])
{
	char *fileNameWithExt;
	FILE *inFile;

	if (argc == 1)																			/* Check for only one argument */
	{
		return (!OK);
	}
	
	while (argc > 1)
	{
		fileNameWithExt = addExtensionToFile(argv[argc], FILE_EXT_ASSEMBLY);
		inFile = getFileAccess(fileNameWithExt, READ_FILE);
		if (inFile)
		{
			parseInputFile(inFile);
			createObjOutput();
			createEntOutput();
			createExtOutput();
		}
		else
		{
			printf(ERR_MSG_CANT_OPEN_FILE, *inFile);
		}
		--argc;
	}

	return (OK);
}


char * addExtensionToFile(char * fileName, char extToAdd[])
{
	char *newFileName;

	strcpy(newFileName, fileName);
	strcpy(newFileName, extToAdd);
	return (newFileName);
}


FILE * getFileAccess(char * fileToOper, char operation[])
{
	FILE *opFile;
	
	if ((opFile = fopen(*fileToOper, operation)) == NULL)												/* Open c file for reading */
	{		
		return (NULL);
	}
	return (opFile);
}


void parseInputFile(FILE *inFile)
{
	char line[MAX_CHARS_IN_LINE];
	statement_type lineType;
	unsigned int DC = 0, IC = 0;

	while ((fgets(line, MAX_CHARS_IN_LINE, inFile)) != NULL)
	{
		lineType = identifyLineType(line);

		switch (lineType)
		{
		case (blank) :
			break;
		
		case (comment) :
			break;

		case (operation) :
			break;

		case (direction) :
			break;
		}
	}
}


statement_type identifyLineType(line)
{
	char *endLinePtr;

	if (endLinePtr = strchr(line, '\n'))
	{
		*endLinePtr = '\0';
	}




	if (endLinePtr = strchr(line, '\0'))
	{
		*endLinePtr = '\n';
	}
}