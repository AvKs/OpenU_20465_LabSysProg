/*
Author:		Avi Karmi
Date:		21.06.2015
Desciption: Performs pre processor operations of removing comments, import include files and replace MACROs in C file.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "Permut.h"


int main(int numArg, char * argName[])
{
	FILE *inFile, *outFile;
	char tempForDelFileName[FILE_MAX_NAME_LEN] = { 0 };										
	char fileExtension[EXTENSION_LENGTH] = { 0 };
	unsigned int errorFlag;
	
	if (numArg == 1)																			/* Check for only one argument */
	{
		return (!OK);
	}
	
	argName++;																					
	
	if ((inFile = fopen(*argName, READ)) == NULL)												/* Open c file for reading */
	{
		printf(ERROR_MSG_CANT_OPEN_FILE, *argName);
		return (!OK);
	}

	strcpy(fileExtension, PHASE1_EXT);
	buildNewFileName(*argName, fileExtension);													/* Construct C1 file name */
	if ((outFile = fopen(*argName, WRITE)) == NULL)												/* Open c1 file for writing */
	{
		printf(ERROR_MSG_CANT_OPEN_FILE, *argName);
		fclose(inFile);
		return (!OK);
	}
	
	removeComments(inFile, outFile);															/* Remove comments in file */
	fclose(inFile);
	fclose(outFile);

	/* --- End of phase 1 --- */

	if ((inFile = fopen(*argName, READ)) == NULL)												/* Open c1 file for reading */
	{
		printf(ERROR_MSG_CANT_OPEN_FILE, *argName);
		return (!OK);
	}

	strcpy(fileExtension, PHASE2_EXT);															
	buildNewFileName(*argName, fileExtension);													/* Construct C2 file name */

	if ((outFile = fopen(*argName, WRITE)) == NULL)												/* Open c2 file for writing */
	{
		printf(ERROR_MSG_CANT_OPEN_FILE, *argName);
		fclose(inFile);
		return (!OK);
	}

	errorFlag = importInclude(inFile, outFile);													/* Import all include files */
	fclose(inFile);
	fclose(outFile);

	if (errorFlag)																				/* Exit with error if include file not found */
	{
		return (!OK);
	}

	/* --- End of phase 2 --- */

	if ((inFile = fopen(*argName, READ)) == NULL)												/* Open c2 file for reading */
	{
		printf(ERROR_MSG_CANT_OPEN_FILE, *argName);
		return (!OK);
	}

	strcpy(fileExtension, PHASE3_TMP_EXT);
	buildNewFileName(*argName, fileExtension);													/* Construct temp file */
	strcpy(tempForDelFileName, *argName);

	if ((outFile = fopen(*argName, READ_AND_WRITE)) == NULL)									/* Open c3 tmp file for reading and writing */
	{
		printf(ERROR_MSG_CANT_OPEN_FILE, *argName);
		fclose(inFile);
		return (!OK);
	}

	fileCopy(inFile, outFile);																	/* Copy C2 to temp file */
	fclose(inFile);
	rewind(outFile);
	inFile = outFile;																			/* Make temp file as input */

	strcpy(fileExtension, PHASE3_EXT);
	buildNewFileName(*argName, fileExtension);													/* Construct C3 file name */

	if ((outFile = fopen(*argName, READ_AND_WRITE)) == NULL)									/* Open c3 file for writing */
	{
		printf(ERROR_MSG_CANT_OPEN_FILE, *argName);
		fclose(inFile);
		return (!OK);
	}

	replaceDefine(inFile, outFile);																/* Replace defines (MACROS) */

	fclose(inFile);
	if (remove(tempForDelFileName))																/* Delete temp file */
	{
		printf(ERROR_MSG_CANT_DEL_FILE, tempForDelFileName);
	}
	fclose(outFile);
	return (OK);
}


/* Constructs new file using string of new extension */
void buildNewFileName(char * strToBuild, char * fileExtension)
{
	char *strPntr;

	strPntr = strchr(strToBuild, '.');															
	*(strPntr + 1) = *fileExtension;															/* Construct extension from string */
	*(strPntr + 2) = *(fileExtension + 1);
	*(strPntr + 3) = *(fileExtension + 2);
	*(strPntr + 4) = '\0';
}


