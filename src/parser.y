/*
    FILE:	parser.y
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards
    
    This file describes a parser for objection stage 2.
*/

/* Preliminary C Code */
%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include "objection.h"
    #include "envhash.h"
    #include "parse_node.h"
    #include "scalar.h"
    #include "interp.h"
    #include "builtin.h"
    #include "mlor.h"
    
    // Turn parser debugging on
    #define YYDEBUG 1
    
    PARSE_NODE *functions;
    PARSE_NODE *definitions;
    
    PARSE_NODE *tmp;
     
    int options = 0;
    
    // Options for the interpreter
    #define OPT_TREE_ONLY 1
    
    // Functions needed to make life bareable
    int id_list(PARSE_NODE *n);
    PARSE_NODE *build_op_node(long type, long op, PARSE_NODE *left, PARSE_NODE *right, long line);
    
%}

/* Define the stack type */
%union {
    long ival;
    PARSE_NODE *nval;
}

/* Tokens and types, and the start symbol */

%token <ival> OPEN_BRACE CLOSE_BRACE OPEN_PAREN CLOSE_PAREN
%token <ival> OPEN_BRACKET CLOSE_BRACKET
%token <ival> LET FOR LAMBDA IF ELSE COND LIST DEFAULT
%token <ival> ASSIGN SEMICOLON ERROR BOOLEAN_OPENER CASE
%token <ival> FUNCTION COMMA REC RECUR ON DEFINE AS

%token <ival> BOOLEAN_AND BOOLEAN_OR BOOLEAN_XOR BOOLEAN_NOT 
%token <ival> BOOLEAN_IMP BOOLEAN_DIMP BOOLEAN_EQ BOOLEAN_NEQ
%token <ival> BOOLEAN_LT BOOLEAN_GT BOOLEAN_LTE BOOLEAN_GTE
%token <ival> BOOLEAN_EQV

%token <ival> LIST_APPEND LIST_CONS LIST_CAR LIST_CDR LIST_ELEMENT_OF
%token <ival> LIST_UNION LIST_INTERSECTION LIST_COMPLEMENT
%token <ival> LIST_BICOMPLEMENT LIST_MEMBER LIST_SUBSET

%token <ival> ARITH_ADD ARITH_SUB ARITH_MUL ARITH_DIV ARITH_MOD

%token <nval> INTEGER DOUBLE STRING SYMBOL BOOLEAN ID CHARACTER

%type  <nval> expression_list expression condition_list condition 
%type  <nval> assignment_list assignment literal statement form form_list
%type  <nval> function_definition assignment_form

%start program

/* These are the operator prcedence rules */
%left BOOLEAN_AND BOOLEAN_OR BOOLEAN_XOR BOOLEAN_DIMP BOOLEAN_IMP
%left BOOLEAN_LT BOOLEAN_GT BOOLEAN_LTE BOOLEAN_GTE BOOLEAN_EQ BOOLEAN_NEQ BOOLEAN_EQV LIST_MEMBER LIST_SUBSET
%left BOOLEAN_NOT
%right LIST_CONS
%left LIST_APPEND
%right LIST_CAR LIST_CDR
%left ARITH_ADD ARITH_SUB LIST_UNION LIST_INTERSECTION LIST_COMPLEMENT LIST_BICOMPLEMENT
%left ARITH_MUL ARITH_DIV ARITH_MOD

%%
/**************************************************************
    Grammar
**************************************************************/
program: 
    program definition | definition
    ;

definition:
    function_definition { add_child(functions, $1); }
    
    |
    
    DEFINE ID AS form
    {
	tmp = new_parse_node();
	set_flag(tmp, FLAG_DEFINITION);
	tmp->value = $2->value;
	add_child(tmp, $4);
	tmp->line = $1;
	
	add_child(definitions, tmp);
    }
    ;

function_definition:
    FUNCTION ID OPEN_PAREN CLOSE_PAREN OPEN_BRACE form_list CLOSE_BRACE
    {
	tmp = new_parse_node();
	tmp->line = $1;
	set_flag(tmp, FLAG_FUNCTION);
	add_child(tmp, $6);
	tmp->value = $2->value;
	$$ = tmp;
    }
    
    |
    
    FUNCTION ID OPEN_PAREN expression_list CLOSE_PAREN OPEN_BRACE form_list CLOSE_BRACE
    {
	if (!id_list($4))
	{
	    extern long errors;
	    errors++;
	    printf("Syntax Error near line %d:\n\t%s\n", $3, 
		"function definitions can contain only identifiers for their parameter lists");
	}
	
	tmp = new_parse_node();
	tmp->line = $1;
	set_flag(tmp, FLAG_FUNCTION);
	tmp->args = $4;
	add_child(tmp, $7);
	tmp->value = $2->value;
	$$ = tmp;
    }
    ;


