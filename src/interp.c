/*
    FILE:	interp.c
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards

    This is the language interpeter for Objection stage 1,
    lots of code, skinny on the comments, need to fix that
    in the next stage, this will do for now though.
*/

#include "interp.h"

void lntab(int l, int d)
{
    printf("Line %3d:", l);
    
    int t;
    for (t = 0; t < d; t++)
	printf("    ");
}


void print_tree(PARSE_NODE *n, int depth)
{
    PARSE_NODE *tmp;
    char buf[256];
    
    if (n == NULL)
    {
	printf("print_tree(): NULL node encountered\n");
	return;
    }
    
    // Program
    if ( flag_set(n, FLAG_PROGRAM) )
    {
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	
	return;
    }
    
    // Function
    if ( flag_set(n, FLAG_FUNCTION) )
    {
	lntab(n->line, depth);
	printf("function defined as %s\n", string_value(n->value));
	
	lntab(n->line, depth);
	printf("with parameters named\n");
	
	for (tmp = n->args; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	
	lntab(n->line, depth);
	printf("and function body of\n");
		
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	
	return;
    }
    
    // Definition
    if ( flag_set(n, FLAG_DEFINITION) )
    {
	lntab(n->line, depth);
	printf("Definition of identifier '%s' as:\n", string_value(n->value));
	print_tree(n->first_child, depth + 1);
	return;
    }
    
    // rec
    if ( flag_set(n, FLAG_REC) )
    {
	lntab(n->line, depth);
	printf("rec with parameters\n");
	
	for (tmp = n->args; tmp; tmp = tmp->next_sibling)
	    print_tree(tmp, depth + 1);
	
	lntab(n->line, depth);
	printf("body of\n");
	
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	    print_tree(tmp, depth + 1);
	
	lntab(n->line, depth);
	printf("applied to\n");
	
	for (tmp = n->app_expression; tmp; tmp = tmp->next_sibling)
	    print_tree(tmp, depth + 1);
	
	return;
    }
    
    
    // recur
    if ( flag_set(n, FLAG_RECUR) )
    {
	lntab(n->line, depth);
	printf("recur on\n");
	
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	    print_tree(tmp, depth + 1);
	    
	return;
    }
    
    
    // let
    if ( flag_set(n, FLAG_LET) )
    {
	lntab(n->line, depth);
	printf("let\n");
		
	for (tmp = n->args; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	
	lntab(n->line, depth);
	printf("for\n");
	
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	
	return;
    }
    
    // if
    if ( flag_set(n, FLAG_IF) )
    {
	lntab(n->line, depth);
	printf("if\n");
	print_tree(n->args, depth + 1);
	
	lntab(n->line, depth);
	printf("then\n");
	
	for (tmp = n->cond_true; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	
	lntab(n->line, depth);
	printf("else\n");
	
	for (tmp = n->cond_false; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	
	return;
    }
    
    // cond
    if ( flag_set(n, FLAG_COND) )
    {
	lntab(n->line, depth);
	printf("cond\n");
	
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	
	return;
    }
    
    // Application
    if ( flag_set(n, FLAG_APPLICATION) )
    {
	lntab(n->line, depth);
	printf("application of\n");
	print_tree(n->app_expression, depth + 1);
	
	lntab(n->line, depth);
	printf("with\n");
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	return;
    }
    
    // List
    if ( flag_set(n, FLAG_LIST) )
    {
	lntab(n->line, depth);
	printf("list containing\n");
	
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	return;
    }
    
    // Integer
    if ( flag_set(n, FLAG_INTEGER) )
    {
	lntab(n->line, depth);
	printf("integer: %d\n", long_value(n->value));
	return;
    }
    
    // Double
    if ( flag_set(n, FLAG_DOUBLE) )
    {
	lntab(n->line, depth);
	printf("double: %f\n", double_value(n->value));
	return;
    }
    
    // Boolean
    if ( flag_set(n, FLAG_BOOLEAN) )
    {
	lntab(n->line, depth);
	printf("boolean: %s\n", string_value(n->value));
	return;
    }
    
    // String
    if ( flag_set(n, FLAG_STRING) )
    {
	lntab(n->line, depth);
	printf("string: \"%s\"\n", string_value(n->value));
	return;
    }
    
    // Character
    if ( flag_set(n, FLAG_CHARACTER) )
    {
	lntab(n->line, depth);
	printf("character: '%s'\n", string_value(n->value));
	return;
    }
    
    // Symbol
    if ( flag_set(n, FLAG_SYMBOL) )
    {
	lntab(n->line, depth);
	printf("symbol: $%s\n", string_value(n->value));
	
	return;
    }
    
    // Identifier
    if ( flag_set(n, FLAG_ID) )
    {
	lntab(n->line, depth);
	printf("identifier: %s\n", string_value(n->value));
	
	return;
    }
    
    // Error
    if ( flag_set(n, FLAG_ERROR) )
    {
	lntab(n->line, depth);
	printf("error\n");
	
	lntab(n->line, depth);
	printf("on condition\n");
	
	print_tree(n->args, depth + 1);
	
	lntab(n->line, depth);
	printf("with consequence(s) of\n");
	
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	
	return;
    }
    
    // A condition
    if ( flag_set(n, FLAG_CONDITION) )
    {
	lntab(n->line, depth);
	
	if (n->args == NULL)
	    printf("default condition\n");
	else
	{
	    printf("condition\n");
	    print_tree(n->args, depth+1);
	}
	
	lntab(n->line, depth);
	printf("with body\n");
	
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    print_tree(tmp, depth + 1);
	}
	
	return;
    }
    
    // Assignment
    if ( flag_set(n, FLAG_ASSIGNMENT) )
    {
	lntab(n->line, depth);
	printf("assignment of identifier %s to\n", string_value(n->args->value));
	print_tree(n->first_child, depth+1);
	return;
    }
    
    // Boolean Operation
    if ( flag_set(n, FLAG_BOOLEAN_OP) )
    {
	switch (n->op)
	{
	    case BOOLEAN_OP_AND:    strcpy(buf, "and");	    break;
	    case BOOLEAN_OP_OR:	    strcpy(buf, "or");	    break;
	    case BOOLEAN_OP_NOT:    strcpy(buf, "not");	    break;
	    case BOOLEAN_OP_XOR:    strcpy(buf, "xor");	    break;
	    case BOOLEAN_OP_IMP:    strcpy(buf, "imp");	    break;
	    case BOOLEAN_OP_DIMP:   strcpy(buf, "dimp");    break;
	    case BOOLEAN_OP_EQ:	    strcpy(buf, "eq");	    break;
	    case BOOLEAN_OP_NEQ:    strcpy(buf, "ne");	    break;
	    case BOOLEAN_OP_LT:	    strcpy(buf, "lt");	    break;
	    case BOOLEAN_OP_GT:	    strcpy(buf, "gt");	    break;
	    case BOOLEAN_OP_LTE:    strcpy(buf, "lte");	    break;
	    case BOOLEAN_OP_GTE:    strcpy(buf, "gte");	    break;
	    
	}
	
	lntab(n->line, depth);
	printf("Boolean Operation: %s\n", buf);
	
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	    print_tree(tmp, depth + 1);
	
	return;
    }
    
    // Arithmatic Operation
    if ( flag_set(n, FLAG_ARITH_OP) )
    {
	switch (n->op)
	{
	    case ARITH_OP_ADD:    strcpy(buf, "addition");	    break;
	    case ARITH_OP_SUB:    strcpy(buf, "subtraction");	    break;
	    case ARITH_OP_MUL:    strcpy(buf, "multiplication");    break;
	    case ARITH_OP_DIV:    strcpy(buf, "division");	    break;
	    case ARITH_OP_MOD:    strcpy(buf, "modulous division"); break;
	}
	
	lntab(n->line, depth);
	printf("Arithmatic Operation: %s\n", buf);
	
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	    print_tree(tmp, depth + 1);
	
	return;
    }
    
    // List Operation
    if ( flag_set(n, FLAG_LIST_OP) )
    {
	switch (n->op)
	{
	    case LIST_OP_APPEND:	strcpy(buf, "append");		break;
	    case LIST_OP_CONS:		strcpy(buf, "cons");		break;
	    case LIST_OP_CAR:		strcpy(buf, "car");		break;
	    case LIST_OP_CDR:		strcpy(buf, "cdr");		break;
	    case LIST_OP_INTERSECTION:	strcpy(buf, "intersect");	break;
	    case LIST_OP_UNION:		strcpy(buf, "union");		break;
	    case LIST_OP_COMPLEMENT:	strcpy(buf, "complement");	break;
	    case LIST_OP_BICOMPLEMENT:	strcpy(buf, "bicomplement");	break;
	    case LIST_OP_MEMBER:	strcpy(buf, "is member");	break;
	    case LIST_OP_SUBSET:	strcpy(buf, "subset");		break;
	}
	
	lntab(n->line, depth);
	printf("List Operation: %s\n", buf);
	
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	    print_tree(tmp, depth + 1);
	
	return;
    }
    
    // Unknown type
    lntab(n->line, depth);
    printf("unknown (this is probably bad), flag value: %10d\n", n->flags );
}


// Semantic error function
void sem_err(char *msg, unsigned int line)
{
    fprintf(stderr, "Error near line %d:\n\t%s\n", line, msg);
    _exit(1);
}


// This does forward referencing for functions
void forward_functions(PARSE_NODE *n, ENVIRONMENT *env)
{
    PARSE_NODE *tmp = NULL;
    SCALAR *tmp_sc = NULL;
    char buf[512];
    int tmp_int = 0;
    CONTINUATION *tmp_c = NULL;
    
    /*
	First we need to load all of the functions
	into the environment. The parser has ensured
	that the root node will contain a list of 
	function definitions as its children.
    */
    for ( tmp = n->first_child; tmp; tmp = next_sibling(tmp) )
    {
	// Retrieve the key for the environment
	char *key = string_value(tmp->value);
	
	// Check for a function definition duplication
	if ( get_from_env(env, key) != NULL )
	{
	    sprintf(buf, "Duplicate Definition of function %s", key);
	    sem_err(buf, tmp->line);
	}
	
	// Construct the new continuation
	assert(tmp_c = (CONTINUATION *)GC_MALLOC( sizeof(CONTINUATION) ));
	tmp_c->env = env;
	tmp_c->function_tree = tmp;
	
	// Construct the new scalar
	tmp_sc = new_scalar();
	tmp_sc->value = (void *)tmp_c;
	tmp_sc->type = ST_CONTINUATION;
	
	// Add the scalar to the environment
	put_in_env(env, key, tmp_sc);
	
	// Save the last line position
	tmp_int = tmp->line;
    }
    
    // Make sure there is a main function
    if ( (tmp_sc = get_from_env(env, "main")) == NULL )
	sem_err("The 'main' function was not found", tmp_int);

    // Make sure the main function is formed properly
    // A corretly formed main function must have zero
    // parameters or one parameter.
    
    CONTINUATION *main_continuation = (CONTINUATION *)tmp_sc->value;
    PARSE_NODE *main_tree = main_continuation->function_tree;
       
    if ( arg_count(main_tree->args) > 1 )
    {
	sem_err(
	    "The 'main' function must have zero or one parameter", 
	    tmp->line
	    );    
    }
}

// After the functions have been stored we can actually
// interpret the meaning of the definitions and store them
// in the environment.
void forward_definitions(PARSE_NODE *n, ENVIRONMENT *env)
{
    PARSE_NODE *tmp = NULL;
    SCALAR *tmp_sc = NULL;
    char buf[512];
    int tmp_int = 0;
    CONTINUATION *tmp_c = NULL;
    
    /*
	Find the meaning of each of the definitions
	using the interpreter, and assign their values
	in the environment.
    */
    for ( tmp = n->first_child; tmp; tmp = next_sibling(tmp) )
    {
	// Retrieve the key for the environment
	char *key = string_value(tmp->value);
	
	// Check for duplicate definitions
	if ( get_from_env(env, key) != NULL )
	{
	    sprintf(buf, "Duplicate Definition of %s", key);
	    sem_err(buf, tmp->line);
	}
	
	// Find the meaning for the definition
	tmp_sc = interpret(tmp->first_child, env);
	
	// Make sure the meaning isn't null
	if (tmp_sc == NULL || tmp_sc->type == ST_NULL)
	{
	    sprintf(buf, "Definition of a null value for %s", key);
	    sem_err(buf, tmp->line);
	}
	
	// Add it into the environment
	put_in_env(env, key, tmp_sc);
    }
}


// Simply begins the interpretation on the main function
void interpret_main(ENVIRONMENT *env)
{
    SCALAR *main_sc = get_from_env(env, "main");
    CONTINUATION *main_continuation = (CONTINUATION *)main_sc->value;
    PARSE_NODE *tmp = main_continuation->function_tree->first_child;
    
    ENVIRONMENT *nenv = new_env();
    nenv->lower = main_continuation->env;
    
    while (tmp)
    {
	interpret(tmp, nenv);
	tmp = tmp->next_sibling;
    }
}


// Think about splitting this one apart, kinda big
// might cause some overhead... hrm...
SCALAR *interpret(PARSE_NODE *n, ENVIRONMENT *env)
{
    PARSE_NODE *tmp = NULL;
    PARSE_NODE *tmp2 = NULL;
    SCALAR *tmp_sc = NULL;
    SCALAR *tmp_sc2 = NULL;
    SCALAR *tmp_sc3 = NULL;
    char buf[256];
    char buf2[256];
    int tmp_int = 0;
    SCALAR_LIST *lst;
    SCALAR_LIST *list1 = NULL;
    SCALAR_LIST *list2 = NULL;
    SCALAR_LIST *list3 = NULL;
    
    // Encountering a null node is bad
    if (n == NULL)
    {
	fprintf(stderr, "FATAL ERROR: interpret(): NULL node encountered\n");
	_exit(1);
    }
    
    // block
    if ( flag_set(n, FLAG_BLOCK) )
    {
	// Simply go through each statement and return a scalar
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    if (tmp->next_sibling == NULL)
		return interpret(tmp, env);
	    else
		interpret(tmp, env); 
	}
    }
    
    // rec
    if ( flag_set(n, FLAG_REC) )
    {
	// Steps for making rec happen
	//
	//  1)	Check to make sure the rec is being
	//	called on the right number of arguments.
	//
	//  2)	Create the "dummy continuation"
	//
	//  3)	Create a new environment containing the
	//	rec's "dummy continuation"
	//
	//  4)	Interpret each of the rec body's statements
	//	on the new environment containing the 
	//	"dummy continuation".
	
	// Check for right number of arguments
	if (arg_count(n->args) != arg_count(n->app_expression))
	{
	    sprintf(buf, "Rec expects %d arguments, given %d on initial call",
		arg_count(n->args), arg_count(n->app_expression));
	    sem_err(buf, n->line);
	}
	
	// Create the "dummy continuation"
	PARSE_NODE *dummy_rec = new_parse_node();
	set_flag(dummy_rec, FLAG_REC2);
	dummy_rec->args = n->args;
	dummy_rec->first_child = n->first_child;
	dummy_rec->app_expression = n->app_expression;
	dummy_rec->line = n->line;
	
	ENVIRONMENT *nenv = new_env();
	nenv->lower = env;
	CONTINUATION *cnt = (CONTINUATION *)GC_MALLOC( sizeof(CONTINUATION) );
	assert(cnt);
	cnt->function_tree = dummy_rec;
	cnt->env = nenv;
	SCALAR *dsc = new_scalar();
	dsc->value = (void *)cnt;
	dsc->type = ST_CONTINUATION;
	
	// Add the continuation to the new environment
	put_in_env(nenv, "$rec-dummy", dsc);
	
	
	// Populate the environment with the initial call values
	tmp = n->args;
	tmp2 = n->app_expression; 
	
	while (tmp && tmp2)
	{
	    put_in_env(nenv, string_value(tmp->value), interpret(tmp2, env));
	    tmp = tmp->next_sibling;
	    tmp2 = tmp2->next_sibling;
	}
	
	// Interpret the rec's body
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	    tmp_sc = interpret(tmp, nenv);
	
	return tmp_sc;
    }
    
    // Special rec (created by the interpreter)
    if ( flag_set(n, FLAG_REC2) )
    {
	/*
	    This is much like a rec, but we don't actually
	    have to make the dummy continuation or check
	    for arguments. Simply run the commands.
	    
	    Since this can only be reached via a recur we
	    are safe in assuming that the environment was
	    populated with the correct values already.
	*/
	
	// Interpret the rec's body
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	    tmp_sc = interpret(tmp, env);
	
	return tmp_sc;
    }
    
    // recur
    if ( flag_set(n, FLAG_RECUR) )
    {
	// Make sure this is in the body of a rec
	tmp_sc = get_highest(env, "$rec-dummy");
	
	// Check to see if we are ina rec
	if (tmp_sc == NULL)
	    sem_err("recur used outside of a rec", n->line);
	
	CONTINUATION *cnt = (CONTINUATION *)tmp_sc->value;
	PARSE_NODE *rn = cnt->function_tree;
		
	// Make sure we are recurring on the correct
	// number of params
	if (arg_count(n->first_child) != arg_count(rn->args))
	{
	    sprintf(buf, "Rec expects %d arguments, given %d on initial call",
		arg_count(rn->args), arg_count(n->first_child));
	    sem_err(buf, n->line);
	}
	
	// Create / Populate the new environment
	ENVIRONMENT *nenv = new_env();
	nenv->lower = env;
	tmp = rn->args;
	tmp2 = n->first_child;
	
	while (tmp && tmp2)
	{
	    put_in_env(nenv, string_value(tmp->value), interpret(tmp2, env));
	    tmp = tmp->next_sibling;
	    tmp2 = tmp2->next_sibling;
	}
	
	// Run the rec
	return interpret(rn, nenv);
    }
    
    
    // let
    if ( flag_set(n, FLAG_LET) )
    {
	// Extend the environment
	ENVIRONMENT *nenv = new_env();
	nenv->lower = env;
	
	// Populate the environment
	for (tmp = n->args; tmp; tmp = tmp->next_sibling)
	{
	    put_in_env(
		nenv, 
		string_value(tmp->args->value), 
		interpret(tmp->first_child, env)
		);
	}
	
	// Interpret the body using the newly extended environment
	return interpret(n->first_child, nenv);
    }
    
    // Assignment
    if ( flag_set(n, FLAG_ASSIGNMENT) )
    {
	// Assignment outside of a let, error!
	sem_err("Assignment outside of let", n->line);
    }
    
    // if
    if ( flag_set(n, FLAG_IF) )
    {
	// Determine if the conditional equates
	// to a boolean value;
	tmp_sc = interpret(n->args, env);
	
	// If it is not a boolean value, error!!!
	if (tmp_sc == NULL || tmp_sc->type != ST_BOOLEAN)
	{
	    sem_err(
		"Conditional in if does not equate to a boolean value",
		n->line
		);
	}
    
	// Determine the path to take
	if (long_value(tmp_sc))
	    return interpret(n->cond_true, env);
	else
	    return interpret(n->cond_false, env);
    }
    
    // cond
    if ( flag_set(n, FLAG_COND) )
    {
	// Go through the list of conditions until one hits
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    if (tmp->args == NULL)
	    {
		// Default condition
		// If there is another condition
		// afterwards, then error, since
		// the default condition must be
		// last..
		if (tmp->next_sibling)
		    sem_err("There can be only one default case, and it must be last", tmp->line);
		
		return interpret(tmp->first_child, env);
	    }
	    else
	    {
		tmp_sc = interpret(tmp->args, env);
		
		if (tmp_sc == NULL || tmp_sc->type != ST_BOOLEAN)
		{
		    sem_err(
		    "Conditional in case does not equate to a boolean value",
		    n->line
		    );
		}
		
		if (long_value(tmp_sc))
		    return interpret(tmp->first_child, env);;
	    }
	}
	
	return NULL;
    }

    // A condition
    if ( flag_set(n, FLAG_CONDITION) )
    {
	// Condition outside of a cond, ERROR!
	sem_err("case outside of a cond", n->line);
    }
    
    // Application
    if ( flag_set(n, FLAG_APPLICATION) )
    {
	// Get the function
	tmp_sc = get_highest(env, string_value(n->app_expression->value));
	
	if (tmp_sc == NULL)
	{  
	    sprintf(buf, "Function '%s' not found", string_value(n->app_expression->value));
	    sem_err(buf, n->line);
	}
	
	if (tmp_sc->type == ST_BUILT_IN)
	{
	    // Grab the function pointer
	    // apply it to the arguments
	    SCALAR *(*bif)(PARSE_NODE *, ENVIRONMENT *) = 
		(SCALAR *(*)(PARSE_NODE *, ENVIRONMENT *))tmp_sc->value;
	    
	    tmp_sc2 = bif(n->first_child, env);
	}
	else if (tmp_sc->type == ST_CONTINUATION)
	{
	    // Applying a user defined function is a litte bit more
	    // difficult. First we need to get ahold of the continuation...
	    CONTINUATION *cont = (CONTINUATION *)tmp_sc->value;
	    
	    // Check the number of arguments
	    if (arg_count(cont->function_tree->args) != arg_count(n->first_child))
	    {
		sprintf(buf, "Function '%s' requires %d arguments, given %d",
		    string_value(n->app_expression->value),
		    arg_count(cont->function_tree->args),
		    arg_count(n->first_child));
		sem_err(buf, n->line);
	    }
	    
	    // Populate the new environment
	    ENVIRONMENT *nenv = new_env();
	    nenv->lower = cont->env;
	    tmp = n->first_child;
	    tmp2 = cont->function_tree->args;
	    
	    while (tmp && tmp2)
	    {
		//printf("Entering value for key %s\n", string_value(tmp2->value));
		
		put_in_env(nenv, string_value(tmp2->value), interpret(tmp, env));
		tmp = tmp->next_sibling;
		tmp2 = tmp2->next_sibling;
	    }
	    
	    // Execute the function
	    for (tmp = cont->function_tree->first_child; tmp; tmp = tmp->next_sibling)
		tmp_sc2 = interpret(tmp, nenv);
	    
	    return tmp_sc2;
	}
	else
	{
	    // If it is not a continuation or a built-in
	    // then it cannot be applied!
	    sprintf(buf, "Attempted application of non function '%s'",
		string_value(n->app_expression->value));
	    sem_err(buf, n->line);
	}
	
	return tmp_sc2;
    }
    
    // List
    if ( flag_set(n, FLAG_LIST) )
    {
	// Need to create the list
	SCALAR_LIST *scl = new_scalar_list();
	
	// Populate the list
	for (tmp = n->first_child; tmp; tmp = tmp->next_sibling)
	{
	    append(scl, interpret(tmp, env));
	}
	
	// Create the new scalar containing the list
	// and return it
	tmp_sc = new_scalar();
	set_list_value(tmp_sc, scl);
	
	return tmp_sc;
    }
    
    // Primitive Constants
    if ( flag_set(n, FLAG_INTEGER) || flag_set(n, FLAG_DOUBLE) ||
	 flag_set(n, FLAG_BOOLEAN) || flag_set(n, FLAG_STRING) ||
	 flag_set(n, FLAG_SYMBOL)  || flag_set(n, FLAG_CHARACTER) )
    {
	// Simply copy the constant scalar and pass it on up
	tmp_sc = copy_scalar(n->value);
	return tmp_sc;
    }
    
    // Identifier
    if ( flag_set(n, FLAG_ID) )
    {
	tmp_sc = get_highest(env, string_value(n->value));
	
	if (tmp_sc == NULL)
	{
	    sprintf(buf, "Identifier '%s' not found", string_value(n->value));
	    sem_err(buf, n->line);
	}
	
	return copy_scalar(tmp_sc);
    }
    
    // Error
    if ( flag_set(n, FLAG_ERROR) )
    {
	// Interpret the condition
	tmp_sc = interpret(n->args, env);
	
	// Make sure the scalar is not null
	if (tmp_sc == NULL)
	{
	    sem_err("Condition for error does not evaluate to a boolean value", n->line);
	}
	
	// Make sure it is of type boolean
	if (tmp_sc->type != ST_BOOLEAN)
	{
	    sem_err("Condition for error does not evaluate to a boolean value", n->line);
	}
	
	// Now we need to determine whether or not
	// we should error out.
	if (long_value(tmp_sc))
	{
	    interpret(n->first_child, env);
	    
	    // Exit like a good boy
	    _exit(0);
	}
				
	// Otherwise simply return a null value
	return NULL;
    }
    
    // Boolean Operation
    if ( flag_set(n, FLAG_BOOLEAN_OP) )
    {
	//printf("BOOLEAN OP: %d\n", n->op);
	
	// Okay first we need to evaluate the
	// children of the statement.
	tmp_sc = interpret(n->first_child, env);
	
	// Now for the special unary not operator
	if (n->op == BOOLEAN_OP_NOT)
	{
	    if (tmp_sc == NULL)
		sem_err("Attempted boolean operation of non boolean values", n->line);
	    
	    if (tmp_sc->type != ST_BOOLEAN)
		sem_err("Attempted boolean operation of non boolean values", n->line);
	    
	    tmp_sc2 = new_scalar();
	    set_bool_value(tmp_sc2, (long_value(tmp_sc)) ? 0 : 1);
	    return tmp_sc2;
	}	
	
	// Continuing on, grab the second child...
	tmp_sc2 = interpret(n->first_child->next_sibling, env);	
    
	// Allocate room for the new return value
	tmp_sc3 = new_scalar();
	
	// Determine if the operator is that of propositional
	// calculus or a comparison and carry out the appropriate
	// course of action.
	if ( n->op == BOOLEAN_OP_AND  ||
	     n->op == BOOLEAN_OP_OR   ||
	     n->op == BOOLEAN_OP_XOR  ||
	     n->op == BOOLEAN_OP_IMP  ||
	     n->op == BOOLEAN_OP_DIMP)
	{
	    // Make sure that we have boolean values to compare.
	    if (tmp_sc == NULL || tmp_sc2 == NULL)
		sem_err("Attempted boolean operation of non boolean values", n->line);
	    
	    if (tmp_sc->type != ST_BOOLEAN || tmp_sc2->type != ST_BOOLEAN)
		sem_err("Attempted boolean operation of non boolean values", n->line);
	    
	    
	    switch (n->op)
	    {
		case BOOLEAN_OP_AND:
		    set_bool_value(tmp_sc3, 
			long_value(tmp_sc) && long_value(tmp_sc2) ? 1 : 0);
		    break;
		    
		case BOOLEAN_OP_OR:	    
		    set_bool_value(tmp_sc3, 
			long_value(tmp_sc) || long_value(tmp_sc2) ? 1 : 0);
		    break;
		    
		case BOOLEAN_OP_XOR:	    
		    set_bool_value(tmp_sc3, 
			(long_value(tmp_sc) || long_value(tmp_sc2)) &&
			!(long_value(tmp_sc) && long_value(tmp_sc2)) ? 1 : 0);
		    break;
		    
		case BOOLEAN_OP_IMP:	    
		    set_bool_value(tmp_sc3, 
			(!long_value(tmp_sc)) ? 1 : (long_value(tmp_sc2)) ? 1 : 0);
		    break;
		    
		case BOOLEAN_OP_DIMP:   
		    set_bool_value(tmp_sc3,
			(long_value(tmp_sc) && long_value(tmp_sc2)) ||
			(!long_value(tmp_sc) && !long_value(tmp_sc2)) ? 1 : 0);
		    break;
		
		default:
		    // THis should never be able to happen...
		    // But in the case of a coding error, I have
		    // given the interpreter an out.
		    //
		    // Never say never
		    // And always leave yourself an out
		    //
		    sem_err("FATAL: Unknown boolean operation!", n->line);
		    break;
	    }
	}
	else
	{
	    // And the comparison operators
	    switch (n->op)
	    {
		case BOOLEAN_OP_EQ:
		    set_bool_value(tmp_sc3, eq_scalar(tmp_sc, tmp_sc2) ? 1 : 0);
		    break;
		case BOOLEAN_OP_NEQ:
		    set_bool_value(tmp_sc3, !eq_scalar(tmp_sc, tmp_sc2) ? 1 : 0);
		    break;
		case BOOLEAN_OP_EQV:
		    set_bool_value(tmp_sc3, eqv_scalar(tmp_sc, tmp_sc2) ? 1 : 0);
		    break;
		case BOOLEAN_OP_LT:
		    set_bool_value(tmp_sc3, lt_scalar(tmp_sc, tmp_sc2) ? 1 : 0 );
		    break;
		case BOOLEAN_OP_GT:
		    set_bool_value(tmp_sc3, gt_scalar(tmp_sc, tmp_sc2) ? 1 : 0 );
		    break;
		case BOOLEAN_OP_LTE:
		    set_bool_value(tmp_sc3, 
			eq_scalar(tmp_sc, tmp_sc2) || lt_scalar(tmp_sc, tmp_sc2) ? 1 : 0);
		    break;
		case BOOLEAN_OP_GTE:
		    set_bool_value(tmp_sc3, 
			eq_scalar(tmp_sc, tmp_sc2) || gt_scalar(tmp_sc, tmp_sc2) ? 1 : 0);
		    break;
	    }
	}    
	
	return tmp_sc3;
    }
    
    // Arithmatic Operation
    if ( flag_set(n, FLAG_ARITH_OP) )
    {
	// Since these are all binary this
	// is actually much more straight
	// forward than the boolean operators
	
	// First grab the evaluations of the right and left side
	tmp_sc = interpret(n->first_child, env);
	tmp_sc2 = interpret(n->first_child->next_sibling, env);
	
	// Figure out what we are doing (need this for error checking)
	switch (n->op)
	{
	    case ARITH_OP_ADD:    strcpy(buf, "addition");	    break;
	    case ARITH_OP_SUB:    strcpy(buf, "subtraction");	    break;
	    case ARITH_OP_MUL:    strcpy(buf, "multiplication");    break;
	    case ARITH_OP_DIV:    strcpy(buf, "division");	    break;
	    case ARITH_OP_MOD:    strcpy(buf, "modulus division");  break;
	}
	
	if (tmp_sc == NULL || tmp_sc2 == NULL)
	{
	    sprintf(buf2, "Attempted use of a null value for %s", buf);
	    sem_err(buf2, n->line);
	}
	
	// We cannot commit arithmetic operations on the
	// following scalar types:
	//
	//  ST_UNDEFINED
	//  ST_BOOLEAN
	//  ST_CONTINUATION
	//  ST_BUILT_IN
	//  ST_NULL
	//  ST_LIST
	//  ST_SYMBOL
	//
	// So error out if either of the sides evaluates
	// to a scalar of any of these types
	
	if (tmp_sc->type == ST_UNDEFINED    || tmp_sc2->type == ST_UNDEFINED	||
	    tmp_sc->type == ST_BOOLEAN	    || tmp_sc2->type == ST_BOOLEAN	||
	    tmp_sc->type == ST_CONTINUATION || tmp_sc2->type == ST_CONTINUATION ||
	    tmp_sc->type == ST_BUILT_IN	    || tmp_sc2->type == ST_BUILT_IN	||
	    tmp_sc->type == ST_NULL	    || tmp_sc2->type == ST_NULL	)
	{
	    sprintf(buf2, "Attempted %s of an invalid type", buf);
	    sem_err(buf2, n->line);
	}
	
	tmp_sc3 = new_scalar();
	
	switch (n->op)
	{
	    case ARITH_OP_ADD:    
		/*
		    Addition is the most versitile, here is how
		    it operates:
		    
		    Lists
			If both operands are lists, it returns a
			list containing all the elements of each
			list.
		    
		    Strings
			If a string is added to anything besides
			a list then the value is converted to a
			string and concatenated on the appropriate
			side of the string.
		    
		    Double
			Any Double/Long mixed arithmetic expressions
			always yeild a double value.
		*/
		
		if (tmp_sc->type == ST_LIST)
		{
		    // Make sure both operands are lists
		    if (tmp_sc2->type != ST_LIST)
		    {
			sem_err("Second Operand must also be a list for list addition", n->line);
		    }
		    
		    // Add the lists to one big list!
		    SCALAR_LIST *list1 = list_value(tmp_sc);
		    SCALAR_LIST *list2 = list_value(tmp_sc2);
		    
		    while (list1->type != SLT_FOOTER)
			list1 = list1->prev;
		    
		    while (list2->type != SLT_HEADER)
			list2 = list2->prev;
			
		    list1->prev->next = list2->next;
		    list2->next->prev = list1->prev;
		    
		    SCALAR_LIST *list3 = list1->prev;
		    while (list3->type != SLT_HEADER)
			list3 = list3->prev;
		    
		    tmp_sc3 = new_scalar();
		    set_list_value(tmp_sc3, list3);
		    
		    list1->next = NULL;
		    list2->next = NULL;
		    list1->prev = NULL;
		    list2->prev = NULL;
		}
		else if (tmp_sc2->type == ST_LIST)
		{
		    sem_err("First operand must also be a list for list addition", n->line);
		}
		else if (tmp_sc->type == ST_STRING)
		{
		    strcpy(buf, "");
		    strcat(buf, string_value(tmp_sc));
		    strcat(buf, string_value(tmp_sc2));
		    set_string_value(tmp_sc3, buf, strlen(buf));
		}
		else if (tmp_sc2->type == ST_STRING)
		{
		    strcpy(buf, "");
		    strcat(buf, string_value(tmp_sc));
		    strcat(buf, string_value(tmp_sc2));
		    set_string_value(tmp_sc3, buf, strlen(buf));
		}
		else
		{
		    // Weed out those pesky symbols and lists
		    if (tmp_sc->type == ST_SYMBOL || tmp_sc2->type == ST_SYMBOL)
		    {
			sprintf(buf2, "Attempted %s of an invalid type", buf);
			sem_err(buf2, n->line);
		    }
		    else if (tmp_sc->type == ST_DOUBLE || tmp_sc2->type == ST_DOUBLE)
		    {
			double d = double_value(tmp_sc) + double_value(tmp_sc2);
			set_double_value(tmp_sc3, d);
		    }
		    else
		    {
			long l = long_value(tmp_sc) + long_value(tmp_sc2);
			set_long_value(tmp_sc3, l);
		    }
		}
		break;
		
		
	    case ARITH_OP_SUB:    
		if ((tmp_sc->type != ST_LONG && tmp_sc->type != ST_DOUBLE) ||
		    (tmp_sc2->type != ST_LONG && tmp_sc2->type != ST_DOUBLE))
		{
		    sem_err("Subtraction requires that both operands are integer or double values", n->line);
		}
		
		if ( tmp_sc->type == ST_DOUBLE || tmp_sc2->type == ST_DOUBLE )
		{
		    double d = double_value(tmp_sc) - double_value(tmp_sc2);
		    set_double_value(tmp_sc3, d);
		}
		else
		{
		    long l = long_value(tmp_sc) - long_value(tmp_sc2);
		    set_long_value(tmp_sc3, l);
		}
		
		break;
		
	    case ARITH_OP_MUL:    
		if ((tmp_sc->type != ST_LONG && tmp_sc->type != ST_DOUBLE) ||
		    (tmp_sc2->type != ST_LONG && tmp_sc2->type != ST_DOUBLE))
		{
		    sem_err("Multiplication requires that both operands are integer or double values", n->line);
		}
		
		if ( tmp_sc->type == ST_DOUBLE || tmp_sc2->type == ST_DOUBLE )
		{
		    double d = double_value(tmp_sc) * double_value(tmp_sc2);
		    set_double_value(tmp_sc3, d);
		}
		else
		{
		    long l = long_value(tmp_sc) * long_value(tmp_sc2);
		    set_long_value(tmp_sc3, l);
		}
		
		break;
		
	    case ARITH_OP_DIV:
		if ((tmp_sc->type != ST_LONG && tmp_sc->type != ST_DOUBLE) ||
		    (tmp_sc2->type != ST_LONG && tmp_sc2->type != ST_DOUBLE))
		{
		    sem_err("Division requires that both operands are integer or double values", n->line);
		}
		
		if ( tmp_sc->type == ST_DOUBLE || tmp_sc2->type == ST_DOUBLE )
		{
		    if (double_value(tmp_sc2) == 0.0)
			sem_err("Attempted division by zero", n->line);
			
		    double d = double_value(tmp_sc) / double_value(tmp_sc2);
		    set_double_value(tmp_sc3, d);
		}
		else
		{
		    if (long_value(tmp_sc2) == 0)
			sem_err("Attempted division by zero", n->line);
			
		    long l = long_value(tmp_sc) / long_value(tmp_sc2);
		    set_long_value(tmp_sc3, l);
		}
		
		break;
		
	    case ARITH_OP_MOD:
		// This can be only between two integers
		// so error out if it is not
		if (tmp_sc->type != ST_LONG || tmp_sc2->type != ST_LONG)
		{
		    sem_err("Modulus division requires that both operands are integer values", n->line);
		}
		
		long l = long_value(tmp_sc) % long_value(tmp_sc2);
		set_long_value(tmp_sc3, l);
		break;
	}
	
	// Finally return the value
	return tmp_sc3;
    }
    
    
    // List operations
    if ( flag_set(n, FLAG_LIST_OP) )
    {
	switch (n->op)
	{
	    case LIST_OP_CONS:
		
		// Grab the item and the list
		tmp_sc = interpret(n->first_child, env);
		tmp_sc2 = interpret(n->first_child->next_sibling, env);
		
		// If the second evaluation isn't a list error out.
		if (tmp_sc2->type != ST_LIST)
		    sem_err("The 'cons' operator expects the right operand to be a list", n->line);
		
		// Okiedokie, do the cons
		lst = list_value(tmp_sc2);
		insert_front(lst, tmp_sc);
		tmp_sc3 = new_scalar();
		set_list_value(tmp_sc3, lst);
		
		break;
		
	    case LIST_OP_APPEND:
		// Grab the item and the list
		tmp_sc = interpret(n->first_child, env);
		tmp_sc2 = interpret(n->first_child->next_sibling, env);
		
		// If the first evaluation isn't a list error out.
		if (tmp_sc->type != ST_LIST)
		    sem_err("The 'append' operator expects the left operand to be a list", n->line);
		
		lst = list_value(tmp_sc);
		append(lst, tmp_sc2);
		tmp_sc3 = new_scalar();
		set_list_value(tmp_sc3, lst);
		
		break;
	    
	    case LIST_OP_CAR:
		tmp_sc = interpret(n->first_child, env);
		
		if (tmp_sc->type != ST_LIST)
		    sem_err("The 'car' operator expects the right operand to be a list", n->line);
		
		lst = list_value(tmp_sc);
		
		while(lst->type != SLT_HEADER)
		    lst = lst->prev;
	    
		lst = lst->next;
		
		tmp_sc3 = copy_scalar(lst->value);
	    
		break;
	    
	    case LIST_OP_CDR:
		tmp_sc = interpret(n->first_child, env);
		
		if (tmp_sc->type != ST_LIST)
		    sem_err("The 'cdr' operator expects the right operand to be a list", n->line);
		    
		lst = list_value(tmp_sc);
		
		if (empty_list(lst))
		    sem_err("The 'cdr' operator can only be applied to non empty lists", n->line);
		
		while(lst->type != SLT_HEADER)
		    lst = lst->prev;
		
		lst->next->prev = lst->prev;
		lst->prev->next = lst->next;
		lst->next->type = SLT_HEADER;
		lst->next->value = NULL;
		
		tmp_sc3 = new_scalar();
		set_list_value(tmp_sc3, lst->next);
		
		lst->next = NULL;
		lst->prev = NULL;
		break;
		
	    case LIST_OP_INTERSECTION:
		// A & B = {x | x ? A and x ? B}
	    
		// Evaluate the operands
		tmp_sc = interpret(n->first_child, env);
		tmp_sc2 = interpret(n->first_child->next_sibling, env);
		
		// Make sure they are both lists
		if (tmp_sc->type != ST_LIST || tmp_sc2->type != ST_LIST)
		    sem_err("The 'intersection' operator requires both operands be lists", n->line);
		
		// Commit the intersection
		list1 = list_value(tmp_sc);
		list2 = list_value(tmp_sc2);
		
		while (list1->prev->type != SLT_HEADER)
		    list1 = list1->next;
		
		while (list2->prev->type != SLT_HEADER)
		    list2 = list2->next;
		
		tmp_sc3 = new_scalar();
		
		if (list1->type == SLT_FOOTER)
		    set_list_value(tmp_sc3, list1);
		else if (list2->type == SLT_FOOTER)
		    set_list_value(tmp_sc3, list2);
		else
		{
		    // Go through and match everything
		    // (this takes a long time)
		    while(list1->type != SLT_FOOTER)
		    {
			int found = 0;
			
			while (list2->prev->type != SLT_HEADER)
			    list2 = list2->next;
			
			while (list2->type != SLT_FOOTER)
			{
			    /*printf("ONE: %s\n", string_value(list1->value));
			    printf("TWO: %s\n", string_value(list2->value));
			    printf("EQ?: %d\n", eq_scalar(list1->value, list2->value));*/
			
			    if (eq_scalar(list2->value, list1->value))
			    {
				//printf("F\n");
				found = 1;
				list3 = list2->next;
				list2->prev->next = list2->next;
				list2->next->prev = list2->prev;
				list2->next = NULL;
				list2->prev = NULL;
				list2 = list3;
				break;
			    }
			    else
			        list2 = list2->next;
			}
			
			    
			if (found)
			    list1 = list1->next;
			else
			{
			    list3 = list1->next;
			    list1->prev->next = list1->next;
			    list1->next->prev = list1->prev;
			    list1->next = NULL;
			    list1->prev = NULL;
			    list1 = list3;
			    
			    set_list_value(tmp_sc3, list1);
			    //printf("%s\n", string_value(tmp_sc3));
			}
		    }
		}
		
		set_list_value(tmp_sc3, list1);
		
		
		
		break;
	    
	    case LIST_OP_UNION:
		// A | B = {x | x ? A or x ? B}
		
		// Evaluate the operands
		tmp_sc = interpret(n->first_child, env);
		tmp_sc2 = interpret(n->first_child->next_sibling, env);
		
		// Make sure they are both lists
		if (tmp_sc->type != ST_LIST || tmp_sc2->type != ST_LIST)
		    sem_err("The 'union' operator requires both operands be lists", n->line);
		
		// Commit the union
		list1 = list_value(tmp_sc);
		list2 = list_value(tmp_sc2);
		
		while (list1->prev->type != SLT_HEADER)
		    list1 = list1->next;
		
		while (list2->prev->type != SLT_HEADER)
		    list2 = list2->next;
		
		tmp_sc3 = new_scalar();
		
		if (list1->type == SLT_FOOTER)
		    set_list_value(tmp_sc3, list2);
		else if (list2->type == SLT_FOOTER)
		    set_list_value(tmp_sc3, list1);
		else
		{
		    // Go through and match everything
		    // (this takes a long time)
		    while(list1->type != SLT_FOOTER)
		    {
			int found = 0;
			
			while (list2->prev->type != SLT_HEADER)
			    list2 = list2->next;
			
			while (list2->type != SLT_FOOTER)
			{
			    /*printf("ONE: %s\n", string_value(list1->value));
			    printf("TWO: %s\n", string_value(list2->value));
			    printf("EQ?: %d\n", eq_scalar(list1->value, list2->value));*/
			
			    if (eq_scalar(list2->value, list1->value))
			    {
				//printf("F\n");
				found = 1;
				list3 = list2->next;
				list2->prev->next = list2->next;
				list2->next->prev = list2->prev;
				list2->next = NULL;
				list2->prev = NULL;
				list2 = list3;
				break;
			    }
			    else
			        list2 = list2->next;
			}
			
			list1 = list1->next;
		    }
		    
		    while (list2->prev->type != SLT_HEADER)
			list2 = list2->next;
		
		    while (list1->prev->type != SLT_HEADER)
			list1 = list1->next;
		    
		    while (list2->type != SLT_FOOTER)
		    {
			append(list1, list2->value);
			list2 = list2->next;
		    }
		}
		
		set_list_value(tmp_sc3, list1);
		
		break;
	    
	    case LIST_OP_COMPLEMENT:
		// A \ B = {x | x ? A and !(x ? B)}
		
		// Evaluate the operands
		tmp_sc = interpret(n->first_child, env);
		tmp_sc2 = interpret(n->first_child->next_sibling, env);
		
		// Make sure they are both lists
		if (tmp_sc->type != ST_LIST || tmp_sc2->type != ST_LIST)
		    sem_err("The 'complement' operator requires both operands be lists", n->line);
		
		// Commit the complement
		list1 = list_value(tmp_sc);
		list2 = list_value(tmp_sc2);
		
		while (list1->prev->type != SLT_HEADER)
		    list1 = list1->next;
		
		while (list2->prev->type != SLT_HEADER)
		    list2 = list2->next;
		
		tmp_sc3 = new_scalar();
		
		if (list1->type == SLT_FOOTER)
		    set_list_value(tmp_sc3, list1);
		else if (list2->type == SLT_FOOTER)
		    set_list_value(tmp_sc3, list2);
		else
		{
		    // Go through and match everything
		    // (this takes a long time)
		    while(list1->type != SLT_FOOTER)
		    {
			int found = 0;
			
			while (list2->prev->type != SLT_HEADER)
			    list2 = list2->next;
			
			while (list2->type != SLT_FOOTER)
			{
			    /*printf("ONE: %s\n", string_value(list1->value));
			    printf("TWO: %s\n", string_value(list2->value));
			    printf("EQ?: %d\n", eq_scalar(list1->value, list2->value));*/
			
			    if (eq_scalar(list2->value, list1->value))
			    {
				//printf("F\n");
				found = 1;
				list3 = list2->next;
				list2->prev->next = list2->next;
				list2->next->prev = list2->prev;
				list2->next = NULL;
				list2->prev = NULL;
				list2 = list3;
				break;
			    }
			    else
			        list2 = list2->next;
			}
			
			    
			if (!found)
			    list1 = list1->next;
			else
			{
			    list3 = list1->next;
			    list1->prev->next = list1->next;
			    list1->next->prev = list1->prev;
			    list1->next = NULL;
			    list1->prev = NULL;
			    list1 = list3;
			    
			    set_list_value(tmp_sc3, list1);
			    //printf("%s\n", string_value(tmp_sc3));
			}
		    }
		}
		
		set_list_value(tmp_sc3, list1);
		break;
	    
	    case LIST_OP_BICOMPLEMENT:
		// Evaluate the operands
		tmp_sc = interpret(n->first_child, env);
		tmp_sc2 = interpret(n->first_child->next_sibling, env);
		
		// Make sure they are both lists
		if (tmp_sc->type != ST_LIST || tmp_sc2->type != ST_LIST)
		    sem_err("The 'bi-directional complement' operator requires both operands be lists", n->line);
		
		// Commit the complement
		list1 = list_value(tmp_sc);
		list2 = list_value(tmp_sc2);
		
		while (list1->prev->type != SLT_HEADER)
		    list1 = list1->next;
		
		while (list2->prev->type != SLT_HEADER)
		    list2 = list2->next;
		
		tmp_sc3 = new_scalar();
		
		if (list1->type == SLT_FOOTER)
		    set_list_value(tmp_sc3, list1);
		else if (list2->type == SLT_FOOTER)
		    set_list_value(tmp_sc3, list2);
		else
		{
		    // Go through and match everything
		    // (this takes a long time)
		    while(list1->type != SLT_FOOTER)
		    {
			int found = 0;
			
			while (list2->prev->type != SLT_HEADER)
			    list2 = list2->next;
			
			while (list2->type != SLT_FOOTER)
			{
			    /*printf("ONE: %s\n", string_value(list1->value));
			    printf("TWO: %s\n", string_value(list2->value));
			    printf("EQ?: %d\n", eq_scalar(list1->value, list2->value));*/
			
			    if (eq_scalar(list2->value, list1->value))
			    {
				//printf("F\n");
				found = 1;
				list3 = list2->next;
				list2->prev->next = list2->next;
				list2->next->prev = list2->prev;
				list2->next = NULL;
				list2->prev = NULL;
				list2 = list3;
				break;
			    }
			    else
			        list2 = list2->next;
			}
			
			    
			if (!found)
			    list1 = list1->next;
			else
			{
			    list3 = list1->next;
			    list1->prev->next = list1->next;
			    list1->next->prev = list1->prev;
			    list1->next = NULL;
			    list1->prev = NULL;
			    list1 = list3;
			    
			    set_list_value(tmp_sc3, list1);
			    //printf("%s\n", string_value(tmp_sc3));
			}
		    }
		    
		    while (list2->prev->type != SLT_HEADER)
			list2 = list2->next;
		
		    while (list1->prev->type != SLT_HEADER)
			list1 = list1->next;
		    
		    while (list2->type != SLT_FOOTER)
		    {
			append(list1, list2->value);
			list2 = list2->next;
		    }
		}
		
		set_list_value(tmp_sc3, list1);
		break;
		
	    case LIST_OP_MEMBER:
		// Evaluate the operands
		tmp_sc = interpret(n->first_child, env);
		tmp_sc2 = interpret(n->first_child->next_sibling, env);
		
		// Make sure the right operand is a list
		if (tmp_sc2->type != ST_LIST)
		    sem_err("The 'element of' operator requires the right operands be a list", n->line);
		
		list1 = list_value(tmp_sc2);
		
		tmp_int = 0;
		
		while (list1->prev->type != SLT_HEADER)
		    list1 = list1->prev;
		
		while (list1->type != SLT_FOOTER)
		{
		    if (eq_scalar(list1->value, tmp_sc))
		    {
			tmp_int = 1;
			break;
		    }
		    
		    list1 = list1->next;
		}
		
		tmp_sc3 = new_scalar();
		set_bool_value(tmp_sc3, tmp_int);
		break;
	    
	    case LIST_OP_SUBSET:
		// Evaluate the operands
		tmp_sc = interpret(n->first_child, env);
		tmp_sc2 = interpret(n->first_child->next_sibling, env);
		
		// Make sure they are both lists
		if (tmp_sc->type != ST_LIST || tmp_sc2->type != ST_LIST)
		    sem_err("The 'subset' operator requires both operands be lists", n->line);
		
		// Commit the subset
		list1 = list_value(tmp_sc);
		list2 = list_value(tmp_sc2);
		
		while (list1->prev->type != SLT_HEADER)
		    list1 = list1->next;
	    
		while (list2->prev->type != SLT_HEADER)
		    list2 = list2->next;
		
		if (empty_list(list2) && !empty_list(list1))
		    tmp_int = 0;
		else if (empty_list(list1))
		    tmp_int = 1;
		else
		{
		    tmp_int = 0;
		    
		    while(list2->type != SLT_FOOTER)
		    {
			int found = 0;
									
			while (list1->prev->type != SLT_HEADER)
			    list1 = list1->next;
			
			while (list1->type != SLT_FOOTER)
			{
			    if (eq_scalar(list2->value, list1->value))
			    {
				found = 1;
				list3 = list1->next;
				list1->prev->next = list1->next;
				list1->next->prev = list1->prev;
				list1->next = NULL;
				list1->prev = NULL;
				list1 = list3;
				break;
			    }
			    else
			        list1 = list1->next;
			}
			
			if (empty_list(list1))
			{
			    tmp_int = 1;
			    break;
			}
			
			if (!found)
			    list2 = list2->next;
			else
			{
			    list3 = list2->next;
			    list2->prev->next = list2->next;
			    list2->next->prev = list2->prev;
			    list2->next = NULL;
			    list2->prev = NULL;
			    list2 = list3;
			}
		    }
		}
		
		tmp_sc3 = new_scalar();
		set_bool_value(tmp_sc3, tmp_int);
		
		break;
		
	    case LIST_OP_ELEMENT_AT:
		
		tmp_sc = interpret(n->first_child, env);
		tmp_sc2 = interpret(n->first_child->next_sibling, env);
		
		if ( tmp_sc == NULL || tmp_sc->type != ST_LIST )
		    sem_err("Element referencing requires that the identifier evaluates to a list", n->line);
		
		if ( tmp_sc2 == NULL || tmp_sc2->type != ST_LONG )
		    sem_err("Index for element reference must evaluate to an integer", n->line);
		
		if (long_value(tmp_sc2) < 0 || long_value(tmp_sc2) > sl_length(list_value(tmp_sc)))
		{
		    sprintf(buf, "Index of '%d' for element reference is out of bounds", long_value);
		    sem_err(buf, n->line);
		}
		
		tmp_sc3 = copy_scalar(get_from_list(list_value(tmp_sc), long_value(tmp_sc2)));
		
		break;
	}
	
	return tmp_sc3;
    }
    
    // Unknown type
    sprintf(buf, "FATAL: Unknown node flag encountered: %10d", n->flags );
    sem_err(buf, 0);
    
    return NULL;
}
