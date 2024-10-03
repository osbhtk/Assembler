#include "assembler.h"

/*
*    Thi file has four functions:
*    1) function is 'updateSymbol' get parametrs for checking and update machine code
*    2) function is 'getOperationNumber' check what is operation need to make, return: 0 or 1 or 2
*    3) function is 'getAddress' get address of symbol from table
*    4) function is 'updateMachineCode' if has addres, update machin code via operation (set address of label or distance to label)
*       to his index
*/

/*main functions for checking and updating machine code according to the received data*/
void updateSymbol(symbol_pointer *head, char *str1, char *str2, char *str3, int IC, int *cntLineCode, unsigned long (*words_array)[]){
    /* symbol_pointer p = *head;*/
    int firstAdres = 0, secondAdres = 0;
    int codeOperation = 0;
    
    /*get operation number*/
    if(strcmp(str1, "") != 0){
        codeOperation = getOperationNumber(str1, str2);
    }
  
    /*get address of parameter to 'firstAdres'*/
    if(strcmp(str2, "") != 0){
        firstAdres = getAddress(head, str2);
    }
  
    /*get address of parameter to 'secondAdres'*/
    if(strcmp(str3, "") != 0 || firstAdres == 0){
        secondAdres = getAddress(head, str3);
    }
  
    /*if has operation and address, update machine code*/
    if(codeOperation != 0){
        if(firstAdres != 0)
            updateMachineCode(words_array, cntLineCode, IC, codeOperation, firstAdres);
        else if(secondAdres != 0)
            updateMachineCode(words_array, cntLineCode, IC, codeOperation, secondAdres);
    }    
}

/*get number operation via command, the number indicates the number of objects in the command.*/
int getOperationNumber(char *strOpr, char *str){
    int numOpr = 0;

    if(strcmp(strOpr, "add") == 0 || strcmp(strOpr, "sub") == 0){
        numOpr = 1;
    }else if(strcmp(strOpr, "mov") == 0 || strcmp(strOpr, "lea") == 0 || strcmp(strOpr, "cmp") == 0){
        numOpr = 1;
    }else if(strcmp(strOpr, "inc") == 0 || strcmp(strOpr, "dec") == 0 || strcmp(strOpr, "prn") == 0){
        numOpr = 1;
    }else if(strcmp(strOpr, "clr") == 0 || strcmp(strOpr, "not") == 0){
        numOpr = 1;        
    }else if(str[0] != '&'){
        if(strcmp(strOpr, "jmp") == 0 || strcmp(strOpr, "jsr") == 0 || strcmp(strOpr, "bne") == 0){
            numOpr = 1;    
        }
    }else if(str[0] == '&'){
        if(strcmp(strOpr, "jmp") == 0 || strcmp(strOpr, "jsr") == 0 || strcmp(strOpr, "bne") == 0){
            numOpr = 2;    
        }
    }
    
    return numOpr;
}

/*checking par in table of symbols*/
int getAddress(symbol_pointer *head, char *str){
    symbol_pointer p = *head;
    int val = 0;
 
    /*remove a first symbol '&'*/
    if (str[0] == '&')
        memmove(str, str+1, strlen(str));
  
    /*search and get address, everything except external*/
    if(str[0] != '#'){
        while (p != NULL){
            if(strcmp((*p).label, str) == 0){
                if((*p).type != EXTERNAL){
                    val = (*p).value;
                }
            }
            p = (*p).next;
        }
    }
     
    return val;
}

/*update machine code (set address of label or distance to label)*/
void updateMachineCode(unsigned long (*words_array)[], int *cntLineCode, int max, int operation, int address){
    int i = *cntLineCode;
  
    /*if symbol is 0, save his index*/
    while((*words_array)[i] != 0 && i <= max){
        i++;
    }
    *cntLineCode = i;
      
    /*make update via operation 1 or 2 to received index*/
    switch(operation){
        /*inc, dec, prn, mov, add, sub, lea, clr, not, cmp and jmp, jsr, bne without '&'*/
        case 1:      
            (*words_array)[*cntLineCode] += address << 3;
            (*words_array)[*cntLineCode] += 2;
            break;
    
        /*jmp, jsr, bne with '&'*/
        case 2:
            (*words_array)[*cntLineCode] = (address - (*cntLineCode + 100 - 1)) << 3;
            (*words_array)[*cntLineCode] += 4;
            break;            
    }
}


