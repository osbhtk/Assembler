#include "assembler.h"

/*
*    put_num_to_DC:  this function recieve 3 parameters and return nothing
*    the first parameter is pointer to array of unsigned long named DC_array, DC_array have all the data/string encode words
*    the second parameter is int pointer named DC, DC is index of the Data Counter in DC_array
*    the third parameter is int named num, num is the value to be encode to DC_array
*    the function encode num to DC_array
*/
void put_num_to_DC(unsigned long (*DC_array)[], int *DC, int num)
{
    ((*DC_array)[(*DC)++]) = num;  /*  encode num to DC_array  */
}

/*
*    put_word_to_IC_array: this function receive 5 parameters and return nothing
*    the first parameter is pointer to array of unsigned long named words_array, words_array have all the encode words
*    the second parameter is int pointer named IC, IC is index of the Data instruction in words_array
*    the third parameter string named par1, par1 is the first none symbol parameter in the current line
*    the fourth parameter string named par2, par2 is the second none symbol parameter in the current line
*    the fifth parameter string named par3, par3 is the third none symbol parameter in the current line
*    the function encode words to words_array, 
*    the function can add one, two or three encode to words_array dependent on the given parameters
*    if one of the parameters is label symbol name, then the function 'save space' for this encode words for the second pass by defulte initialize the word to zero
*/
void put_word_to_IC_array(unsigned long (*words_array)[], int *IC, char *par1, char *par2, char *par3)
{
    int opcode_number, source_number, source_register, dest_number, dest_register, funct_number;
    /*  reset all value by defulte to zero  */
    opcode_number = 0;
    source_number = 0;
    source_register = 0;
    dest_number = 0;
    dest_register = 0;
    funct_number = 0;
    /*  set opcode_number and funct_number  */
    if (strcmp(par1,"mov") == 0)
    {
        opcode_number = 0;
    }
    if (strcmp(par1,"cmp") == 0)
    {
        opcode_number = 1;
    }
    if (strcmp(par1,"add") == 0)
    {
        opcode_number = 2;
        funct_number = 1;
    }
    if (strcmp(par1,"sub") == 0)
    {
        opcode_number = 2;
        funct_number = 2;
    }
    if (strcmp(par1,"lea") == 0)
    {
        opcode_number = 4;
    }
    if (strcmp(par1,"clr") == 0)
    {
        opcode_number = 5;
        funct_number = 1;
    }
    if (strcmp(par1,"not") == 0)
    {
        opcode_number = 5;
        funct_number = 2;
    }
    if (strcmp(par1,"inc") == 0)
    {
        opcode_number = 5;
        funct_number = 3;
    }
    if (strcmp(par1,"dec") == 0)
    {
        opcode_number = 5;
        funct_number = 4;
    }
    if (strcmp(par1,"jmp") == 0)
    {
        opcode_number = 9;
        funct_number = 1;
    }
    if (strcmp(par1,"bne") == 0)
    {
        opcode_number = 9;
        funct_number = 2;
    }
    if (strcmp(par1,"jsr") == 0)
    {
        opcode_number = 9;
        funct_number = 3;
    }
    if (strcmp(par1,"red") == 0)
    {
        opcode_number = 12;
    }
    if (strcmp(par1,"prn") == 0)
    {
        opcode_number = 13;
    }
    if (strcmp(par1,"rts") == 0)
    {
        opcode_number = 14;
    }
    if (strcmp(par1,"stop") == 0)
    {
        opcode_number = 15;
    }

    /*  have source and destination  */
    if ((opcode_number == 0) || (opcode_number == 1) || (opcode_number == 2) || (opcode_number == 4))
    {
        /*  setting source  */
        if (is_number(par2))
        {
            source_number = 0;
        }
        if (is_label(par2))
        {
            source_number = 1;
        }
        if (is_register(par2))
        {
            source_number = 3;
            source_register = (int)(par2[1]) - 48;
        }
        /*  setting destination  */
        if (is_number(par3))
        {
            dest_number = 0;
        }
        if (is_label(par3))
        {
            dest_number = 1;
        }
        if (is_register(par3))
        {
            dest_number = 3;
            dest_register = (int)(par3[1]) - 48;
        }
    }
    /*  have only destination  */
    if ((opcode_number == 5) || (opcode_number == 9) || (opcode_number == 12) || (opcode_number == 13))
    {
        /*  setting destination  */
        if (is_number(par2))
        {
            dest_number = 0;
        }
        if (is_label(par2))
        {
            dest_number = 1;
        }
        if (par2[0] == '&')
        {
            dest_number = 2;
        }
        if (is_register(par2))
        {
            dest_number = 3;
            dest_register = (int)(par2[1]) - 48;
        }
    }
    /*  set first word  */
    (*words_array)[*IC] += (opcode_number)<<18;
    (*words_array)[*IC] += (source_number)<<16;
    (*words_array)[*IC] += (source_register)<<13;
    (*words_array)[*IC] += (dest_number)<<11;
    (*words_array)[*IC] += (dest_register)<<8;
    (*words_array)[*IC] += (funct_number)<<3;
    (*words_array)[*IC] += (1)<<2;
    (*words_array)[*IC] += (0)<<1;
    (*words_array)[*IC] += (0);
    (*IC)++;
    /*  initialize or save space if there is more words  */
    /*  checking source  */
    if ((source_number == 0) && ((opcode_number == 0) || (opcode_number == 1) || (opcode_number == 2)))
    {
        (*words_array)[*IC] += (atoi(par2+1))<<3;
        (*words_array)[*IC] += (1)<<2;
        (*words_array)[*IC] += (0)<<1;
        (*words_array)[*IC] += (0);
        (*IC)++;
    }
    if ((source_number == 1) || (source_number == 2))
    {
        (*IC)++;
    }
    /*  checking destination  */
    if ((dest_number == 0) && (opcode_number == 13))
    {
        ((*words_array)[*IC]) += (atoi(par2+1))<<3;
        ((*words_array)[*IC]) += (1)<<2;
        ((*words_array)[*IC]) += (0)<<1;
        ((*words_array)[*IC]) += (0);
        (*IC)++;
    }
    if ((dest_number == 0) && (opcode_number == 1))
    {
        ((*words_array)[*IC]) += (atoi(par3+1))<<3;
        ((*words_array)[*IC]) += (1)<<2;
        ((*words_array)[*IC]) += (0)<<1;
        ((*words_array)[*IC]) += (0);
        (*IC)++;
    }
    if ((dest_number == 1) || (dest_number == 2))
    {
        (*IC)++;
    }
}

/*
*    put_DC_array_to_words_array:  this function receive 4 parameters and return nothing
*    the first parameter is pointer to array of unsigned long named words_array, words_array have all the encode words
*    the second parameter is pointer to array of unsigned long named DC_array, DC_array have all the data/string encode words
*    the third parameter is int named IC, IC is Instruction Counter, its an index of how many words has been initialize in words_array
*    the fourth parameter is in named DC, DC is Data Counte, its an index of how many words has been initialize in DC_array
*    this function add all the encode words from DC_array to the end of words_array by order (from the first words in DC_array to the last one)
*/
void put_DC_array_to_words_array(unsigned long (*words_array)[], unsigned long (*DC_array)[], int IC, int DC)
{
    int i, j;  /*  temporary indexs  */
    for(i = IC, j = 0; j < DC; i++, j++)  /*  loop to add all the encode words from DC_array to the end of words_array  */
    {
        (*words_array)[i] = (*DC_array)[j];
    }
}