form_list:
    form_list form 
    {
	add_sibling($1, $2);
	$$ = $1;
    }
    
    | form
    ;


form:
    statement 
    |
    OPEN_BRACE form_list CLOSE_BRACE 
    {
	// Beautiful blocks, :'( (tee hee hee)
	tmp = new_parse_node();
	set_flag(tmp, FLAG_BLOCK);
	add_child(tmp, $2);
	tmp->line = $1;
	$$ = tmp; 
    } 
    |
    expression SEMICOLON    { $$ = $1; }
    ;


statement:
    LET assignment_form FOR form
    {
	tmp = new_parse_node();
	set_flag(tmp, FLAG_LET);
	tmp->line = $1;
	tmp->args = $2;
	add_child(tmp, $4);
	$$ = tmp;
    }
    
    |
    
    IF OPEN_PAREN expression CLOSE_PAREN form ELSE form
    {
	tmp = new_parse_node();
	set_flag(tmp, FLAG_IF);
	tmp->line = $1;
	tmp->args = $3;
	tmp->cond_true = $5;
	tmp->cond_false = $7;
	$$ = tmp;
    }
    
    |
    
    COND OPEN_BRACE condition_list CLOSE_BRACE
    {
	// Conds are structured like so:
	// children - conditions
	tmp = new_parse_node();
	set_flag(tmp, FLAG_COND);
	tmp->line = $1;
	add_child(tmp, $3);
	$$ = tmp;
    }
    
    |
    
    ERROR OPEN_PAREN expression CLOSE_PAREN form
    {
	tmp = new_parse_node();
	set_flag(tmp, FLAG_ERROR);
	tmp->line = $1;
	tmp->args = $3;
	add_child(tmp, $5);
	$$ = tmp;
    }
    
    |
    
    REC OPEN_PAREN expression_list CLOSE_PAREN 
    OPEN_BRACE form_list CLOSE_BRACE ON 
    OPEN_PAREN expression_list CLOSE_PAREN
    {
	// Rec expressions are like this
	//
	// args		  - the id list for the rec
	// children	  - body of the rec
	// app_expression - the values to begin recurring on
    
	if (!id_list($3))
	{
	    extern long errors;
	    errors++;
	    printf("Syntax Error near line %d:\n\t%s\n", $3, 
		"function definitions can contain only identifiers for their parameter lists");
	}
    
	tmp = new_parse_node();
	set_flag(tmp, FLAG_REC);
	tmp->line = $1;
	tmp->args = $3;
	add_child(tmp, $6);
	tmp->app_expression = $10;
	$$ = tmp;
    }   
    ;

expression_list:
    expression_list COMMA expression 
    {
	add_sibling($1, $3);
	$$ = $1;
    }
    
    | 
    
    expression
    ;

