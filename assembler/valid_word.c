#include "assembler.h"

/*
*    is_valid_word1:  this function receive 4 parameters and return int
*    the first parameter is string named par1, par1 have the first none symbol parameter from current input line
*    the second parameter is string named par2, par2 have the second none symbol parameter from current input line
*    the third parameter is pointer to int named error_flag, error_flag will turn on if function will find an error
*    the sixth parameter is int named number_of_line that reprecent the current line from input file
*    the function check is the combination of par1 and par2 is valid
*    if its valid then the function return 1
*    if its not valid the function return 0 and print to stdout the error and the number of line
*/
int is_valid_words1(char *par1, char *par2, int *error_flag, int number_of_line)
{
    if ((strcmp(par1,"clr") == 0) || (strcmp(par1,"not") == 0) || (strcmp(par1,"inc") == 0) || 
        (strcmp(par1,"dec") == 0) || (strcmp(par1,"red") == 0))
    {
        if (is_label(par2) || is_register(par2))
        {
            return 1;  /*  valid word  */
        }
        else  /* error  */
        {
            fprintf(stderr, "Error in line %d, unvalid destination name '%s'\n",number_of_line, par2);
            *error_flag = 1;
            return 0;
        }
    }
    if ((strcmp(par1,"jmp") == 0) || (strcmp(par1,"bne") == 0) || (strcmp(par1,"jsr") == 0))
    {
        if (is_label(par2) || ((par2[0] == '&') && is_label(par2 + 1)))
        {
            return 1;  /*  valid word  */
        }
        else  /*  error  */
        {
            fprintf(stderr, "Error in line %d, unvalid destination name '%s'\n",number_of_line, par2);
            *error_flag = 1;
            return 0;
        }
    }
    if (strcmp(par1,"prn") == 0)
    {
        if (is_label(par2) || is_register(par2))
        {
            return 1;  /*  valid word  */
        }
        else if (is_number(par2) == 1)
        {
            if (is_valid_number(par2) == -1)  /*  error  */
            {
                fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, ",number_of_line, atoi(par2+1));
                fprintf(stderr, "the minimum valid value is %d\n",MIN_SIZE_21_DIGITS);
                *error_flag = 1;
                return 0;
            }
            if (is_valid_number(par2) == -2)  /*  error  */
            {
                fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, ",number_of_line, atoi(par2+1));
                fprintf(stderr, "the maximum valid value is %d\n",MAX_SIZE_21_DIGITS);
                *error_flag = 1;
                return 0;
            } 

            return 1;  /*  valid word  */
        }       
        else if (is_number(par2) == 0)  /*  error  */
        {
            fprintf(stderr, "Error in line %d, the number '%s' is not valid, only numbers are valid after the '#'\n",number_of_line,par2);
            *error_flag = 1;
            return 0;
        }
        else  /*  error  */
        {
            fprintf(stderr, "Error in line %d, unvalid destination name '%s'\n",number_of_line, par2);
            *error_flag = 1;
            return 0;
        }
    }
    return 0;  /*  defulte error  */    
}

