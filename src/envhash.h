/*
    FILE:	envhash.h
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards
    
    This is part of a library for an environment hash
    structure and coorisponding functions.
*/

#ifndef ENVHASH_H
#define ENVHASH_H

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "objection.h"
#include "scalar.h"

/********************************************************
*   Error handling
********************************************************/
typedef enum environment_error ENVIRONMENT_ERROR;
enum environment_error
{
    EE_NONE,
    EE_NULL_ARGUMENT,
    EE_NULL_BUCKET,
    EE_NULL_BUCKET_VALUE,
    EE_NULL_ENVIRONMENT,
    EE_SCALAR_NOT_FOUND
};

ENVIRONMENT_ERROR env_error;


/********************************************************
*   bucket_entry structure
********************************************************/
typedef struct bucket_entry BUCKET_ENTRY;

struct bucket_entry
{
    /* Reference to the next bucket entry */
    BUCKET_ENTRY *next;
    BUCKET_ENTRY *previous;
    
    /* String key, and type of the entry */
    char *key;
    SCALAR *value;
};

/********************************************************
*   Environment structure
********************************************************/
#define MAX_ENTRIES	47

typedef struct environment ENVIRONMENT;

struct environment
{
    /* 
	Lower and higher environments...
	(see stage1.readme for more information) 
    */
    ENVIRONMENT *lower;
    ENVIRONMENT *higher;	
    
    /* 
	Hashtable map fpr the values of identifiers
	in the environment.
    */
    BUCKET_ENTRY *entries[MAX_ENTRIES];
};




/********************************************************
*   Function prototypes
********************************************************/

void display_env(ENVIRONMENT *env);
void eo(char *s);

/* The error message function */
void env_error_out(char *str);

/* Hashing specific */	   
unsigned int hash(const char *hash_string);

/* Bucket entry specific */
BUCKET_ENTRY *new_bucket_entry(const char *key);

/* Environment specific */
ENVIRONMENT *new_env();
void put_in_env(ENVIRONMENT *env, char *key, SCALAR *scalar);
SCALAR *get_from_env(ENVIRONMENT *env, char *key);
void remove_from_env(ENVIRONMENT *env, char *key);

/* Utility environment functions */
void set_lower(ENVIRONMENT *a, ENVIRONMENT *b);	    // Macro this
void set_higher(ENVIRONMENT *a, ENVIRONMENT *b);    // Macro this
SCALAR *get_highest(ENVIRONMENT *top, char *key);
void remove_highest(ENVIRONMENT *top, char *key);

#endif


