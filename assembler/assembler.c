#include "assembler.h"

/*
*    the program assembler is program that receive the names of assembler files and complie them
*    if the compilation was successfully with out errors then the program will crate 3 output file 
*    if the compilation was not successfully with out errors,
*    then the program will print the specific error to the user and no output will be created
*    the three output files are object, enteries and external
*    the object's file will have the number of instuction words and data words and their coded words
*    the enteries's file will have the name of the entry symbols and their address 
*    the external's file will have the name of the extern symbols and their address of where they was used
*    the maximum size of the given file name is SIZE_OF_FILE_NAME  
*/
int main(int argc, char *argv[])
{
    unsigned long words_array[SIZE_OF_WORDS] = {0};  /*  set array of words to zero by defulte  */
    unsigned long DC_array[SIZE_OF_WORDS] = {0};  /*  set array of data words to zero by defulte  */
    symbol_pointer head;  /*typedef struct for table of symbols*/ 
    symbol_pointer ext;   /*typedef struct for table of externals*/ 
    int i = 0;  /*  index to analyst all the input files from command line  */
    int error_flag;  /*  indicate if there was an error in the input file  */
    int DC;  /*  date counter  */
    int IC;  /*  instruction counter  */
    FILE *input = NULL;  /*  set pointer to the input file, by default its set to null  */
    char file_name[SIZE_OF_FILE_NAME];  /*  temporary string to hold the file name  */
    int j;  /*  index to reset words_array and DC_array  */

    while (++i < argc)  /*  loop to analyst all the input files from command line  */
    {
        DC = IC = 0;  /*  reset DC and IC before/after using them  */
        head = NULL;  /* reset head before/after using him  */
        ext = NULL;  /* reset ext before/after using him  */
        file_name[0] = '\0';  /*  reset file_name before/after using him  */
        error_flag = 0;  /*  reset error_flag before/after using him  */
        
        if (strlen(argv[i]) > SIZE_OF_FILE_NAME)  /*  fail open/read the given file */ 
        {
            fprintf(stderr, "Error - the max size of file name length is %d, the name '%s' is too long\n",SIZE_OF_FILE_NAME-1 ,argv[i]);
        } 
        else
        {       
            strcpy(file_name, argv[i]);  /*  copy the file name from command line to the string file_name  */
            if ((input = fopen(strcat(file_name,".as"), "r")) == NULL)  /*  fail open/read the given file  */
            {            
                fprintf(stderr, "Error - can't write/open the file '%s'\n",argv[i]);         
            }
             else  /*  open/read the file successfully  */
             {
                 fprintf(stdout, "***************************\nReading the file number %d,  '%s.as':\n\n",i ,argv[i]);                 
                 first_pass(&DC, &IC, &error_flag, input, &head, &words_array, &DC_array);
                 set_symbols(&head, IC);

                 /*SECOND PASS*/
                 if (error_flag == 0){                     
                     if ((input = fopen(file_name, "r")) != NULL){
                         second_pass(&DC, &IC, &error_flag, input, &head, &words_array, &DC_array, &ext);
                               
                         /*building files with using strtok*/
                         if(error_flag == 0){
                                  
                             /*create object file*/
                             strcpy(file_name, argv[i]);
                             buildObjectFile(file_name, words_array, IC, DC); 
                       
                             /*create entries file*/
                             strcpy(file_name, argv[i]);                          
                             buildEntriesFile(file_name, head);                  
                        
                             /*create extermal file*/
                             strcpy(file_name, argv[i]);
                             buildExternalsFile(file_name, ext);
                        
                             /*output successfully finish*/                       
                             strcpy(file_name, argv[i]);
                             strcat(file_name,".as");
                        
                             fprintf(stdout, "File '%s' successfully readed.\n", file_name);                        
                         }
                     }else fprintf(stderr, "Error - can't write/open the file '%s' to second pass.\n",argv[i]);        	
                 }           
                 fprintf(stdout, "***************************\n\n");
             }
        }
        
        /*  free and clear all the dynamic memory and arrays  */
        for (j = 0; j < SIZE_OF_WORDS; j++)  /*  reset words_array and DC_array before/after using them  */
        {
            words_array[j] = 0;
            DC_array[j] = 0;
        } 
        if (head != NULL)  /*  release the dynamically allocated memory  */
        {
            free_symbol(&head);   
        } 
        if (ext != NULL)  /*  release the dynamically allocated memory  */
        {
            free_symbol(&ext);  
        } 
        if (input != NULL)  /*  close the input pointer after using him  */
        {
            fclose(input);
            input = NULL;
        }
    }
    return 0;
}

