#include "assembler.h"

/*
*    Thi file has four functions:
*    1) function is 'updateExtern' get parametrs for searching external, update machine code and set external to list, return 0 or 1 to 
*       'second pass'
*    2) function is 'ixExtern' check parametr if his external, retutn 1 or 0
*    3) function is 'setMachineCode' if parametr is external, set '1' to machine code to his index
*    4) function is 'setToExtern' set external to list and return 1 to 'updateExtern'
*/

/*main functions for searching external, update machine code and set external to list according to the received data*/
int updateExtern(symbol_pointer *head, char *str1, char *str2, int IC, int *cntLineCode, unsigned long (*words_array)[], symbol_pointer *ext, int *error_flag, int line){
    int flag = 0;
  
    if(strcmp(str1, "") != 0){
        /*check parameter if is symbol*/
        if(is_label(str1)){
            /*if paramert is 'external', update to machine code and set to list*/
            if(ixExtern(head, str1, error_flag, line)){
                setMachineCode(words_array, cntLineCode, IC);
                flag = setToExtern(str1, cntLineCode, ext);
            }
        }
    }
 
    if(strcmp(str2, "") != 0){
        /*check parameter if is symbol*/  
        if(is_label(str2)){
            /*if paramert is 'external, update to machine code and set to list*/ 
            if(ixExtern(head, str2, error_flag, line)){
                setMachineCode(words_array, cntLineCode, IC);
                flag = setToExtern(str2, cntLineCode, ext);
            }
        }
    }
  
    return flag;
}

/*check paramets is it external*/
int ixExtern(symbol_pointer *head, char *strPar, int *error_flag, int line){
    symbol_pointer p = *head;
    int flagSymbol = 0;
    int flagExt = 0;
  
    /*chech if parametr has to list of symbols*/
    while (p != NULL){
        if(strcmp((*p).label, strPar) == 0){
            flagSymbol = 1;
        }
        p = (*p).next;
    }
  
    p = *head;
  
    if(flagSymbol == 1){
        while (p != NULL){
            if(strcmp((*p).label, strPar) == 0 && (*p).type == EXTERNAL){
              flagExt = 1;
            }
            p = (*p).next;
        }
    }else if(flagSymbol == 0){
        *error_flag = 1;
        fprintf(stderr, "Error - parameter is '%s' in %d line not exist in list of symbols. \n", strPar, line);
    }
  
    return flagExt;
}

/*set '1' to machine code to his index*/
void setMachineCode(unsigned long (*words_array)[], int *cntLineCode, int max){
    int i = *cntLineCode;
  
    /*if symbol is 0, save his index*/
    while((*words_array)[i] != 0 && i <= max){
        i++;
    }
    *cntLineCode = i;
    
    /*set '1' to index of external*/
    (*words_array)[*cntLineCode] = 1;
}

/*set received extern to list and return 1*/
int setToExtern(char *strVar, int *cntLineCode, symbol_pointer *ext){
    symbol_pointer p, tmp;
    p = *ext;
  
    tmp = (symbol_pointer) malloc(sizeof(symbol));  /*  create a new symbol link  */
    p = find_last(ext);  /*  set the symbol pointer to the last symbol  */
  
    if(p == NULL)/*  empty list, connect the ext to the first given symbol  */
        *ext = tmp;
    else  /*  not empty list, need to be connected to p  */
        (*p).next = tmp;
  
    /*  new symbol is connected to list, now set the symbol  */
    strcpy((*tmp).label, strVar);  /*  set the symbol's label name  */
    (*tmp).value = *cntLineCode + FIRST_ADDRESS;  /*  set the symbol's value  */
    (*tmp).type = EXTERNAL;
    (*tmp).next = NULL;
    
    return 1;
}