/* Copies file without comments */
void removeComments(FILE * inFile, FILE * outFile)
{
	char inputChr;
	int state = OUT;
	
	while ((inputChr = getc(inFile)) != EOF)									
	{
		switch (state)
		{
			case (OUT) :																		/* Not in comment */
				if (inputChr == '/')
				{
					state = LEFT_SLASH;
				}
				else
				{
					putc(inputChr, outFile);
					if (inputChr == '\"')
					{
						state = IN_STRING;
					}
				}
				break;
			
			case (LEFT_SLASH) :																	/* In left slash of comment */
				if (inputChr == '*')
				{
					state = IN_COMMENT;
				}
				else if (inputChr == '/')
				{
					state = IN_CPP_COM;
				}
				else
				{
					putc('/', outFile);
					putc(inputChr, outFile);
					state = OUT;
				}
				break;

			case (IN_COMMENT) :																	/* In comments state */
				if (inputChr == '*')
				{
					state = RIGHT_STAR;
				}
				break;

			case (IN_CPP_COM) :																	/* In C++ comment */
				if (inputChr == '\n')
				{
					state = OUT;
					putc('\n', outFile);
				}
				break;

			case (RIGHT_STAR) :																	/* Check if comment finialize  */
				if (inputChr == '/')
				{
					state = OUT;
				}
				else 
				{
					state = IN_COMMENT;
				}
				break;

			case (IN_STRING) :																	/* In string state  */
				if (inputChr == '\"')
				{
					state = OUT;
				}
				putc(inputChr, outFile);
				break;
		}
	}		
}


unsigned int importInclude(FILE * inFile, FILE * outFile)
{
	char line[MAX_LINE];

	while ((fgets(line, MAX_LINE, inFile)) != NULL)
	{
		char *enterPtr = strchr(line, '\n');													/* Find '\n'  */
		char *includePtr = strstr(line, INCLUDE_STR_SEARCH);									/* Find include str  */

		if (enterPtr)
		{
			*enterPtr = '\0';
		}

		if (includePtr)
		{
			FILE *includeFile;
			char *includeFileName = extractStrBetweenChars(includePtr, '\"', '\"', FALSE);		/* extract include file name  */

			if (includeFileName)
			{
				if ((includeFile = fopen(includeFileName, READ)) == NULL)						/* Open include file for reading */
				{
					printf(ERROR_MSG_CANT_OPEN_FILE, includeFileName);
					return (!OK);
				}

				fileCopy(includeFile, outFile);													/* Copy include into C2 */
				fclose(includeFile);
				free(includeFileName);
			}	
		}
		else
		{
			enterPtr = strchr(line, '\0');														/* Change string end to enter */
			if (enterPtr)
			{
				*enterPtr = '\n';
			}

			fputs(line, outFile);
		}
	}
	return (OK);
}


char * extractStrBetweenChars(char *strToExtractFrom, char chrBegin, char chrEnd, unsigned int spaceTillEndOfLine)
{
	char *result;
	char strCopyExtract[MAX_LINE] = { 0 };
	unsigned int chrBeginIndx = 0, chrEndIndx, len, spaceCounter = 0, spaceDuplicateAvoidFlag = FALSE, foundNotSpaceChr = FALSE;
	
	strcpy(strCopyExtract, strToExtractFrom);
	
	if (chrBegin == '\t')																		/* Hande tabs as spaces */
	{
		chrBegin == ' ';
	}
	if (chrEnd == '\t')
	{
		chrEnd == ' ';
	}

	while ((strCopyExtract[chrBeginIndx] != chrBegin) && (strCopyExtract[chrBeginIndx] != '\0') && (!spaceDuplicateAvoidFlag))
	{
		chrBeginIndx++;
		
		if (strCopyExtract[chrBeginIndx] == '\t')												/* Change tabs to spaces */
		{
			strCopyExtract[chrBeginIndx] = ' ';
		}

		if ((chrBegin == ' ') && (strCopyExtract[chrBeginIndx - 1] == ' ') && (strCopyExtract[chrBeginIndx] != ' '))
		{
			spaceDuplicateAvoidFlag = TRUE;														/* Ignore multiple spaces */
			chrBeginIndx--;
		}
	}

	if (strCopyExtract[chrBeginIndx] == '\0')
	{
		return (NULL);
	}

	chrBeginIndx++;
	chrEndIndx = chrBeginIndx;
	spaceDuplicateAvoidFlag = FALSE;

	while ((strCopyExtract[chrEndIndx] != chrEnd) && (strCopyExtract[chrEndIndx] != '\0') && (!spaceDuplicateAvoidFlag))
	{
		chrEndIndx++;

		if (strCopyExtract[chrEndIndx] == '\t')
		{
			strCopyExtract[chrEndIndx] = ' ';
		}

		if ((chrEnd == ' ') && (strCopyExtract[chrEndIndx - 1] == ' '))							/* find ending index */
		{
			if (!spaceTillEndOfLine)															/* Check if ignore multiple spaces*/
			{
				spaceDuplicateAvoidFlag = TRUE;
				chrEndIndx--;
			}
			else
			{
				unsigned int i = chrEndIndx;

				while (strCopyExtract[i] != '\0') 
				{
					if (strCopyExtract[i] != ' ')
					{
						foundNotSpaceChr = TRUE;
					}
				}

				if (!foundNotSpaceChr)
				{
					spaceDuplicateAvoidFlag = TRUE;
					chrEndIndx--;
				}
			}
		}
	}

	if (strCopyExtract[chrEndIndx] == '\0')
	{
		return (NULL);
	}
	chrEndIndx--;
	
	len = chrEndIndx - chrBeginIndx + 1;
	result = (char*)malloc(sizeof(char)*(len + 1));
	strncpy(result, strCopyExtract + chrBeginIndx, len);									/* construct string result */
	result[len] = '\0';
	return (result);
}


