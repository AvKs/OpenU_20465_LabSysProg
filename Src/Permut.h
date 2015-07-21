/*
Author:		Avi Karmi
Date:		21.06.2015
Desciption: Performs pre processor operations of removing comments, import include files and replace MACROs in C file.
*/


#ifndef MAIN
#define MAIN

#define TRUE						1
#define FALSE						0
#define OK							0
#define PHASE1_EXT					"c1 "
#define PHASE2_EXT					"c2 "
#define PHASE3_EXT					"c3 "
#define PHASE3_TMP_EXT				"tmp"
#define READ						"r"
#define WRITE						"w"
#define READ_AND_WRITE				"w+"
#define MAX_LINE					255
#define	EXTENSION_LENGTH			4
#define FILE_MAX_NAME_LEN			25

#define ERROR_MSG_CANT_OPEN_FILE	"Cannot open file '%s', terminating the program.\n\n"
#define ERROR_MSG_CANT_DEL_FILE		"Unable to delete the file '%s'. \n\n"

#define INCLUDE_STR_SEARCH			"#include"
#define DEFINE_STR_SEARCH			"#define"

enum status {OUT, IN_STRING, LEFT_SLASH, IN_COMMENT, RIGHT_STAR, IN_CPP_COM};

void buildNewFileName(char * strToBuild, char * fileExtension);
void removeComments(FILE * inFile, FILE * outFile);
unsigned int importInclude(FILE * inFile, FILE * outFile);
char * extractStrBetweenChars(char *strToExtractFrom, char chrBegin, char chrEnd, unsigned int spaceTillEndOfLine);
void fileCopy(FILE * inFile, FILE * outFile);
void replaceDefine(FILE * inFile, FILE * outFile);
char * findAndReplace(char * origStr, char * replaceStr, char * withStr);

#endif
