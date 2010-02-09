/*
    FILE:	parse_node.h
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards
    
    This is the header for the parse node which is used by
    the parser to construct a parse tree.
    
    
    The parse node has the following fields:
    
    parent -
	Pointer to the parent of the node 
	(null if the node is the root node).
	
    next_sibling - 
	Pointer to the node's next sibling 
	(null if the node has not next sibling).
	
    previous_sibling -
	Pointer to the node's previous sibling (null 
	if the node doesn't have a previous sibling).
    
    first_child -
	Pointer to the first child of the node
	(null if the node has no children).
	
    line -
	What line in the source file this node
	was generated from.
    
    flags -
	Node attribute flags (at this point this is
	only used to determine what type of node the
	node in question is).
    
    op -
	Short integer that holds the type of operation
	the node represents if it is a boolean or arithmetic
	operation.
    
    app_expression
	Holds a pointer to the parse node that consists of
	the expression to be applied in an application.
    
    args
	Used by for a variety of reasons, mainly it is used
	when a specific type of construct deals with values
	that are parametric (such as function applications).
	
    cond_true -
	Used to hold the true condition nodes for an if 
	statement.
    
    cond_false -
	Used to hold the false condition nodes for an if 
	statement.
    
    value -
	If the parse node represents a value, then this is
	a scalar that holds that value.
*/

#ifndef PARSE_NODE_H
#define PARSE_NODE_H

/********************************************************
*   Includes
********************************************************/
#include <stdlib.h>
#include "scalar.h"
#include "objection.h"

/********************************************************
*   Node flags, basically to tell what type of
*   node is in the parse tree.
********************************************************/

// These are flags for the language's constructs
#define FLAG_LET	    1
#define FLAG_LETREC	    2
#define FLAG_RECUR	    4
#define FLAG_IF		    8
#define FLAG_COND	    16
#define FLAG_APPLICATION    32
#define FLAG_LIST	    64
#define FLAG_INTEGER	    128
#define FLAG_DOUBLE	    256
#define FLAG_STRING	    512
#define FLAG_SYMBOL	    1024
#define FLAG_ID		    2048
#define FLAG_ERROR	    4096
#define FLAG_CONDITION	    8192
#define FLAG_ASSIGNMENT	    16384
#define FLAG_REC	    32768
#define FLAG_PROGRAM	    65536
#define FLAG_BOOLEAN_OP	    131072
#define FLAG_ARITH_OP	    262144
#define FLAG_FUNCTION	    524288
#define FLAG_BOOLEAN	    1048576
#define FLAG_LIST_OP	    2097152
#define FLAG_DEFINITION	    4194304
#define FLAG_CHARACTER	    8388608
#define FLAG_BLOCK	    16777216

// Special flags for interpreation
#define FLAG_REC2	    33554432
			    
// These are for arithmetic operations
#define ARITH_OP_ADD	0
#define ARITH_OP_SUB	1
#define ARITH_OP_MUL	2
#define ARITH_OP_DIV	3
#define ARITH_OP_MOD	4

// These are for boolean operations
#define BOOLEAN_OP_AND		0
#define BOOLEAN_OP_OR		1
#define BOOLEAN_OP_NOT		2
#define BOOLEAN_OP_XOR		3
#define BOOLEAN_OP_IMP		4
#define BOOLEAN_OP_DIMP		5
#define BOOLEAN_OP_EQ		6
#define BOOLEAN_OP_NEQ		7
#define BOOLEAN_OP_LT		8
#define BOOLEAN_OP_GT		9
#define BOOLEAN_OP_LTE		10
#define BOOLEAN_OP_GTE		11
#define BOOLEAN_OP_EQV		12

// These are for list operations
#define LIST_OP_APPEND		0
#define LIST_OP_CONS		1
#define LIST_OP_CAR		2
#define LIST_OP_CDR		3
#define LIST_OP_INTERSECTION	4
#define LIST_OP_UNION		5
#define LIST_OP_COMPLEMENT	6
#define LIST_OP_BICOMPLEMENT	7
#define LIST_OP_MEMBER		8
#define LIST_OP_SUBSET		9
#define LIST_OP_ELEMENT_AT	10


/********************************************************
*   Macros
********************************************************/

#define first_child(P)	(P)->first_child
#define next_sibling(P) (P)->next_sibling
#define prev_sibling(P) (P)->previous_sibling
#define parent(P)	(P)->parent
#define line_num(P)	(P)->line
#define op(P)		(P)->op
#define set_op(P, O)	(P)->op = (O)
#define app_expr(P)	(P)->app_expression
#define args(P)		(P)->args
#define cond_true(P)	(P)->cond_true
#define cond_false(P)	(P)->cond_false
#define value(P)	(P)->value

#define flag_set(P, F)	    ((P)->flags & (F))
#define set_flag(P, F)	    ((P)->flags = (P)->flags | (F))
#define unset_flag(P, F)    ((P)->flags = (P)->flags & ~(F))

/********************************************************
*   Parse node structure
*   For building parse trees
*   
*   As of stage two this is really officially an 
*   acyclic graph instead of a tree, due to the fact
*   that children are separated in different ways.
*
********************************************************/
typedef struct parse_node PARSE_NODE;
struct parse_node
{
    PARSE_NODE *parent;
    PARSE_NODE *next_sibling;
    PARSE_NODE *previous_sibling;
    PARSE_NODE *first_child;
    
    // Holds the line number in the source file
    long line;
    
    // Attribute Flags
    unsigned long flags;
    
    // This is used for boolean and arithmatic operations
    short op;
    
    // This is used primarily as the expression
    // portion of an application
    PARSE_NODE *app_expression;
    
    // This is used by many forms for an argument
    // extension from a parse node
    PARSE_NODE *args;
    
    // These are used by conditionals
    PARSE_NODE *cond_true;
    PARSE_NODE *cond_false;
    
    // Scalar value for the node:
    // allows for easy dynamic typing
    SCALAR *value;
};

/********************************************************
*   Node function prototypes
********************************************************/
void		display_node(PARSE_NODE *);
void		display_node_type(PARSE_NODE *);
int		sibling_count(PARSE_NODE *);
int		arg_count(PARSE_NODE *);
void            add_child(PARSE_NODE *, PARSE_NODE *);
void		add_sibling(PARSE_NODE *, PARSE_NODE *);
PARSE_NODE *    new_parse_node();

#endif