/*
*    is_valid_word2:  this function receive 5 parameters and return int
*    the first parameter is string named par1, par1 have the first none symbol parameter from current input line
*    the second parameter is string named par2, par2 have the second none symbol parameter from current input line
*    the third parameter is string named par3, par3 have the third none symbol parameter from current input line
*    the fourth parameter is pointer to int named error_flag, error_flag will turn on if function will find an error
*    the fifth parameter is int named number_of_line that reprecent the current line from input file
*    the function check is the combination of par1, par2 and par3 is valid
*    if its valid then the function return 1
*    if its not valid the function return 0 and print to stdout the error and the number of line
*/
int is_valid_words2(char *par1, char *par2, char *par3, int *error_flag, int number_of_line)
{
    if ((strcmp(par1,"mov") == 0) || (strcmp(par1,"add") == 0) || (strcmp(par1,"sub") == 0))
    {
        /*  check source  */
        if (!(is_number(par2)) && (!is_label(par2)) && (!is_register(par2)))  /*  error */
        {
            fprintf(stderr, "Error in line %d, unvalid source name '%s'\n",number_of_line, par2);
            *error_flag = 1;
            return 0;
        }
        if (is_number(par2) == 1)
        {
            if (is_valid_number(par2) == -1)  /*  error  */
            {
                fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, ",number_of_line, atoi(par2+1));
                fprintf(stderr, "the minimum valid value is %d\n",MIN_SIZE_21_DIGITS);
                *error_flag = 1;
                return 0;
            }
            if (is_valid_number(par2) == -2)  /*  error  */
            {
                fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, ",number_of_line, atoi(par2+1));
                fprintf(stderr, "the maximum valid value is %d\n",MAX_SIZE_21_DIGITS);
                *error_flag = 1;
                return 0;
            } 
        }
        /*  check destination  */
        if  ((!is_register(par3)) && (!is_label(par3)))  /*  error  */
        {
            fprintf(stderr, "Error in line %d, unvalid destination name '%s'\n",number_of_line, par3);
            *error_flag = 1;
            return 0;
        }
        /*  if code reach here word are valid  */
        return 1;  /*  valid word  */
    }
    if (strcmp(par1,"cmp") == 0)
    {
        /*  check source  */
        if ((!is_number(par2)) && (!is_label(par2)) && (!is_register(par2)))  /*  error */
        {
            fprintf(stderr, "Error in line %d, unvalid source name '%s'\n",number_of_line, par2);
            *error_flag = 1;
            return 0;
        }
        if (is_number(par2) == 1)
        {
            if (is_valid_number(par2) == -1)  /*  error  */
            {
                fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, ",number_of_line, atoi(par2+1));
                fprintf(stderr, "the minimum valid value is %d\n",MIN_SIZE_21_DIGITS);
                *error_flag = 1;
                return 0;
            }
            if (is_valid_number(par2) == -2)  /*  error  */
            {
                fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, ",number_of_line, atoi(par2+1));
                fprintf(stderr, "the maximum valid value is %d\n",MAX_SIZE_21_DIGITS);
                *error_flag = 1;
                return 0;
            } 
        }
        /*  check destination  */
        if  ((!is_register(par3)) && (!is_label(par3)) && (!is_number(par3)))  /*  error  */
        {
            fprintf(stderr, "Error in line %d, unvalid destination name '%s'\n",number_of_line, par3);
            *error_flag = 1;
            return 0;
        }
        if (is_number(par3) == 1)
        {
            if (is_valid_number(par3) == -1)  /*  error  */
            {
                fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, ",number_of_line, atoi(par3+1));
                fprintf(stderr, "the minimum valid value is %d\n",MIN_SIZE_21_DIGITS);
                *error_flag = 1;
                return 0;
            }
            if (is_valid_number(par3) == -2)  /*  error  */
            {
                fprintf(stderr, "Error in line %d, the number value '%d' is out of limits, ",number_of_line, atoi(par3+1));
                fprintf(stderr, "the maximum valid value is %d\n",MAX_SIZE_21_DIGITS);
                *error_flag = 1;
                return 0;
            } 
        }
        /*  if code reach here word are valid  */
        return 1;  /*  valid word  */
    }
    if (strcmp(par1,"lea") == 0)
    {
        /*  check source  */
        if (!is_label(par2))  /*  error */
        {
            fprintf(stderr, "Error in line %d, unvalid source name '%s'\n",number_of_line, par2);
            *error_flag = 1;
            return 0;
        }
        /*  check destination  */
        if  ((!is_register(par3)) && (!is_label(par3)))  /*  error  */
        {
            fprintf(stderr, "Error in line %d, unvalid destination name '%s'\n",number_of_line, par3);
            *error_flag = 1;
            return 0;
        }
        /*  if code reach here word are valid  */
        return 1;  /*  valid word  */
    }
    return 0;  /*  defulte error  */    
}

