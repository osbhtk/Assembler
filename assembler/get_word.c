#include "assembler.h"

/*
*    get_DC_words:  this function reveive 6 parameters and return nothing
*    the first parameter is pointer to array of unsigned long named DC_array, DC_array have all the data/string encode words
*    the second parameter is int pointer named DC, DC is index of the Data Counter in DC_array
*    the third parameter is string named line, the function will initialize encode data/string words from line
*    the fourth parameter is int named symbol_flag, this flag 1 if there is symbol in line and 0 if the is not symbol in line
*    the fifth parameter is int pointer named error_flag, error flag will turn on if the function found error
*    the sixth parameter is int named number_of_line that reprecent the current line from input file
*    the function check the given line and sends to put_DC_words the parameters to be encode data/string words in DC_array
*    if the function found an error its will print to stdout the error and the line with indication which error has been happend
*/
void get_DC_words(unsigned long (*DC_array)[],int *DC,char *line, int symbol_flag, int *error_flag, int number_of_line)
{
    char c;
    int sign;
    int num;
    int j;
    int i = 0;
    c = line[i];
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    if (symbol_flag == 1)  /*  first prameret is symbol, need to skip the symbol  */
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
    if (line[i] == '.' && line[i+1] == 'd' && line[i+2] == 'a' && line[i+3] == 't' && line[i+4] == 'a')  /*  data line  */
    {
        i += 4;
        c = line[++i];
        while (c == ' ' || c == '\t')  /*  skip spaces  */
        {
            c = line[++i];
        }
        if (c == '-')  /*  have negative sign  */
        {
            sign = -1;
            c = line[++i];  /*  give c the first character in the number  */
        }
        else if (c == '+')  /*  have positive sign  */
        {
            sign = 1;
            c = line[++i];  /*  give c the first character in the number  */
        }
        else if (isdigit(c))  /*  dont have sign  */
        {
            sign = 1;
        }
        else if (c == ',')  /*  error  */
        {
            fprintf(stderr, "Error in line %d, illegal comma\n",number_of_line);
            *error_flag = 1;
            return;
        }
        if (!isdigit(c))  /*  error  */
        {
            if (line[i-1] == ' ' || line[i-1] == '\t' || line[i-1] == ',')
            {
                fprintf(stderr, "Error in line %d, the command '.data' can storage only integers\n",number_of_line);
                *error_flag = 1;
                return;
            }
            if (line[i-1] == '+')
            {
                fprintf(stderr, "Error in line %d, need to be number after '+'\n",number_of_line);
                *error_flag = 1;
                return;
            }
            if (line[i-1] == '-')
            {
                fprintf(stderr, "Error in line %d, need to be number after '-'\n",number_of_line);
                *error_flag = 1;
                return;
            }
            if (line[i] == '\n' || line[i] == '\0' || line[i] == EOF)
            {
                fprintf(stderr, "Error in line %d, need to be number after '.data'\n",number_of_line);
                *error_flag = 1;
                return;
            } 
        }
        num = 0;
        while (isdigit(c))  /*  put the given first number in num variable  */
        {
            num = (num*10) + ((int)(c) - 48);
            c = line[++i];
        }
        num *= sign;  /*  update the sign of number  */
        if ((MIN_SIZE_24_DIGITS <= num) && (num <= MAX_SIZE_24_DIGITS))  /*  inside valid range  */
        {
            put_num_to_DC(DC_array, DC, num);  /*  add the num to DC_array  */
        }
        else  /*  error  */
        {
            if (num < MIN_SIZE_24_DIGITS)
            {
                fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, the minimum valid value is %d\n",number_of_line,num,MIN_SIZE_24_DIGITS);
                *error_flag = 1;
                return;
            }
            if (num > MAX_SIZE_24_DIGITS)
            {
                fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, the maximum valid value is %d\n",number_of_line,num,MAX_SIZE_24_DIGITS);
                *error_flag = 1;
                return;
            }
        }
        while (c == ' ' || c == '\t')  /*  skip spaces  */
        {
            c = line[++i];
        }
        if (c == '\n' || c == '\0' || c == EOF)  /*  finish setting line to DC_array  */
        {
            return;
        }
        if (c != ',')  /*  error  */
        {
            fprintf(stderr, "Error in line %d, missing comma\n",number_of_line);
            *error_flag = 1;
            return;
        }
        /*  c now is ','  */
        while (c != '\n' && c != '\0' && c != EOF)  /*  loop to add all the numbers after '.data'  */
        {
            c = line[++i];
            while (c == ' ' || c == '\t')  /*  skip spaces  */
            {
                c = line[++i];
            }
            if (c == '\n' || c == '\0' || c == EOF)  /*  error  */
            {
                fprintf(stderr, "Error in line %d, missing parameter after comma\n",number_of_line);
                *error_flag = 1;
                return;
            }
            if (c == ',')  /*  error  */
            {
                fprintf(stderr, "Error in line %d, multiple consecutive commas\n",number_of_line);
                *error_flag = 1;
                return;
            }
            if (c == '-')  /*  have negative sign  */
            {
                sign = -1;
                c = line[++i];  /*  give c the first character in the number  */
            }
            else if (c == '+')  /*  have positive sign  */
            {
                sign = 1;
                c = line[++i];  /*  give c the first character in the number  */
            }
            else if (isdigit(c))  /*  dont have sign  */
            {
                sign = 1;
            }
            if (!isdigit(c))  /*  error  */
            {
                if (line[i-1] == ' ' || line[i-1] == '\t' || line[i-1] == ',')
                {
                    fprintf(stderr, "Error in line %d, the command '.data' can storage only integers\n",number_of_line);
                    *error_flag = 1;
                    return;
                }
                if (line[i-1] == '+')
                {
                    fprintf(stderr, "Error in line %d, need to be number after '+'\n",number_of_line);
                    *error_flag = 1;
                    return;
                }
                if (line[i-1] == '-')
                {
                    fprintf(stderr, "Error in line %d, need to be number after '-'\n",number_of_line);
                    *error_flag = 1;
                    return;
                }
            }
            num = 0;  /*  reset the num to use it again  */
            while (isdigit(c))  /*  put the given number in num variable  */
            {
                num = (num*10) + ((int)(c) - 48);
                c = line[++i];
            }
            num *= sign;  /*  update the sign of number  */
            
            if ((MIN_SIZE_24_DIGITS <= num) && (num <= MAX_SIZE_24_DIGITS))  /*  inside valid range  */
            {
                put_num_to_DC(DC_array, DC, num);  /*  add the num to DC_array  */
            }
            else  /*  error  */
            {
                if (num < MIN_SIZE_24_DIGITS)
                {
                    fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, the minimum valid value is %d\n",number_of_line,num,MIN_SIZE_24_DIGITS);
                    *error_flag = 1;
                    return;
                }
                if (num > MAX_SIZE_24_DIGITS)
                {
                    fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, the maximum valid value is %d\n",number_of_line,num,MAX_SIZE_24_DIGITS);
                    *error_flag = 1;
                    return;
                }
            }
            while (c == ' ' || c == '\t')  /*  skip spaces  */
            {
                c = line[++i];
            }
            if (c == '\n' || c == '\0' || c == EOF)  /*  finish setting line to DC_array  */
            {
                return;
            }
            if (c != ',')  /*  error  */
            {
                fprintf(stderr, "Error in line %d, missing comma\n",number_of_line);
                *error_flag = 1;
                return;
            }
        }
    }  /*  end of storing data line  */
    else  /*  string line  */
    {
        i += 6;
        c = line[++i];
        while (c == ' ' || c == '\t')  /*  skip spaces  */
        {
            c = line[++i];
        }
        if (c != '"')  /*  error  */
        {
            fprintf(stderr, "Error in line %d, need to be \" after '.string'\n",number_of_line);
            *error_flag = 1;
            return;
        }
        /*  now i have the index of the first quotation mark  */
        /*  find last quotation mark index and store it in j  */
        j = strlen(line);
        while ((c = line[--j] != '"'))
        ;
        if (i == j)  /*  error  */
        {
            fprintf(stderr, "Error in line %d, only one quotation mark instead of two\n",number_of_line);
            *error_flag = 1;
            return;
        }
        i++;  /*  set the index i for the first character inside the quotation mark  */
        for (; i < j; i++)  /*  add all the characters between the quotation marks  */
        {
            c = line[i];
            num = (int)(c);
            put_num_to_DC(DC_array, DC, num);
        }
        put_num_to_DC(DC_array, DC, 0);  /*  add end of string '\0'  */
        /*  set the index i and c to the character after end of string  */
        i = i + 2;  
        c = line[i];
        while (c == ' ' || c == '\t')  /*  skip spaces  */
        {
            c = line[++i];
        }
        if (c != '\n' && c != '\0' && c != EOF)  /*  error  */
        {
            fprintf(stderr, "Error in line %d, only white spaces is valid after the quotation marks\n",number_of_line);
            *error_flag = 1;
            return;
        }
    }  /*  end of storing sting line  */
}


