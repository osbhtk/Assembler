#include "assembler.h"
/*
*    This file has two functions:
*    1) function is 'second_pass' get parameters for checking, updating machine code, and create list with extern parameters
*    2) function is 'get_parFourPars' get a line and cut to four strings
*/

/* main function to checking and updating machine code and create list with extern parameters */
void second_pass(int *DC,int *IC,int *error_flag,FILE *input, symbol_pointer *head,unsigned long (*words_array)[],unsigned long (*DC_array)[], symbol_pointer *ext){
    char line[SIZE_OF_LINE];  /*  string that will save each line from input file  */
    char par1[SIZE_OF_PAR];  /*  string that hold the first parameter from line  */
    char par2[SIZE_OF_PAR];  /*  string that hold the second parameter from line  */
    char par3[SIZE_OF_PAR];  /*  string that hold the third parameter from line  */
    char par4[SIZE_OF_PAR];  /*  string that hold the fourth parameter from line  */
    
    int symbol_flag;  /*  flag that reprecent if the line have or have not a symbol  */
    int number_of_line = 0;  /*  index to reprecent which line we read from input  */
    
    int cntLineCode = 0;  /*count for machine code*/
    
    /*loop to read all the line from input*/
    while (fgets(line, SIZE_OF_LINE, input)){
        /*reset the symbol flag before/after using him*/
        symbol_flag = 0;  
        number_of_line++;
        
        /*not a comment line and not empty line*/
        if ((line[0] != ';') && (is_empty_or_comment_line(line) == 0)){
            get_parFourPars(line, &par1, &par2, &par3, &par4);
            
            /*first paramet is symbol SKIP IT*/
            if (par1[strlen(par1)-1] == ':'){
                strcpy(par1, par2);
                strcpy(par2, par3);
                strcpy(par3, par4);
                symbol_flag = 1;                                
            }
            
            /*if the line have '.data' or '.string' or '.extern' go to NEXT LINE*/
            if (strcmp(par1, ".data") != 0 && strcmp(par1, ".string") != 0 && strcmp(par1, ".extern") != 0){
                /*if its '.entry' add entry*/
                /*if its not '.entry' update encoding*/
                if (strcmp(par1,".entry") == 0){
                    add_symbol(head, par2, ENTRY, *DC, *IC, error_flag, number_of_line);
                }else{
                    if(updateExtern(head, par2, par3, *IC, &cntLineCode, words_array, ext, error_flag, number_of_line) != 1){
                        updateSymbol(head, par1, par2, par3, *IC, &cntLineCode, words_array);
                    }
                }
            }
            
            /*worrning but not an error*/
            if (((symbol_flag == 1) && (strcmp(par2,".extern") == 0)) || ((symbol_flag == 1) && (strcmp(par2,".entry") == 0))){
                fprintf(stdout,"Warning, in line %d the label '%s' is meaningless\n",number_of_line, par1);
            }              
        }
    }
}


/* cut a line to four strings */
void get_parFourPars(char *line, char (*par1)[], char (*par2)[], char (*par3)[],char (*par4)[]){
    char ch;
    int i = 0, j = 0;
        
    (*par1)[0] = '\0';
    (*par2)[0] = '\0';
    (*par3)[0] = '\0';
    (*par4)[0] = '\0';
    
    ch = line[i];
    
    /*  skip spaces  */
    while ((ch == ' ') || (ch == '\t')){
        ch = line[++i];
    }
    j = 0;

    /*  enter to par1 the first none spaces parameter  */
    while (isspace(ch) == 0){    
        (*par1)[j++] = ch;
        ch = line[++i];
    }
    (*par1)[j] = '\0';  /*  set end of string  */
   
    /*  skip spaces  */
    while (isspace(ch)){
        ch = line[++i];
    }
    
    /*  end of line  */
    if ((ch == '\n') || (ch == '\0') || (ch == EOF)){
        (*par2)[0] = '\0';  /*  set par2 to be empty string  */
        return;
    }
    j = 0;
    /*  enter to par2 the second none spaces parameter  */
    while (isspace(ch) == 0 && ch != ','){  
        (*par2)[j++] = ch;
        ch = line[++i];
    }
    (*par2)[j] = '\0';  /*  set end of string  */    
    
    /*  skip spaces  */
    while (isspace(ch) || ch == ','){
        ch = line[++i];
    }
    
    if(strlen(line) > i){
        /*  end of line  */
        if ((ch == '\n') || (ch == '\0') || (ch == EOF)){
            (*par3)[0] = '\0';  /*  set par3 to be empty string  */
            return;
        }
        j = 0;
        /*  enter to par3 the second none spaces parameter  */
        while (isspace(ch) == 0 && ch != ','){                    
            (*par3)[j++] = ch;
            ch = line[++i];
        }
        (*par3)[j] = '\0';  /*  set end of string  */    
      
      
        /*  skip spaces  */
        while (isspace(ch) || ch == ','){
            ch = line[++i];
        }
        
        if(strlen(line) > i){ 
        
            /*  end of line  */
            if ((ch == '\n') || (ch == '\0') || (ch == EOF)){
            (*par4)[0] = '\0';  /*  set par4 to be empty string  */
            return;
        }
        j = 0;
        /*  enter to par4 the second none spaces parameter  */
        while (isspace(ch) == 0){
            (*par4)[j++] = ch;
            ch = line[++i];
        }
        (*par4)[j] = '\0';  /*  set end of string  */            
        }
    }
}
    