/*
*    is_number:  the function receive 1 parameter and return nothing
*    the first parameter is string named par, par have the parameter to be checked
*    the function check if par is number type of instuction line
*    in instuction line number start with '#' and can have sign ('-' or '+') or dont have sign
*    after the character '#' and the sign can be only number.
*    if par is valid instuction number then return 1
*    if par is not valid instuction number then return 0
*/
int is_number(char *par)
{
    int i = 0;
    if (par[i++] != '#')  /*  error  */
    {
        return 0;
    }
    if ((par[i] == '-') || (par[i] == '+'))  /*  have sign, skip it  */
    {
            i++;
    }
    if (par[i] == '\0')  /*  error  */
    {
        return 0;
    }
    while (par[i] != '\0')
    {
        if ((isdigit(par[i++]) == 0)) /*  error  */
        {
            return 0;
        }
    }
    /*  if code reach here word are valid  */
    return 1;  /*  valid word  */
}

int is_valid_number(char *par)
{
    int tmp;
    tmp = atoi(par+1);
    if ((MIN_SIZE_21_DIGITS <= tmp) && (tmp <= MAX_SIZE_21_DIGITS))
    {
        return 1;
    }
    if (tmp < MIN_SIZE_21_DIGITS)  /*  error  */
    {
        return -1;
    }
    if (tmp > MAX_SIZE_21_DIGITS)  /*  error  */
    {
        return -2;
    }
    return 0; /*  defulte error  */
}

/*
*    is_register:  the function receive 1 parameter and return nothing
*    the first parameter is string named par, par have the parameter to be checked
*    the function check if par is register name
*    the registers are r1,r2,r3,r4,r5,r6
*    if par is register name then return 1
*    if par is not register name then return 0
*/
int is_register(char *par)
{
    if ((strcmp(par,"r1") == 0) || (strcmp(par,"r2") == 0) || (strcmp(par,"r3") == 0) || 
        (strcmp(par,"r4") == 0) || (strcmp(par,"r5") == 0) || (strcmp(par,"r6") == 0))
    {
        return 1;  /*  valid word  */
    }
    else  /*  error  */
    {
        return 0;
    }
}

/*
*    is_register:  the function receive 1 parameter and return nothing
*    the first parameter is string named par, par have the parameter to be checked
*    the function check if par is symbol's label name
*    the symbol's label name start with alpha character and after the first character only alpha characters or numbers 
*    if par is symbol's label name then return 1
*    if par is not symbol's label name then return 0
*/
int is_label(char *par)
{
    int i = 0;
    if (is_register(par))  /*  error  */
    {
        return 0;
    }
    if (is_command(par))  /*  error  */
    {
        return 0;
    }
    if (isalpha(par[i]) == 0)  /*  error  */
    {
        return 0;
    }
    while (par[i] != '\0')
    {
        if ((isdigit(par[i]) == 0) && (isalpha(par[i]) == 0))  /*  error  */
        {
            return 0;
        }
        i++;
    }   
    /*  if code reach here word are valid  */
    return 1;  /*  valid word  */
}

/*
*    is_register:  the function receive 1 parameter and return nothing
*    the first parameter is string named par, par have the parameter to be checked
*    the function check if par is command name
*    the command names are: mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, jsr, red, prn, rts and stop
*    if par is command name then return 1
*    if par is not command name then return 0
*/
int is_command(char *par)
{
    /*  check if par is command  */
    if ((strcmp(par,"mov") == 0) || (strcmp(par,"cmp") == 0) || (strcmp(par,"add") == 0) || (strcmp(par,"sub") == 0) ||
        (strcmp(par,"lea") == 0) || (strcmp(par,"clr") == 0) || (strcmp(par,"not") == 0) || (strcmp(par,"inc") == 0) ||
        (strcmp(par,"dec") == 0) || (strcmp(par,"jmp") == 0) || (strcmp(par,"bne") == 0) || (strcmp(par,"jsr") == 0) ||
        (strcmp(par,"red") == 0) || (strcmp(par,"prn") == 0) || (strcmp(par,"rts") == 0) || (strcmp(par,"stop") == 0))
    {
        return 1;
    }
    /*  else, par is not a command  */
    return 0;
}