/*
*    get_IC_words:  this function receive 6 parameters and return nothing
*    the first parameter is pointer to array of unsigned long named words_array, DC_array have all the encode words
*    the second parameter is int pointer named IC, IC is index of the Data instruction in words_array
*    the third parameter is string named line, the function will initialize encode words from line
*    the fourth parameter is int named symbol_flag, this flag 1 if there is symbol in line and 0 if the is not symbol in line
*    the fifth parameter is int pointer named error_flag, error flag will turn on if the function found error
*    the sixth parameter is int named number_of_line that reprecent the current line from input file
*    the function check the given line and sends to put_word_to_IC_array the parameters to be encode words in words_array
*    if the function found an error its will print to stdout the error and the line with indication which error has been happend
*/
void get_IC_words(unsigned long (*words_array)[],int *IC,char *line, int symbol_flag, int *error_flag, int number_of_line)
{
    char c;
    int i, j;
    char par1[SIZE_OF_PAR];
    char par2[SIZE_OF_PAR];
    char par3[SIZE_OF_PAR];
    i = 0;
    c = line[i];
    par1[0] = '\0';  /*  set par1 to be an empty string  */
    par2[0] = '\0';  /*  set par2 to be an empty string  */
    par3[0] = '\0';  /*  set par3 to be an empty string  */

    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    if (symbol_flag == 1)  /*  first prameret is symbol, need to skip the symbol  */
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
    if (c == ',')  /*  error  */
    {
        fprintf(stderr, "Error in line %d, illegal comma\n",number_of_line);
        *error_flag = 1;
        return;
    }
    /*  found none spaces character  */
    j = 0;
    while (isspace(c) == 0 && c != EOF  && c != '\0' && c != ',')  /*  set first parameret from line inside par1 string  */
    {
        par1[j++] = c;
        c = line[++i];
    }
    par1[j] = '\0';  /*  set end of string  */
    if (is_command(par1) == 0)  /* error  */
    {
        fprintf(stderr, "Error in line %d, the command '%s' is unknown command\n",number_of_line, par1);
        *error_flag = 1;
        return;
    }
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    if (c == ',')  /*  error  */
    {
        fprintf(stderr, "Error in line %d, illegal comma\n",number_of_line);
        *error_flag = 1;
        return;
    }
    if (c == '\n' || c == EOF || c == '\0')  /*  finished initialize line  */
    {
       if ((strcmp(par1,"rts") != 0) && (strcmp(par1,"stop") != 0))  
        {
            fprintf(stderr, "Error in line %d, missing paramerets after command '%s'\n",number_of_line, par1);
            *error_flag = 1;
            return;
        }
        put_word_to_IC_array(words_array, IC, par1, par2, par3);
        return;  /* finish successfully initialize line and update IC_array  */
    }
    /*  found none space character  */
    j = 0;
    while (isspace(c) == 0 && c != ',' && c != '\0' && c != EOF)  /*  set second parameret from line inside par2 string  */
    {
        par2[j++] = c;
        c = line[++i];
    }
    par2[j] = '\0';  /*  set end of string  */
    if ((strcmp(par1,"rts") == 0) || (strcmp(par1,"stop") == 0))  /*  error  */
    {
        fprintf(stderr, "Error in line %d, extraneous text after end of command\n",number_of_line);
        *error_flag = 1;
        return;
    }
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    if (c == '\n' || c == EOF || c == '\0')  /*  finished initialize line  */
    {
       if ((strcmp(par1,"clr") != 0) && (strcmp(par1,"not") != 0) &&(strcmp(par1,"inc") != 0) && (strcmp(par1,"dec") != 0) && 
            (strcmp(par1,"jmp") != 0) && (strcmp(par1,"bne") != 0) && (strcmp(par1,"jsr") != 0) && (strcmp(par1,"red") != 0) &&
            (strcmp(par1,"prn") != 0))  /*  error */ 
        {
            fprintf(stderr, "Error in line %d, missing paramerets after command '%s'\n",number_of_line, par1);
            *error_flag = 1;
            return;
        }
        if (is_valid_words1(par1, par2, error_flag, number_of_line))
        {
            put_word_to_IC_array(words_array, IC, par1, par2, par3);
        }
        return;  /* finish initialize line and update IC_array  */
    }
    /*  found none space character  */
    if (c != ',')  /*  error  */
    {
        fprintf(stderr, "Error in line %d, missing command\n",number_of_line);
        *error_flag = 1;
        return;
    }
    /*   c is ',' character  */
    c = line[++i];  /*  set c to the character after ','  */
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    if (c == ',')  /*  error  */
    {
        fprintf(stderr, "Error in line %d, multiple consecutive commas\n",number_of_line);
        *error_flag = 1;
        return;
    }
    /*  found none space character  */
    j = 0;
    while (isspace(c) == 0 && c != ',' && c != '\0' && c != EOF)  /*  set third parameret from line inside par3 string  */
    {
        par3[j++] = c;
        c = line[++i];
    }
    par3[j] = '\0';  /*  set end of string  */
    while (c == ' ' || c == '\t')  /*  skip spaces  */
    {
        c = line[++i];
    }
    if (c != '\n' && c != EOF && c != '\0')  /*  error  */
    {
        fprintf(stderr, "Error in line %d, extraneous text after end of command\n",number_of_line);
        *error_flag = 1;
        return;
    }
    /*  finished initialize line  */
    if ((strcmp(par1,"clr") == 0) || (strcmp(par1,"not") == 0) ||(strcmp(par1,"inc") == 0) || (strcmp(par1,"dec") == 0) || 
        (strcmp(par1,"jmp") == 0) || (strcmp(par1,"bne") == 0) || (strcmp(par1,"jsr") == 0) || (strcmp(par1,"red") == 0) || 
        (strcmp(par1,"prn") == 0))  /*  error  */
    {
        fprintf(stderr, "Error in line %d, extraneous text after end of command\n",number_of_line);
        *error_flag = 1;
        return;
    }
    if (is_valid_words2(par1, par2, par3, error_flag, number_of_line))
    {
        put_word_to_IC_array(words_array, IC, par1, par2, par3);
    }
    return; /* finish initialize line and update IC_array  */
}



