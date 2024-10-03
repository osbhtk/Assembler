#include "assembler.h"

/*
*    add_symbol:  this function receive 7 parameters and return nothing
*    the first parameter (named head) is pointer to pointer to symbol variable,
*    this pointer point to the first linked symbol's list (if its empty, its point to NULL)
*    the second parameter is string named name, name have the label name of the symbol
*    the third parameter is types variable named type_value, type_value have the type of the symbol
*    the fourth parameter is int named DC, DC is Data Counter that count how many data/string words has been initialize 
*    the fifth parameter is int named IC, IC is Instruction Counter that count how many instuction words has been initialize
*    the sixth parameter is int pointer named error_flag, error flag will turn on if an error has been found
*    the seventh parameter is int named number_of_line, number_of_line reprecent the index of the current line from input
*    this function add new symbol to the symbol's linked list and initialize his name, type, and value by the given parameters
*    if the symbol name is already in the linked list, update is type or send error to stdout 
*    its will update the type only to DATA_AND_ENTRY or CODE_AND_ENTRY
*    its send error if trying to add CODE/DATA type to existing CODE/DATA type respectively
*    if the new symbol is valid its will be added to the last symbol in the linked list
*/
void add_symbol(symbol_pointer *head, char *name, types type_value, int DC, int IC, int *error_flag, int number_of_line)
{
    int i;
    symbol_pointer p, tmp;
    p = already_exists(head, name);  /*  check is the label named exist, if its does return pointer of the exsists symbol.
                                         if its doenst exist return NULL  */
    if (p != NULL)  /*  the name exists in linked list, update his value or send error message  */
    {
        if ((type_value == CODE && (*p).type == CODE) || (type_value == DATA && (*p).type == DATA))  /*  error  */
        {
            fprintf(stderr, "Error in line %d, the label '%s' is already exists\n",number_of_line, name);
            *error_flag = 1;
            return;
        }
        else if (((*p).type == DATA && type_value == ENTRY) || ((*p).type == ENTRY && type_value == DATA))  /*  update  */
        {
            (*p).type = DATA_AND_ENTRY;
            return;
        }
        else if (((*p).type == CODE && type_value == ENTRY) || ((*p).type == ENTRY && type_value == CODE))  /*  update  */
        {
            (*p).type = CODE_AND_ENTRY;
            return;
        }
        else if(((*p).type == CODE && type_value == EXTERNAL) || ((*p).type == DATA && type_value == EXTERNAL) ||
                ((*p).type == CODE_AND_ENTRY && type_value == EXTERNAL) || ((*p).type == DATA_AND_ENTRY && type_value == EXTERNAL))
        {
            fprintf(stderr, "Error in line %d, the label '%s' is already exists as local and local label cannot be declared as external\n",number_of_line, name);
            *error_flag = 1;
            return;
        }
        else  /*  p type is DATA_AND_ENTRY/CODE_AND_ENTRY/ENTRY/EXTERNAL and doesn't need to be change  */
        {
            return;
        }
    }
    /*  label doesn't exists in symbols and need to be added  */
    i = unvalid_name(name);
    if (i != 0)  /*  cheak if valid name of label  */
    {
        switch(i)
        {
            case 1:
                fprintf(stderr,"Error in line %d, the name '%s' is reserved word of the language\n",number_of_line, name);
                *error_flag = 1;
                break;
            case 2:
                fprintf(stderr,"Error in line %d with name '%s', the first character is not alpha\n",number_of_line, name);
                *error_flag = 1;
                break;
            case 3:
                fprintf(stderr,"Error in line %d, the name %s is exceeding the maximum size\n",number_of_line, name);
                *error_flag = 1;
                break;
            case 4:
                fprintf(stderr,"Error in line %d, the name %s is unvalid character, only alpha and digits are valid\n",number_of_line, name);
                *error_flag = 1;
                break;
        }
        return;
    }
    if (type_value == ENTRY)  /*  cheack if valid symbol type  */
    {
        fprintf(stderr,"Error in line %d, there is no symbol '%s' to put in entry\n",number_of_line, name);
        *error_flag = 1;
        return;
    }
    /*  valid label need to be added to symbol's  */
    tmp = (symbol_pointer) malloc(sizeof(symbol));  /*  create a new symbol link  */
    p = find_last(head);  /*  set the symbol pointer to the last symbol  */
    if(p == NULL)  /*  empty list, connect the head to the first given symbol  */
    {
        *head = tmp;
    }
    else  /*  not empty list, need to be connected to p  */
    {
        (*p).next = tmp;
    }
    /*  new symbol is connected to list, now set the symbol  */
    strcpy((*tmp).label,name);  /*  set the symbol's label name  */
    /*  set the symbol's value  */
    if (type_value == DATA)
    {
        (*tmp).value = DC;
    }
    if (type_value == CODE)
    {
        (*tmp).value = IC + FIRST_ADDRESS;
    }
    if (type_value == EXTERNAL)
    {
        (*tmp).value = 0;
    }
    (*tmp).type = type_value;  /*  set the symbol's type  */
    (*tmp).next = NULL;  /*  set the symbol's next  */
}




