/*
    FILE:	parse_node.c
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards
    
    implementation of the parse node functions
*/

#include "parse_node.h"

// Displays the type of node a node is
// useful for debugging
void display_node_type(PARSE_NODE *n)
{
    if (n == NULL)
	printf("NULL NODE\n");
    else if ( flag_set(n, FLAG_LET))
	printf("LET\n");
    else if ( flag_set(n, FLAG_LETREC))
	printf("LETREC\n");
    else if ( flag_set(n, FLAG_RECUR))
	printf("RECUR\n");
    else if ( flag_set(n, FLAG_IF))
	printf("IF\n");
    else if ( flag_set(n, FLAG_COND))
	printf("COND\n");
    else if ( flag_set(n, FLAG_APPLICATION))
	printf("APPLICATION\n");
    else if ( flag_set(n, FLAG_LIST))
	printf("LIST\n");
    else if ( flag_set(n, FLAG_INTEGER))
	printf("INTEGER\n");
    else if ( flag_set(n, FLAG_DOUBLE))
	printf("DOUBLE\n");
    else if ( flag_set(n, FLAG_STRING))
	printf("STRING\n");
    else if ( flag_set(n, FLAG_SYMBOL))
	printf("SYMBOL\n");
    else if ( flag_set(n, FLAG_ID))
	printf("ID\n");
    else if ( flag_set(n, FLAG_ERROR))
	printf("ERROR\n");
    else if ( flag_set(n, FLAG_CONDITION))
	printf("CONDITION\n");
    else if ( flag_set(n, FLAG_ASSIGNMENT))
	printf("ASSIGNMENT\n");
    else if ( flag_set(n, FLAG_REC))
	printf("REC\n");
    else if ( flag_set(n, FLAG_PROGRAM))
	printf("PROGRAM\n");
    else if ( flag_set(n, FLAG_BOOLEAN_OP))
	printf("BOOLEAN OP\n");
    else if ( flag_set(n, FLAG_ARITH_OP))
	printf("ARITH OP\n");
    else if ( flag_set(n, FLAG_FUNCTION))
	printf("FUNCTION\n");
    else if ( flag_set(n, FLAG_BOOLEAN))
	printf("BOOLEAN\n");
    else if ( flag_set(n, FLAG_CHARACTER))
	printf("CHARACTER\n");
    else
	printf("Unknown: %d\n", n->flags);
	
}

// Displays information about a node to stdout
void display_node(PARSE_NODE *n)
{
    printf("NODE:\n");
    printf("Node type: ");
    display_node_type(n);
    printf("Line: %d\n", n->line);
    
    printf("Parent: ");
    display_node_type(n->parent);
    
    printf("Next sibling: ");
    display_node_type(n->next_sibling);
    
    printf("First Child: ");
    display_node_type(n->first_child);
    
    printf("Op Code: %d\n", n->op);
    
    printf("App Expression: ");
    display_node_type(n->app_expression);
    
    printf("Args: ");
    display_node_type(n->args);
    
    printf("Condition True: ");
    display_node_type(n->cond_true);
    
    printf("Condition False: ");
    display_node_type(n->cond_false);
    
    if (n->value)
	printf("Value: %s\n", string_value(n->value));
}

// Counts the number of siblings a node has
// Returns -1 if the node is null
int sibling_count(PARSE_NODE *n)
{
    int siblings = -1;
    PARSE_NODE *node = n;
    
    while (node != NULL)
    {
	siblings++;
	node = node->next_sibling;
    }
    
    return siblings;
}

// Counts the number of arguments for an argument list
int arg_count(PARSE_NODE *n)
{
    int siblings = 0;
    PARSE_NODE *node = n;
    
    while (node != NULL)
    {
	siblings++;
	node = node->next_sibling;
    }
    
    return siblings;
}

// Adds a child to a parent node
void add_child(PARSE_NODE *parent, PARSE_NODE *new_child)
{
    if (!parent->first_child)
    {
	parent->first_child = new_child;
	new_child->previous_sibling = NULL;
	new_child->parent = parent;
    }
    else
    {
	PARSE_NODE *tmp = parent->first_child;
	while (tmp->next_sibling != NULL) tmp = tmp->next_sibling;
	tmp->next_sibling = new_child;
	new_child->previous_sibling = tmp;
    }
}

// Adds a new sibling on the end of a sibling list
void add_sibling(PARSE_NODE *a, PARSE_NODE *b)
{
    PARSE_NODE *tmp = a;
    
    while (tmp->next_sibling != NULL) tmp = tmp->next_sibling;
    tmp->next_sibling = b;
}

// Allocates and creates a newly initialized
// parse node, then returns it
PARSE_NODE *new_parse_node()
{
    PARSE_NODE *n = (PARSE_NODE *)GC_MALLOC( sizeof(PARSE_NODE) );
    assert(n);
    n->parent = NULL;
    n->next_sibling = NULL;
    n->previous_sibling = NULL;
    n->first_child = NULL;
    n->cond_true = NULL;
    n->cond_false = NULL;
    n->app_expression = NULL;
    n->flags = 0;
    n->value = new_scalar();
    n->op = -1;
    return n;
}



