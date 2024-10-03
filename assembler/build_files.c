#include "assembler.h"

/*
*    Thi file has three function:
*    1) function is 'buildObjectFile' create a 'name.ob' file and fill;
*    2) function is 'buildExternalsFile' create a 'name.ent' file and fill;
*    3) function is 'buildEntriesFile' create a 'name.ext' file and fill;
*/

/*creating a file with object type*/
void buildObjectFile(char *nameFile, unsigned long *words_array, int IC, int DC){
    FILE *writeFile;
    int i;
    unsigned tmp;
  
    /*write file with format .ob*/
    if((writeFile = fopen(strcat(nameFile, ".ob"), "w")) != NULL){
        fprintf(writeFile, "%7d %d\n", IC, DC);
        for (i = 0; i < (IC+DC); i++){
            tmp = (unsigned)(words_array[i]);
            /*  fix the encode word if its out of bound  */
            if (tmp >= 0XFF000000)
                tmp -= 0XFF000000;         
      
            fprintf(writeFile, "%07d %06x\n", (i + FIRST_ADDRESS), tmp);
        }
        fprintf(stdout, "Object file '%s' successfully created and filled.\n", nameFile);
    }
    else{
        fprintf(stderr, "Error - can't open/create the object file '%s'. Check the amount of free space in memory.\n", nameFile);
        exit(1);
    }
    
    fclose(writeFile);  
}

/*creating a file with entries type*/
void buildEntriesFile(char *nameFile, symbol_pointer head){
    FILE *writeFile;
    symbol *p = head;

    /*write file with format .ent*/
    if((writeFile = fopen(strcat(nameFile, ".ent"), "w")) != NULL){
        while (p != NULL){
            if ((*p).type == CODE_AND_ENTRY)
                fprintf(writeFile, "%s %07d\n", (*p).label, (*p).value);
      
            if ((*p).type == DATA_AND_ENTRY)
                fprintf(writeFile, "%s %07d\n", (*p).label, (*p).value);
            
        p = (*p).next;
        }
        fprintf(stdout, "Entries file '%s' successfully created and filled.\n", nameFile);
    }
    else{
        fprintf(stderr, "Error - can't open/create the entries file '%s'. Check the amount of free space in memory.\n", nameFile);
        exit(1);
    }
  
    fclose(writeFile);  
}

/*creating a file with externals type*/
void buildExternalsFile(char *nameFile, symbol_pointer ext){
    FILE *writeFile;
    symbol *p = ext;
   
    /*write file with format .ext*/
    if((writeFile = fopen(strcat(nameFile, ".ext"), "w")) != NULL){
        while (p != NULL){
            fprintf(writeFile, "%s %07d\n", (*p).label, (*p).value);        
            p = (*p).next;
        }
        fprintf(stdout, "Externals file '%s' successfully created and filled.\n", nameFile);
    }
    else{
        fprintf(stderr, "Error - can't open/create the externals file '%s'. Check the amount of free space in memory.\n", nameFile);
        exit(1);
    }
    
    fclose(writeFile);  
}