expression:

    ID OPEN_PAREN CLOSE_PAREN
    {
	tmp = new_parse_node();
	set_flag(tmp, FLAG_APPLICATION);
	tmp->line = $1->line;
	tmp->app_expression = $1;
	$$ = tmp;
    }
    
    |

    ID OPEN_BRACE form_list CLOSE_BRACE
    {
	// Applications are structured like so
	// app_expression   - expression to be applied
	// children	    - the arguments
	tmp = new_parse_node();
	set_flag(tmp, FLAG_APPLICATION);
	tmp->line = $1->line;
	tmp->app_expression = $1;
	add_child(tmp, $3);
	$$ = tmp;
    }
    
    |
    
    ID OPEN_PAREN expression_list CLOSE_PAREN
    {
	// Applications are structured like so
	// app_expression   - expression to be applied
	// children	    - the arguments
	tmp = new_parse_node();
	set_flag(tmp, FLAG_APPLICATION);
	tmp->line = $1->line;
	tmp->app_expression = $1;
	add_child(tmp, $3);
	$$ = tmp;
    }
    
    |
    
    LIST OPEN_PAREN CLOSE_PAREN
    {
	tmp = new_parse_node();
	set_flag(tmp, FLAG_LIST);
	tmp->line = $1;
	$$ = tmp;
    }
    
    |
    
    LIST OPEN_BRACE form_list CLOSE_BRACE
    {
	// Lists are pretty basic
	// children - expression to make as elements
	tmp = new_parse_node();
	set_flag(tmp, FLAG_LIST);
	tmp->line = $1;
	add_child(tmp, $3);
	$$ = tmp;
    }
    
    |
    
    LIST OPEN_PAREN expression_list CLOSE_PAREN
    {
	// Lists are pretty basic
	// children - expression to make as elements
	tmp = new_parse_node();
	set_flag(tmp, FLAG_LIST);
	tmp->line = $1;
	add_child(tmp, $3);
	$$ = tmp;
    }
    
    |
    
    RECUR OPEN_PAREN expression_list CLOSE_PAREN
    {
	// A recur statement
	// children - items to recur on
    
	tmp = new_parse_node();
	set_flag(tmp, FLAG_RECUR);
	tmp->line = $1;
	add_child(tmp, $3);
	$$ = tmp;
    }
    
    |
    
    RECUR OPEN_BRACE form_list CLOSE_BRACE
    {
	tmp = new_parse_node();
	set_flag(tmp, FLAG_RECUR);
	tmp->line = $1;
	add_child(tmp, $3);
	$$ = tmp;
    }
     
    |
    
    ID OPEN_BRACKET expression CLOSE_BRACKET
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_ELEMENT_AT, $1, $3, $1->line); }
    
    |
    
    expression ARITH_ADD expression
    { $$ = build_op_node(FLAG_ARITH_OP, ARITH_OP_ADD, $1, $3, $2); }
    
    |
    
    expression ARITH_SUB expression
    { $$ = build_op_node(FLAG_ARITH_OP, ARITH_OP_SUB, $1, $3, $2); }
    
    |
    
    expression ARITH_MUL expression
    { $$ = build_op_node(FLAG_ARITH_OP, ARITH_OP_MUL, $1, $3, $2); }
    
    |
    
    expression ARITH_DIV expression
    { $$ = build_op_node(FLAG_ARITH_OP, ARITH_OP_DIV, $1, $3, $2); }
    
    |
    
    expression ARITH_MOD expression
    { $$ = build_op_node(FLAG_ARITH_OP, ARITH_OP_MOD, $1, $3, $2); }
    
    |
    
    BOOLEAN_NOT expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_NOT, $2, NULL, $1); }
    
    |
    
    expression BOOLEAN_AND expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_AND, $1, $3, $2); }
    
    |
    
    expression BOOLEAN_OR expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_OR, $1, $3, $2); }
    
    |
    
    expression BOOLEAN_XOR expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_XOR, $1, $3, $2); }
    
    |
    
    expression BOOLEAN_IMP expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_IMP, $1, $3, $2); }
    
    |
    
    expression BOOLEAN_DIMP expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_DIMP, $1, $3, $2); }

    |
    
    expression BOOLEAN_EQ expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_EQ, $1, $3, $2); }
    
    |
    
    expression BOOLEAN_NEQ expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_NEQ, $1, $3, $2); }

    |
    
    expression BOOLEAN_LT expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_LT, $1, $3, $2); }
    
    |
    
    expression BOOLEAN_GT expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_GT, $1, $3, $2); }
    
    |
    
    expression BOOLEAN_LTE expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_LTE, $1, $3, $2); }
    
    |
    
    expression BOOLEAN_GTE expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_GTE, $1, $3, $2); }
    
    |
    
    expression BOOLEAN_EQV expression
    { $$ = build_op_node(FLAG_BOOLEAN_OP, BOOLEAN_OP_EQV, $1, $3, $2); }
        
    |
    
    LIST_CAR expression
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_CAR, $2, NULL, $1); }
    
    |
    
    LIST_CDR expression
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_CDR, $2, NULL, $1); }
    
    |
    
    expression LIST_CONS expression
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_CONS, $1, $3, $2); }
    
    |
    
    expression LIST_APPEND expression
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_APPEND, $1, $3, $2); }
    
    |
    
    expression LIST_UNION expression
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_UNION, $1, $3, $2); }
    
    |
    
    expression LIST_INTERSECTION expression
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_INTERSECTION, $1, $3, $2); }
    
    |
    
    expression LIST_COMPLEMENT expression
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_COMPLEMENT, $1, $3, $2); }
    
    |
    
    expression LIST_BICOMPLEMENT expression
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_BICOMPLEMENT, $1, $3, $2); }
    
    |
    
    expression LIST_MEMBER expression
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_MEMBER, $1, $3, $2); }
    
    |
    
    expression LIST_SUBSET expression
    { $$ = build_op_node(FLAG_LIST_OP, LIST_OP_SUBSET, $1, $3, $2); }
    
    |
    
    OPEN_PAREN expression CLOSE_PAREN { $$ = $2; } | ID | literal 
    ;

/*
    Conditions
*/
condition_list:
    condition_list condition
    {
	add_sibling($1, $2);
	$$ = $1;
    }
    
    |
    
    condition
    ;

condition:
    CASE OPEN_PAREN expression CLOSE_PAREN form
    {
	// args - condition
	// children expression list
	tmp = new_parse_node();
	set_flag(tmp, FLAG_CONDITION);
	tmp->line = $3->line;
	tmp->args = $3;
	add_child(tmp, $5);
	$$ = tmp;
    }
    
    |
    
    DEFAULT form
    {
	// With the default case we have
	// args	    - NULL
	// children - expression list
	tmp = new_parse_node();
	set_flag(tmp, FLAG_CONDITION);
	tmp->line = $1;
	add_child(tmp, $2);
	$$ = tmp;
    }
    ;