/*
*    already_exists:  this fuction receive 2 parameters and return pointer to symbol variable
*    the first parameter (named head) is pointer to pointer to symbol variable,
*    this pointer point to the first linked symbol's list (if its empty, its point to NULL)
*    the secodn parameter is string named name, name have the symbol's label name
*    the function check if the given string 'name' is already exists in the symbol's linked list
*    if the name exists then the function return pointer of the exists symbol variable
*    if the name does not exists the function return NULL
*/
symbol_pointer already_exists(symbol_pointer *head, char *name)
{   
    symbol_pointer prev, curr;  /*  set temporary symbols pointers  */
    prev = NULL;
    if (*head == NULL)  /*  empty list  */
    {
        return NULL;
    }
    curr = *head;  /*  set the curr pointer to the first symbol  */
    while (curr != NULL)  /*  find symbol with same label name  */
    {
        prev = curr;
        curr = (*curr).next;
        if (strcmp((*prev).label,name) == 0)
        {
            return prev;  /*  found symbol with same label name, return it  */
        }
    }
    return NULL;  /*  didn't found symbol with same label name  */
}

/*
*    find_last:  this function receive 1 paramerer and return poiner to symbol variable
*    the first parameter (named head) is pointer to pointer to symbol variable,
*    this pointer point to the first linked symbol's list (if its empty, its point to NULL)
*    the function find the last symbol in symbol's linked list and return pointer that point to him
*    if the symbol's linked list is empty then its return NULL
*/
symbol_pointer find_last(symbol_pointer *head)
{
    symbol_pointer prev, curr;  /*  set temporary symbols pointers  */
    prev = NULL;
    if (*head == NULL)  /*  empty list  */
    {
        return NULL;
    }
    curr = *head;  /*  set the curr pointer to the first symbol  */
    while (curr != NULL)  /*  find the last symbol in list  */
    {
        prev = curr;
        curr = (*curr).next;
    }
    return prev;  /*  return the last symbol in list  */
}

/*
*    set_symbols:  this function receive 2 parameters and return nothing
*    the first parameter (named head) is pointer to pointer to symbol variable,
*    this pointer point to the first linked symbol's list (if its empty, its point to NULL)
*    the second parameter is int named IC, IC is value of the instruction counter index
*    the function update the symbols by adding to the DATA type symbols the value of IC
*/
void set_symbols(symbol_pointer *head,int IC)
{
    symbol_pointer p;
    p = *head;
    while (p != NULL)  /*  loop to check all the symbols in the linked list  */
    {
        if ((*p).type == DATA)  /*  if its DATA type, update his value  */
        {
            (*p).value += IC + FIRST_ADDRESS;
        }
        p = (*p).next;
    }
}

/*
*    free_symbol:  this function receive 1 parameter and return nothing
*    the first parameter (named head) is pointer to pointer to symbol variable,
*    this pointer point to the first linked symbol's list (if its empty, its point to NULL)
*    the function free all the symbols in the linked list from the first to the last
*/
void free_symbol(symbol_pointer *head)
{
    symbol *tmp;  /*  temporary pointer to hold the symbol to be free  */
    while(*head != NULL)  /*  loop to free all the symbols  */
    {
        tmp = *head;
        *head = (*tmp).next;
        free(tmp);
    }
}

/*
*    unvalid_name:  this function receive 1 parameter and return int
*    the first parameter is string named name, name have the name to be check if is valid
*    the function check if the given name is valid,
*    if name is valid name then return 0
*    if name is reserved word of the language then return 1
*    if the first character is not alpha then return 2
*    if the name is exceeding the maximum size then return 3
*    if name have unvalid character then return 4
*/
int unvalid_name(char *name)
{
    char c;
    int i = 0;
    c = name[i];
    if ((is_register(name) == 1) || (is_command(name) == 1))
    {
        return 1;  /*  reserved word of the language  */
    }
    if (isalpha(c) == 0)
    {
        return 2;  /*  the first character is not alpha  */
    }
    if (strlen(name) > 31)
    {
        return 3;  /*  exceeding the maximum size  */
    }
    while (c != '\0')
    {
        if ((isalpha(c) == 0) && (isdigit(c) == 0))
        {
            return 4;  /*  unvalid character, only alpha and digits are valid  */
        }
        c = name[++i];  /*  set c to the next character  */
    }

    return 0;  /*  valid name  */
}