/* Performs file copy */
void fileCopy(FILE * inFile, FILE * outFile)
{
	int inputChr;

	while ((inputChr = getc(inFile)) != EOF)
	{
		putc(inputChr, outFile);
	}
}



void replaceDefine(FILE * inFile, FILE * outFile)
{
	char line[MAX_LINE];

	while ((fgets(line, MAX_LINE, inFile)) != NULL)
	{
		char *enterPtr = strchr(line, '\n');
		char *definePtr = strstr(line, DEFINE_STR_SEARCH);
		char *removedDefineStr, *defineStrToSearch, *defineStrToReplace, *resultOfDefineReplace;

		if (enterPtr)
		{
			*enterPtr = '\0';
		}

		if (definePtr)																		/* Check if found define */
		{
			removedDefineStr = findAndReplace(definePtr, DEFINE_STR_SEARCH, "");			/* Erase define */
			
			defineStrToSearch = extractStrBetweenChars(definePtr, ' ', ' ', FALSE);			/* string to replace */
			defineStrToReplace = extractStrBetweenChars(removedDefineStr + strlen(defineStrToSearch) + 1, ' ', ' ', TRUE);	/* string which replaces */
			
			enterPtr = strchr(line, '\0');
			if (enterPtr)
			{
				*enterPtr = '\n';	
			}
			fputs(removedDefineStr, outFile);
					
			rewind(inFile);
			rewind(outFile);
			while ((fgets(line, MAX_LINE, inFile)) != NULL)
			{
				char *definePtrStrToReplace = strchr(line, defineStrToSearch);

				if (definePtrStrToReplace)
				{
					resultOfDefineReplace = findAndReplace(line, defineStrToSearch, defineStrToReplace);
					enterPtr = strchr(line, '\0');
					if (enterPtr)
					{
						*enterPtr = '\n';
					}
					fputs(resultOfDefineReplace, outFile);
				}
			}
		}
		else
		{
			enterPtr = strchr(line, '\0');
			if (enterPtr)
			{
				*enterPtr = '\n';
			}
			fputs(line, outFile);
		}
	}
}


/* Finds string in string and replaces it */
char * findAndReplace(char * origStr, char * replaceStr, char * withStr)
{
	char *result;																			/* The return string */
	char *ins;																				/* The next insert point */
	char *tmp;																				/* Varies */
	int len_replace;																		/* Length of replace */
	int len_with;																			/* Length of with */
	int len_front;																			/* Distance between replace and end of last replace */
	int count;																				/* Number of replacements */

	if (!origStr)
	{
		return (NULL);
	}

	if (!replaceStr)
	{
		replaceStr = "";
	}

	len_replace = strlen(replaceStr);

	if (!withStr)
	{
		withStr = "";
	}

	len_with = strlen(withStr);
	ins = origStr;

	for (count = 0; tmp = strstr(ins, replaceStr); ++count)
	{
		ins = tmp + len_replace;
	}

	result = malloc(strlen(origStr) + (len_with - len_replace) * count + 1);
	tmp = result;

	if (!result)
	{
		return (NULL);
	}

	while (count--)
	{
		ins = strstr(origStr, replaceStr);
		len_front = ins - origStr;
		tmp = strncpy(tmp, origStr, len_front) + len_front;
		tmp = strcpy(tmp, withStr) + len_with;
		origStr += len_front + len_replace;
	}

	strcpy(tmp, origStr);
	return (result);
}
