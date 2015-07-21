/*
Author:		Anastasia Vasilzhenko && Avi Karmi
Date:		19.07.2015
Description: 
*/

#include <stdio.h>
#include <string.h>
#include "Main.h"


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
		inFile = openFileToRead(fileNameWithExt);
		if (inFile)
		{
			parseAsmFile(inFile);
			createObjOutput();
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


FILE * openFileToRead(char * fileToRead)
{
	FILE *inFile;
	
	if ((inFile = fopen(*fileToRead, READ_FILE)) == NULL)												/* Open c file for reading */
	{		
		return (NULL);
	}
	return (inFile);
}