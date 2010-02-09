/*
    FILE:	builtin.h
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards
    
    This is the main header file for the built-in function
    library.
*/

#ifndef OBJECTION_BUILT_IN_H
#define OBJECTION_BUILT_IN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "objection.h"
#include "scalar.h"
#include "parse_node.h"
#include "envhash.h"
#include "interp.h"


// A very helpful macro for base_environment()
#define init_bifunc(V, K, F) \
    strcpy(built_in_keys[(V)], (K)); \
    built_in_functions[(V)] = (F) 


// Checks to make sure the correct number of arguments
// were supplied to the function, spits an error if
// an incorrect number were supplied.
#define exact_args(F, N) \
    if (arg_count(n) != (N)) { \
	sprintf(bi_buf, "Function '%s' requires %d arguments, given %d", (F), (N), arg_count(n)); \
	sem_err(bi_buf, n->line); }

#define min_args(F, M) \
    if (arg_count(n) < (M)) { \
	sprintf(bi_buf, "Function '%s' requires at least %d arguments, given %d", (F), (M), arg_count(n)); \
	sem_err(bi_buf, n->line); }


// Builtin numeric ids
#define MAX_BUILT_INS	23

// Type predicates
#define BI_IS_INTEGER	0
#define BI_IS_DOUBLE	1
#define BI_IS_NUMBER	2
#define BI_IS_STRING	3
#define BI_IS_SYMBOL	4
#define BI_IS_LIST	5
#define BI_IS_NULL	6

// I/O
#define BI_ECHO		7
#define BI_ECHOLN	8
#define BI_GET_INTEGER	9
#define BI_GET_DOUBLE	10
#define BI_GET_STRING	11
#define BI_GET_LINE	12
#define BI_GET_SYMBOL	13

// List Functions
#define BI_IS_EMPTY	14
#define BI_LENGTH	15

// Conversion Functions
#define BI_TO_INTEGER	16
#define BI_TO_DOUBLE	17
#define BI_TO_STRING	18
#define BI_TO_SYMBOL	19

// New Built-ins
#define BI_IS_CHAR	20
#define BI_GET_CHAR	21
#define BI_RAND		22


// Function Prototypes

ENVIRONMENT *base_environment();
void escape_print(const char *str);

SCALAR *bi_isinteger(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_isdouble(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_isnumber(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_isstring(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_issymbol(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_islist(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_isnull(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_echo(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_echoln(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_getinteger(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_getdouble(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_getstring(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_getline(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_getsymbol(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_isempty(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_length(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_elementat(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_tointeger(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_todouble(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_tostring(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_tosymbol(PARSE_NODE *n, ENVIRONMENT *env);

SCALAR *bi_ischaracter(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_getcharacter(PARSE_NODE *n, ENVIRONMENT *env);
SCALAR *bi_rand(PARSE_NODE *n, ENVIRONMENT *env);

#endif