/*
    Assignments
    
    assign_form :=  { <assign_list> } | <assign>
    assign_list :=  <assign_list> <assign>
    assign	:=  <id> = <form>
*/
assignment_form:
    OPEN_BRACE assignment_list CLOSE_BRACE  { $$ = $2; }
    
    |
    
    assignment_list
    {
	if (sibling_count($1))
	{
	    extern long errors;
	    errors++;
	    fprintf(stderr,
		"Syntax error near line %d:\n\tWithout parenthesis, there can be only one assignment",
		$1->line
		);
	}
	
	$$ = $1;
    }
    ;

assignment_list:
    assignment_list assignment
    {
	add_sibling($1, $2);
	$$ = $1;
    } 
    
    |
    
    assignment
    ;

assignment:
    ID ASSIGN form
    {
	// The layout of an assignment
	// args     - id to assign to
	// children - epression of assignment
	tmp = new_parse_node();
	set_flag(tmp, FLAG_ASSIGNMENT);
	tmp->line = $1->line;
	tmp->args = $1;
	add_child(tmp, $3);
	$$ = tmp;
    }
    ;

/*
    Literals
*/
literal:
    INTEGER | DOUBLE | STRING | CHARACTER | SYMBOL | BOOLEAN
    ;



%%
/**************************************************************
    Additional C Code
**************************************************************/

// Returns true iff every a parse node and
// each sibling are identifiers
int id_list(PARSE_NODE *n)
{
    if (n == NULL)
	return 0;
    
    while (n != NULL)
	if (flag_set(n, FLAG_ID))
	    n = n->next_sibling;
	else
	    return 0;
    
    return 1;
}

// Constructs an operator node given the correct values
PARSE_NODE *build_op_node(long type, long op, PARSE_NODE *left, PARSE_NODE *right, long line)
{
    PARSE_NODE *tmp_n = new_parse_node();
    set_flag(tmp_n, type);
    set_op(tmp_n, op);
    
    add_child(tmp_n, left);
    
    if (right != NULL)
	add_child(tmp_n, right);
    
    tmp_n->line = line;
    
    return tmp_n;
}

int main(int argc, char **argv)
{
    // Initialize the garbage collector
    GC_init();
 
    extern FILE *yyin;
    extern long errors;
    
    int tree_only = 0;
    int mlor_only = 0;
    
    argv++;
    if (argv[0] == NULL)
    {
	fprintf(stderr, "usage: stage2 [-t|-m] <filename>\n");
	fflush(stderr);
	return 0;
    }
    
    // Check for options
    if (!strcmp(argv[0], "-t"))
    {
	tree_only = 1;
	argv++;
	if (argv[0] == NULL)
	{
	    fprintf(stderr, "usage: stage2 [-t|-m] <filename>\n");
	    fflush(stderr);
	    return 0;
	}
    }
    else if (!strcmp(argv[0], "-m"))
    {
	mlor_only = 1;
	argv++;
	if (argv[0] == NULL)
	{
	    fprintf(stderr, "usage: stage2 [-t|-m] <filename>\n");
	    fflush(stderr);
	    return 0;
	}
    }
    
    // Set up the root nodes
    functions = new_parse_node();
	set_flag(functions, FLAG_PROGRAM);
    definitions = new_parse_node();
	set_flag(definitions, FLAG_PROGRAM);
    
    // Open the file and parse its contents
    yyin = fopen(argv[0], "r");
    yyparse();
    
    // If there were errors we are done
    // Otherwise, dependant upon options
    // either show the tree or interpret
    // the file.
    if (errors)
	printf("\n%d Parse Error%s Detected\n", errors, errors == 1 ? "" : "s");
    else
    {
	if (tree_only)
	{
	    // Simply print the parse tree
	    printf("Functions:\n--------------------\n");
	    print_tree(functions, 0);
	    printf("--------------------\n\nDefinitions:\n--------------------\n");
	    print_tree(definitions, 0);
	    printf("--------------------\n");
	}
	else if (mlor_only)
	{
	    MLOR_BLOCK *prg = new_block();
	    translate_node(functions->first_child->first_child, prg, 1);
	    print_mlor(prg->first);
	}
	else
	{
	    // Setup the base environments
	    ENVIRONMENT *base = base_environment();
	    ENVIRONMENT *defs = new_env();
	    defs->lower = base;
	    
	    // Call the forward referencers
	    forward_functions(functions,  defs);
	    forward_definitions(definitions, defs);
	    
	    // Interpret the main function
	    interpret_main(defs);
	}
    }
    
    return 0;
}



