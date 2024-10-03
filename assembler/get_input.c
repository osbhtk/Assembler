#include "assembler.h"

/*
*    first_pass:  this function receive 7 parameters and return nothing
*    the first parameter is int pointer to Data Counter, its increas each time a word is added to DC_array
*    the second parameter is int pointer to Instruction Counter, its increas each time a word is added to words_array
*    the third parameter is int pointer error_flag, its change to 1 if the program found an error
*    the fourth parameter is FILE pointer named input, this pointer point to the given assebmler file to be readed
*    the fifth parameter (named head) is pointer to pointer to symbol variable,
*    this pointer point to the first linked symbol's list (if its empty, its point to NULL)
*    the sixth parameter is pointer to array of unsigned long named words_array, words_array will save all the encode words by order
*    the seventh parameter is pointer to array of unsigned long named DC_array, DC_array will save all the data and string words
*    this function represent the first pass to initialize all the encode words in words_array and 'save space' for more words in the second pass
*    after initialize all the instruction words in words_array, all the data/string words will be added at the end of words_array
*    if this function found an error then there will not be a call to the  second pass
*/
void first_pass(int *DC,int *IC,int *error_flag,FILE *input,symbol_pointer *head,unsigned long (*words_array)[],unsigned long (*DC_array)[])
{
    char line[SIZE_OF_LINE];  /*  string that will save each line from input file  */
    char par1[SIZE_OF_PAR];  /*  string that hold the first parameter from line  */
    char par2[SIZE_OF_PAR];  /*  string that hold the second parameter from line  */
    char par3[SIZE_OF_PAR];  /*  string that hold the third parameter from line  */
    char tmp[SIZE_OF_PAR];  /*  temporary string to hold the label  */
    int symbol_flag;  /*  flag that reprecent if the line have or have not a symbol  */
    int number_of_line = 0;  /*  index to reprecent which line we read from input  */

    while (fgets(line, SIZE_OF_LINE, input))  /*  loop to read all the line from input  */
    {
        symbol_flag = 0;  /*  reset the symbol flag before/after using him  */
        number_of_line++;

        if ((line[0] != ';') && (is_empty_or_comment_line(line) == 0))  /*  not a comment line and not empty line  */
            {
            get_par(line, &par1, &par2);  /*  set the parameters from line in par1 and par2  */
            if (par1[strlen(par1)-1] == ':')  /*  first paramet is symbol  */
            {
                symbol_flag = 1;  /*  turn on flag  */
                par1[strlen(par1)-1] = '\0';  /*  change the last character ':' to end of string '\0'  */
            }
            if (is_storage_line(par1,par2,symbol_flag))  /*  the line have '.data' or '.string'  */
            {
                if(symbol_flag == 1)  /*  if have symbol, add the symbol to linked list of symbols  */
                {
                    add_symbol(head,par1,DATA,*DC,*IC,error_flag,number_of_line);
                }
                get_DC_words(DC_array, DC, line, symbol_flag, error_flag, number_of_line);  /*  add the data/string words to DC_array  */
            }
            else if (strcmp(par1,".extern") == 0)  /*  if its have external symbol, update it in symbol linked list  */
            {
                add_symbol(head,par2,EXTERNAL,*DC,*IC,error_flag,number_of_line);
            }
            else if (strcmp(par1,".entry") == 0)  /*  if its have entry symbol, need to be update it in symbol linked list at second loop  */
            {
                if (strcmp(par2,"") == 0)  /*  error  */
                {
                    fprintf(stderr, "Error in line %d, there is no label after '.entry'\n",number_of_line);
                    *error_flag = 1;
                }
            }
            else if (((symbol_flag == 1) && (strcmp(par2,".extern") == 0)) ||
                    ((symbol_flag == 1) && (strcmp(par2,".entry") == 0)))  /*  worrning but not an error  */
            {
                strcpy(tmp,par1);  /*  copy the label name into tmp  */
                get_three_par(line,&par1,&par2,&par3);  /*  skip the symbol and add the first 3 parameters into par1-par3  */
                fprintf(stdout,"Warning, in line %d the label '%s' is meaningless\n",number_of_line, tmp);
                if (strcmp(par1,".extern") == 0)  /*  if its have external symbol, update it in symbol linked list  */
                {
                    add_symbol(head,par2,EXTERNAL,*DC,*IC,error_flag,number_of_line);
                }
                if (strcmp(par1,".entry") == 0)  /*  if its have entry symbol, need to be update it in symbol linked list at second loop  */
                {
                    if (strcmp(par2,"") == 0)  /*  error  */
                    {
                        fprintf(stderr, "Error in line %d, there is no label after '.entry'\n",number_of_line);
                        *error_flag = 1;
                    }
                }
            }
            else  /* this is instruction line  */
            {
                if(symbol_flag == 1)  /*  if have symbol, add the symbol to linked list of symbols  */
                {
                    add_symbol(head,par1,CODE,*DC,*IC,error_flag,number_of_line);
                }
                get_IC_words(words_array, IC, line, symbol_flag, error_flag, number_of_line);  /*  add the encode words from current line to words_array  */
            }
        }
    }
    put_DC_array_to_words_array(words_array,DC_array, *IC, *DC);  /*  add all the DC_array words to the end of words_array  */
}


