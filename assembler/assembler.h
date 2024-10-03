#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define SIZE_OF_WORDS 1000  /*  size of words array  */
#define SIZE_OF_LINE 81  /*  size of line  */
#define FIRST_ADDRESS 100  /*  index of the first address  */
#define SIZE_OF_PAR 50  /*  size of parameter  */
#define SIZE_OF_FILE_NAME 33  /*  size of the given file name  */
#define MAX_SIZE_24_DIGITS 8388607  /*  max number in 24 digits  */
#define MIN_SIZE_24_DIGITS -8388608  /*  min number in 24 digits  */
#define MAX_SIZE_21_DIGITS 1048575  /*  max number in 21 digits  */
#define MIN_SIZE_21_DIGITS -1048576  /*  min number in 21 digits  */

/*  setting enums for the symbol's field 'type'  */
typedef enum {DATA = 0, CODE = 1, DATA_AND_ENTRY = 2, CODE_AND_ENTRY = 3,ENTRY = 4, EXTERNAL = 5, EXTERNAL_AND_ENTRY = 6}types;   


/*  pointer to linked list of symbols  */
typedef struct linked_symbols *symbol_pointer;

/*  linked list of symbols  */
typedef struct linked_symbols 
    {
        char label[32];
        int value;
        types type;
        symbol_pointer next;
    } symbol;

/*  function prototype  */

/****FIRST PASS****/
/*get_input.c*/
void first_pass(int *DC, int *IC, int *error_flag, FILE *input, symbol_pointer *head, unsigned long (*words_array)[], unsigned long (*DC_array)[]);
void get_par(char *line,char (*par1)[],char (*par2)[]);
int is_storage_line(char *par1, char *par2,int symbol_flag);
int is_empty_or_comment_line(char *line);
void get_three_par(char *line,char (*par1)[],char (*par2)[],char (*par3)[]);

/*get_word.c*/
void get_DC_words(unsigned long (*DC_array)[],int *DC,char *line, int symbol_flag, int *error_flag, int number_of_line);
void get_IC_words(unsigned long (*words_array)[],int *IC,char *line, int symbol_flag, int *error_flag, int number_of_line);

/*put_word.c*/
void put_num_to_DC(unsigned long (*DC_array)[], int *DC, int num);
void put_word_to_IC_array(unsigned long (*words_array)[], int *IC, char *par1, char *par2, char *par3);
void put_DC_array_to_words_array(unsigned long (*words_array)[], unsigned long (*DC_array)[], int IC, int DC);

/*symbol.c*/
void add_symbol(symbol_pointer *head, char *name, types type_value, int DC, int IC, int *error_flag, int number_of_line);
symbol_pointer find_last(symbol_pointer *head);
symbol_pointer already_exists(symbol_pointer *head, char *name);
void set_symbols(symbol_pointer *head,int IC);
void free_symbol(symbol_pointer *head);
int unvalid_name(char *name);

/*valid_word.c*/
int is_valid_words1(char *par1, char *par2, int *error_flag, int number_of_line);
int is_valid_words2(char *par1, char *par2, char *par3, int *error_flag, int number_of_line);
int is_number(char *par);
int is_valid_number(char *par);
int is_register(char *par);
int is_label(char *par);
int is_command(char *par);


/****SECOND PASS****/
/*second_pass.c*/
void second_pass(int *DC, int *IC, int *error_flag, FILE *input, symbol_pointer *head, unsigned long (*words_array)[], unsigned long (*DC_array)[], symbol_pointer *ext);
void get_parFourPars(char *line, char (*par1)[], char (*par2)[], char (*par3)[],char (*par4)[]);

/*update_obj.c*/
void updateSymbol(symbol_pointer *head, char *str1, char *str2, char *str3, int IC, int *cntLineCode, unsigned long (*words_array)[]);
int getOperationNumber(char *strOpr, char *str);
int getAddress(symbol_pointer *head, char *str);
void updateMachineCode(unsigned long (*words_array)[], int *cntLineCode, int max, int operation, int address);

/*put_extern.c*/
int updateExtern(symbol_pointer *head, char *str1, char *str2, int IC, int *cntLineCode, unsigned long (*words_array)[], symbol_pointer *ext, int *error_flag, int line);
int ixExtern(symbol_pointer *head, char *strPar, int *error_flag, int line);
void setMachineCode(unsigned long (*words_array)[], int *cntLineCode, int max);
int setToExtern(char *strVar, int *cntLineCode, symbol_pointer *ext);

/*build_files.c*/
void buildObjectFile(char *nameFile, unsigned long *words_array, int IC, int DC);
void buildEntriesFile(char *nameFile, symbol_pointer head);
void buildExternalsFile(char *nameFile, symbol_pointer ext);

