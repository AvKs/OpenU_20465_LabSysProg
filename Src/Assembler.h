/*
Author:		Anastasia Vasilzhenko && Avi Karmi
Date:		19.07.2015
Description:
*/


#ifndef MAIN
#define MAIN

/* === Numbers Definition === */
#define OK							0
#define EXT_LEN						3
#define REG_NUM						8
#define MEM_LOC_NUM					1000
#define MAX_CHARS_IN_LINE			255

/* === Strings Definition === */
#define FILE_EXT_ASSEMBLY			"as "
#define FILE_EXT_OBJECT				"ob "
#define FILE_EXT_ENTRIES			"ent"
#define FILE_EXT_EXTERNALS			"ext"
#define READ_FILE					"r "

/* === Error Messages Definition === */
#define ERR_MSG_CANT_OPEN_FILE		"Unable to open file '%s', terminating the program.\n"
#define ERR_MSG_NO_OPER_SRC			"No such source operand"
#define ERR_MSG_NO_OPER_DEST		"No such destination operand"
#define ERR_MSG_ILLEGAL_OPCODE		"Ileggal opcode"
#define ERR_MSG_ILLEGAL_REG			"Ileggal register"
#define ERR_MSG_ILLEGAL_LABEL		"Ileggal label"
#define ERR_MSG_SYNTAX_ERROR		"Global syntax error"

/* === Typedef && Enum === */
enum boolean{ FALSE=0, TRUE=1 };
enum opcode_type{ mov = 0, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, red, prn, jsr, rts, stop };
enum addressing_type{ A = 0, E, R };
typedef enum{ blank, comment, operation, direction } statement_type;
enum direction_type{ data, string, extrn, entry, code, relocatable };

/* === Structures === */
struct SymbolTable
{
	char *name;
	int value;
	int type;
};

/* === Function Prototype === */
char * addExtensionToFile(char * fileName, char extToAdd[]);
FILE * getFileAccess(char * fileToOper, char operation[]);
void parseInputFile(FILE *inFile);
enum statement_type identifyLineType(line);

#endif