/*
*    get_par:  this function receive 3 parameters and return nothing
*    the first parameter is string named line, line have the current line from input file
*    the second parameter is pointer to string named par1, in par1 the function will save the first parameter from line 
*    the thirs parameter is pointer to string named par2, in par2 the function will save the second parameter from line 
*    this function receive string named line and save the first parameter fron line in par1 and second parameter from line in par2
*    if line have only 1 parameter its set par2 to empty string
*/
void get_par(char *line,char (*par1)[],char (*par2)[])
{
    char c;
    int j;
    int i = 0;
    c = line[i];
    while ((c == ' ') || (c == '\t'))  /*  skip spaces  */
    {
        c = line[++i];
    }
    j = 0;
    while (isspace(c) == 0)  /*  enter to par1 the first none spaces parameter  */
    {
        (*par1)[j++] = c;
        c = line[++i];
    }
    (*par1)[j] = '\0';  /*  set end of string  */
    while (isspace(c))  /*  skip spaces  */
    {
        c = line[++i];
    }
    if ((c == '\n') || (c == '\0') || (c == EOF))  /*  end of line  */
    {
        (*par2)[0] = '\0';  /*  set par2 to be empty string  */
        return;
    }
    j = 0;
    while (isspace(c) == 0)  /*  enter to par2 the second none spaces parameter  */
    {
        (*par2)[j++] = c;
        c = line[++i];
    }
    (*par2)[j] = '\0';  /*  set end of string  */
}

/*
*    is_storage_line:  this function receive 3 parameters and return int
*    the first parameter is string named par1, par1 have the first parameret from the current input line
*    the second parameter is string named par2, par2 have the second parameret from the current input line
*    the third parameter is int named symbol_flag, symbol_flag reprecent if the first parameter is symbol
*    the function cheack if the current line according to the 3 parameters is storage line or not
*    if the line is '.data' or '.string' then its storage line and return 1
*    if the line is not '.data' or '.string' then its not storage line and return 0
*/   
int is_storage_line(char *par1, char *par2,int symbol_flag)
{
    if (symbol_flag == 1)  /*  check par2  */
    {
        if ((strcmp(par2,".data") == 0) || (strcmp(par2,".string") == 0))  /*  cheack if storage line  */
        {
            return 1;  /*  it is storage line  */
        }
        return 0;  /*  it is not storage line  */
    }
    /*  symbol_flag != 1, check par1  */
    if ((strcmp(par1,".data") == 0) || (strcmp(par1,".string") == 0))  /*  cheack if storage line  */
    {
        return 1;  /*  it is storage line  */
    }
    return 0;  /*  it is not storage line  */
}

