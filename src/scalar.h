/*
    FILE:	scalar.h
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards
    
    Header file for the scalar library, newly revamped to
    take advantage of the nice garbage collection that
    is being used.
*/


#ifndef SCALAR_H
#define SCALAR_H

/********************************************************
*   Includes
********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/errno.h>
#include "objection.h"

/********************************************************
*   Scalar and Scalar List Macros
* 
*   Macros a hella faster than functions, so I am
*   considering replacing a lot of the functions
*   like accessors and mutators with macros to
*   increase the speed of the interpreter and
*   the parser.
*
*   This is a definate for stage 3
*
********************************************************/
#define empty_list(L) \
    ((L)->type == SLT_HEADER && (L)->next->type == SLT_FOOTER) || \
    ((L)->type == SLT_FOOTER && (L)->prev->type == SLT_HEADER)
    
#define is_head(L)  ((L)->type == SLT_HEADER)
#define is_foot(L)  ((L)->type == SLT_FOOTER)
#define is_data(L)  ((L)->type == SLT_VALUE)

#define sc_value(L) ((L)->value)

#define sc_type(S)  ((S)->type)

#define to_list(S)  (S)->type == ST_LIST ? (SCALAR_LIST *)(S)->value : NULL

/********************************************************
*   Scalar types (edit as needed)
********************************************************/
typedef enum scalar_type SCALAR_TYPE;
enum scalar_type 
{ 
    ST_UNDEFINED = 0, 
    ST_LONG = 1, 
    ST_DOUBLE = 2, 
    ST_STRING = 3,
    ST_BOOLEAN = 4,
    ST_CONTINUATION = 5,
    ST_BUILT_IN = 6,
    ST_SYMBOL = 7,
    ST_LIST = 8,
    ST_NULL = 9,
    ST_CHARACTER = 10,
    ST_FILE = 11,
    ST_REGISTER = 12 // This is only used by MLOR
};

/********************************************************
*   Scalar errors (edit as needed)
********************************************************/
typedef enum scalar_error SCALAR_ERROR;
enum scalar_error 
{ 
    SE_NONE, 
    SE_NULL_SCALAR, 
    SE_NULL_VALUE, 
    SE_VALUE_REALLOC,
    SE_NULL_STRING,
    SE_CIRCULAR_REFERENCE,
    SE_RANGE,
    SE_INVALID_TYPE_CAST
};

/********************************************************
*   Global error variable
********************************************************/
SCALAR_ERROR scalar_error;

/********************************************************
*   The scalar structure
********************************************************/
typedef struct scalar SCALAR;
struct scalar
{
    SCALAR_TYPE type;
    void *value;
    SCALAR *next_op;
};

/********************************************************
*   The scalar list structure
********************************************************/
typedef enum scalar_list_type SCALAR_LIST_TYPE;
enum scalar_list_type
{
    SLT_HEADER,
    SLT_VALUE, 
    SLT_FOOTER
};

typedef struct scalar_list SCALAR_LIST;
struct scalar_list
{
    SCALAR_LIST_TYPE type;
    SCALAR_LIST *next;
    SCALAR_LIST *prev;
    SCALAR *value;
};

/********************************************************
*   Function definitions
********************************************************/

// Only used for debugging, and very useful at that!
void display_scalar_type(SCALAR *s);

// Main scalar functions
void scalar_error_out(const char *pre);
SCALAR *new_scalar();

int set_long_value(SCALAR *scalar, long i);
int set_bool_value(SCALAR *scalar, long b);
int set_double_value(SCALAR *scalar, double d);
int set_string_value(SCALAR *scalar, char *s, unsigned int len);
int set_symbol_value(SCALAR *scalar, char *s);
int set_char_value(SCALAR *scalar, char s);

long long_value(SCALAR *s);
double double_value(SCALAR *s);
char *string_value(SCALAR *s);
char char_value(SCALAR *s);
SCALAR *copy_scalar(SCALAR *s);

long eq_scalar(SCALAR *a, SCALAR *b);
long lt_scalar(SCALAR *a, SCALAR *b);
long gt_scalar(SCALAR *a, SCALAR *b);
long eqv_scalar(SCALAR *a, SCALAR *b);


int set_file_value(SCALAR *scalar, FILE *f);
FILE *file_value(SCALAR *s);

// Scalar list functions
void set_list_value(SCALAR *scalar, SCALAR_LIST *list);
int empty_list_scalar(SCALAR *scalar);
SCALAR_LIST *list_value(SCALAR *scalar);
SCALAR_LIST *new_scalar_list();
SCALAR_LIST *new_list_entry(SCALAR_LIST *prev, SCALAR_LIST *next, SCALAR *val);
SCALAR_LIST *copy_list(SCALAR_LIST *list);
SCALAR_LIST *append(SCALAR_LIST *list, SCALAR *value);
SCALAR_LIST *insert_front(SCALAR_LIST *list, SCALAR *value);

SCALAR *get_from_list(SCALAR_LIST *list, long index);
long sl_length(SCALAR_LIST *list);

#endif


