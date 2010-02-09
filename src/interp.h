/*
    FILE:	interp.h
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards
    
    This is the main header file for the interpreter.
*/

#ifndef INTERP_H
#define INTERP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "objection.h"
#include "scalar.h"
#include "envhash.h"
#include "parse_node.h"

/********************************************************
*   The continuation structure
********************************************************/
typedef struct continuation CONTINUATION;
struct continuation
{
    ENVIRONMENT *env;
    PARSE_NODE *function_tree;
};

// Display function for semantic errors
void sem_err(char *msg, unsigned int line);

// Simple interpreter, only prints the parse tree
void print_node(PARSE_NODE *n);
void print_tree(PARSE_NODE *n, int depth);

// These two allow for "quick and dirty" interpretation
// a precurser to the objection compiler
void forward_functions(PARSE_NODE *n, ENVIRONMENT *env);
void forward_definitions(PARSE_NODE *n, ENVIRONMENT *env);
void interpret_main(ENVIRONMENT *env);
SCALAR *interpret_rec(PARSE_NODE *n, PARSE_NODE *rn, ENVIRONMENT *env);
SCALAR *interpret(PARSE_NODE *n, ENVIRONMENT *env);

#endif