/*
*    is_empty_line:  this function receive 1 parameter and return int
*    the first parameter is string named line, line have the current input line
*    the function check if line is empty line
*    if line is empty line its return 1
*    if line is not empry line its return 0
*/
int is_empty_or_comment_line(char *line)
{
    int i = 0;  /*  set index for the first character in line  */
    int j = 0;  /*  set index for the first character in line  */
    char c = line[j];
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++j];
    }
    if (c == ';')  /*  comment line  */
    {
        return 1;
    }
    while ((line[i] != '\n') && (line[i] != '\0') && (line[i] != EOF))  /*  check all line  */
    {
        if (isspace(line[i]) == 0)  /*  not white space  */
        {
            return 0;
        }
        i++;
    }
    return 1;  /*  all line is white spaces  */
} 


/*
*    get _three_par:  this function receive 4 parameters and return nothing
*    the first parameter is string named line, line have the current line from the source file
*    the second parameter is string named par1, in par1 will be set the first parameter from line
*    the third parameter is string named par2, in par2 will be set the second parameter from line
*    the forth parameter is string named par3, in par3 will be set the third parameter from line
*    this function skip the first symbol in line (if there is) and then put the three first parameters in par1-par3
*    if there is only 1 parameter then par2 and par3 will be set to empty string
*    if there is only 2 parameters then par3 will be set to empty string
*/
void get_three_par(char *line,char (*par1)[],char (*par2)[],char (*par3)[])
{
    /*  two temporary string to hold the first two parameters from line include the symbol (if there is)  */
    char tmp1[SIZE_OF_PAR];
    char tmp2[SIZE_OF_PAR];
    char c;  /*  temporary character to hold the current character from line  */
    int i, j;  /*  indexs to read line and initialize it in par1-par2  */
    i = 0;  /*  set i for the beginning of the line  */
    c = line[i];  /*  set c for the first character  */

    get_par(line, &tmp1, &tmp2);  /*  set the parameters from line in par1 and par2  */
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    if (tmp1[strlen(tmp1)-1] == ':')  /*  first prameret is symbol, need to skip the symbol  */
    {
        while (c != ' ' && c != '\t')  /*  skip symbol  */
        {
             c = line[++i];
        }
    }
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    /*  found none spaces character  */
    j = 0;  /*  set j to the beginning of par1  */
    while (isspace(c) == 0 && c != EOF  && c != '\0' && c != ',')  /*  set first parameret from line inside par1 string  */
    {
        (*par1)[j++] = c;
        c = line[++i];
    }
    (*par1)[j] = '\0';  /*  set end of string  */
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    if (c == '\n' || c == EOF || c == '\0')  /*  finished initialize line  */
    {
        (*par2)[0]= '\0';  /*  set to empty string  */
        (*par3)[0]= '\0';  /*  set to empty string  */
        return;  /* finish successfully initialize line and update IC_array  */
    }
    /*  found none space character  */
    j = 0;  /*  set j to the beginning of par2  */
    while (isspace(c) == 0 && c != ',' && c != '\0' && c != EOF)  /*  set second parameret from line inside par2 string  */
    {
        (*par2)[j++] = c;
        c = line[++i];
    }
    (*par2)[j] = '\0';  /*  set end of string  */    
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    if (c == '\n' || c == EOF || c == '\0')  /*  finished initialize line  */
    {
        (*par3)[0] = '\0';  /*  set to empty string  */
        return;  /* finish initialize line and update IC_array  */
    }
    /*   c is ',' character  */
    c = line[++i];  /*  set c to the character after ','  */
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    /*  found none space character  */
    j = 0;
    while (isspace(c) == 0 && c != ',' && c != '\0' && c != EOF)  /*  set third parameret from line inside par3 string  */
    {
        (*par3)[j++] = c;
        c = line[++i];
    }
    (*par3)[j] = '\0';  /*  set end of string  */   
}

