/*
    FILE:	builtin.c
    PROJECT:	objection stage 
    AUTHOR:	Ryan Richards
    
    This is the implementation of the built-in function
    library.
*/

#include "builtin.h"

// Used by the random number generator
int seeded;

// Constructs the base environment containing
// all of the built in functions.
ENVIRONMENT *base_environment()
{
    ENVIRONMENT *env = new_env();
    int i;
    
    // Initialize for rand
    seeded = 0;
    
    // Holds the keys
    char built_in_keys[MAX_BUILT_INS][25];
    
    // Holds the functions
    SCALAR *(*built_in_functions[MAX_BUILT_INS])(PARSE_NODE *, ENVIRONMENT *);
    
    // Initialize the arrays
    init_bifunc(BI_IS_INTEGER,	"isInteger",	bi_isinteger); 	
    init_bifunc(BI_IS_DOUBLE,	"isDouble",	bi_isdouble);	
    init_bifunc(BI_IS_NUMBER,	"isNumber",	bi_isnumber);	
    init_bifunc(BI_IS_STRING,	"isString",	bi_isstring);	
    init_bifunc(BI_IS_SYMBOL,	"isSymbol",	bi_issymbol);	
    
    init_bifunc(BI_IS_LIST,	"isList",	bi_islist);	
    init_bifunc(BI_IS_NULL,	"isNull",	bi_isnull);	
    init_bifunc(BI_ECHO,	"echo",		bi_echo);	
    init_bifunc(BI_ECHOLN,	"echoln",	bi_echoln);	
    init_bifunc(BI_GET_INTEGER, "getInteger",	bi_getinteger);	
    
    init_bifunc(BI_GET_DOUBLE,	"getDouble",	bi_getdouble);
    init_bifunc(BI_GET_STRING,	"getString",	bi_getstring);	
    init_bifunc(BI_GET_LINE,	"getLine",	bi_getline);
    init_bifunc(BI_GET_SYMBOL,	"getSymbol",	bi_getsymbol);
    init_bifunc(BI_IS_EMPTY,	"isEmpty",	bi_isempty);
    
    init_bifunc(BI_LENGTH,	"length",	bi_length);
    init_bifunc(BI_TO_INTEGER,	"toInteger",	bi_tointeger);	
    init_bifunc(BI_TO_DOUBLE,	"toDouble",	bi_todouble);	
    init_bifunc(BI_TO_STRING,	"toString",	bi_tostring);	
    init_bifunc(BI_TO_SYMBOL,	"toSymbol",	bi_tosymbol);	
    
    init_bifunc(BI_IS_CHAR,  "isChar",	bi_ischaracter);	
    init_bifunc(BI_GET_CHAR, "getChar",	bi_getcharacter);	
    
    init_bifunc(BI_RAND, "rand", bi_rand);
    
    // Add them into the environment we created
    for (i = 0; i < MAX_BUILT_INS; i++)
    {
	SCALAR *tmp = new_scalar();
	tmp->value = built_in_functions[i];
	tmp->type = ST_BUILT_IN;
	put_in_env(env, built_in_keys[i], tmp);
    }
    
    return env;
}

// Prints "escaped objection strings" to standard out
void escape_print(const char *str)
{
    int pos;
    
    for (pos = 0; str[pos] != 0; pos++)
    {
	if (str[pos] == '\\')
	{
	    switch (str[++pos])
	    {
		case 'n':
		    printf("\n");
		    break;
		case 't':
		    printf("\t");
		    break;
		default:
		    printf("%c", str[pos]);
	    }
	}
	else
	{
	    printf("%c", str[pos]);
	}
    }
}

SCALAR *bi_isinteger(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there is exactly 1 argument
    exact_args("isInteger", 1);
    bi_tmp_scalar = interpret(n, env);
    
    // Determine if the evaluation is an integer
    if (bi_tmp_scalar == NULL)
	bi_tmp_int = 0;
    else if (bi_tmp_scalar->type == ST_LONG)
	bi_tmp_int = 1;
    else
	bi_tmp_int = 0;
    
    // Construct the return scalar
    bi_tmp_scalar = new_scalar();
    set_bool_value(bi_tmp_scalar, bi_tmp_int);
    
    // Return the value
    return bi_tmp_scalar;
}

SCALAR *bi_isdouble(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there is exactly 1 argument
    exact_args("isDouble", 1);
    bi_tmp_scalar = interpret(n, env);
    
    // Determine if the evaluation is a double
    if (bi_tmp_scalar == NULL)
	bi_tmp_int = 0;
    else if (bi_tmp_scalar->type == ST_DOUBLE)
	bi_tmp_int = 1;
    else
	bi_tmp_int = 0;
    
    // Construct the return scalar
    bi_tmp_scalar = new_scalar();
    set_bool_value(bi_tmp_scalar, bi_tmp_int);
    
    // Return the value
    return bi_tmp_scalar;
}

SCALAR *bi_isnumber(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there is exactly 1 argument
    exact_args("isNumber", 1);
    bi_tmp_scalar = interpret(n, env);
    
    // Determine if the evaluation is a number
    if (bi_tmp_scalar == NULL)
	bi_tmp_int = 0;
    else if (bi_tmp_scalar->type == ST_LONG ||
	     bi_tmp_scalar->type == ST_DOUBLE)
	bi_tmp_int = 1;
    else
	bi_tmp_int = 0;
    
    // Construct the return scalar
    bi_tmp_scalar = new_scalar();
    set_bool_value(bi_tmp_scalar, bi_tmp_int);
    
    // Return the value
    return bi_tmp_scalar;
}

SCALAR *bi_isstring(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there is exactly 1 argument
    exact_args("isString", 1);
    bi_tmp_scalar = interpret(n, env);
    
    // Determine if the evaluation is a string
    if (bi_tmp_scalar == NULL)
	bi_tmp_int = 0;
    else if (bi_tmp_scalar->type == ST_STRING)
	bi_tmp_int = 1;
    else
	bi_tmp_int = 0;
    
    // Construct the return scalar
    bi_tmp_scalar = new_scalar();
    set_bool_value(bi_tmp_scalar, bi_tmp_int);
    
    // Return the value
    return bi_tmp_scalar;
}

SCALAR *bi_issymbol(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there is exactly 1 argument
    exact_args("isSymbol", 1);
    bi_tmp_scalar = interpret(n, env);
    
    // Determine if the evaluation is a symbol
    if (bi_tmp_scalar == NULL)
	bi_tmp_int = 0;
    else if (bi_tmp_scalar->type == ST_SYMBOL)
	bi_tmp_int = 1;
    else
	bi_tmp_int = 0;
    
    // Construct the return scalar
    bi_tmp_scalar = new_scalar();
    set_bool_value(bi_tmp_scalar, bi_tmp_int);
    
    // Return the value
    return bi_tmp_scalar;
}

SCALAR *bi_islist(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there is exactly 1 argument
    exact_args("isList", 1);
    bi_tmp_scalar = interpret(n, env);
    
    // Determine if the evaluation is a list
    if (bi_tmp_scalar == NULL)
	bi_tmp_int = 0;
    else if (bi_tmp_scalar->type == ST_LIST)
	bi_tmp_int = 1;
    else
	bi_tmp_int = 0;
    
    // Construct the return scalar
    bi_tmp_scalar = new_scalar();
    set_bool_value(bi_tmp_scalar, bi_tmp_int);
    
    // Return the value
    return bi_tmp_scalar;
}

SCALAR *bi_isnull(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there is exactly 1 argument
    exact_args("isNull", 1);
    bi_tmp_scalar = interpret(n, env);
    
    // Determine if the evaluation is null
    if (bi_tmp_scalar == NULL)
	bi_tmp_int = 1;
    else if (bi_tmp_scalar->type == ST_NULL)
	bi_tmp_int = 1;
    else
	bi_tmp_int = 0;
    
    // Construct the return scalar
    bi_tmp_scalar = new_scalar();
    set_bool_value(bi_tmp_scalar, bi_tmp_int);
    
    // Return the value
    return bi_tmp_scalar;
}

SCALAR *bi_echo(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure they supply at least one argument
    min_args("echo", 1);
    
    while (n != NULL)
    {
	SCALAR *scalar = interpret(n, env);
	char *str = string_value(scalar);
	
	// Simply ignore imperative functions
	if (str == NULL)
	    continue;
	
	escape_print(str);
	n = n->next_sibling;
    }
    
    return NULL;
}

SCALAR *bi_echoln(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure they supply at least one argument
    min_args("echo", 1);
    
    while (n != NULL)
    {
	bi_tmp_scalar = interpret(n, env);
	bi_tmp_str = string_value(bi_tmp_scalar);
	
	// Simply ignore imperative functions
	if (bi_tmp_str == NULL)
	    continue;
	
	escape_print(bi_tmp_str);
	
	// The only difference from echo
	printf("\n");
	
	n = n->next_sibling;
    }
    
    return NULL;
}

SCALAR *bi_getinteger(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there are no arguments
    exact_args("getInteger", 0);
    
    // Grab the integer
    int i;
    scanf("%d", &i);
    
    // Construct the scalar
    bi_tmp_scalar = new_scalar();
    set_long_value(bi_tmp_scalar, i);
    
    // Finally return the scalar
    return bi_tmp_scalar;
}

SCALAR *bi_getdouble(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there are no arguments
    exact_args("getDouble", 0);
    
    // Grab the double
    double d;
    scanf("%lf", &d);
    
    // Construct the scalar
    bi_tmp_scalar = new_scalar();
    set_double_value(bi_tmp_scalar, d);
    
    // Finally return the scalar
    return bi_tmp_scalar;
}

SCALAR *bi_getstring(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there are no arguments
    exact_args("getString", 0);
    
    // Grab the string
    scanf("%s", bi_buf);
    
    // Construct the scalar
    bi_tmp_scalar = new_scalar();
    set_string_value(bi_tmp_scalar, bi_buf, strlen(bi_buf));
    
    // Finally return the scalar
    return bi_tmp_scalar;
}

SCALAR *bi_getline(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there are no arguments
    exact_args("getLine", 0);
    
    // Grab the string
    scanf("[^\n]", bi_buf);
    
    // Construct the scalar
    bi_tmp_scalar = new_scalar();
    set_string_value(bi_tmp_scalar, bi_buf, strlen(bi_buf));
    
    // Finally return the scalar
    return bi_tmp_scalar;
}

SCALAR *bi_getsymbol(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there are no arguments
    exact_args("getSymbol", 0);
    
    // Grab the symbol
    scanf("%s", bi_buf);
    
    // Construct the scalar
    bi_tmp_scalar = new_scalar();
    set_symbol_value(bi_tmp_scalar, bi_buf);
    
    // Finally return the scalar
    return bi_tmp_scalar;
}

SCALAR *bi_isempty(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    exact_args("isEmpty", 1);
    
    bi_tmp_scalar = interpret(n, env);
    
    // Make sure we evaluated a list
    if (bi_tmp_scalar == NULL)
	sem_err("'isEmpty' expects a list argument", n->line);
    else if (bi_tmp_scalar->type != ST_LIST)
	sem_err("'isEmpty' expects a list argument", n->line);
    
    assert( list_value(bi_tmp_scalar) );
    
    int i = (empty_list(list_value(bi_tmp_scalar))) ? 1 : 0;
    
    // Construct and return the scalar
    bi_tmp_scalar = new_scalar();
    set_bool_value(bi_tmp_scalar, i);
    return bi_tmp_scalar;
}

SCALAR *bi_length(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    exact_args("length", 1);
    
    bi_tmp_scalar = interpret(n, env);
    
    // Make sure we evaluated a list
    if (bi_tmp_scalar == NULL)
	sem_err("'length' expects a list argument", n->line);
    else if (bi_tmp_scalar->type != ST_LIST)
	sem_err("'length' expects a list argument", n->line);
    
    int i = sl_length(list_value(bi_tmp_scalar));
    
    // Construct and return the scalar
    bi_tmp_scalar = new_scalar();
    set_long_value(bi_tmp_scalar, i);
    return bi_tmp_scalar;
}

SCALAR *bi_tointeger(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    extern SCALAR_ERROR scalar_error;
 
    // Check for the correct number of arguments
    exact_args("toInteger", 1);
    
    // Attempt to make the cast
    bi_tmp_scalar = new_scalar();
    set_long_value(bi_tmp_scalar, long_value(interpret(n, env)));
    
    // See if an error flag was set
    if (scalar_error != SE_NONE)
	sem_err("'toInteger' was unable to cast argument to an integer", n->line);
    
    return bi_tmp_scalar;
}

SCALAR *bi_todouble(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    extern SCALAR_ERROR scalar_error;
 
    // Check for the correct number of arguments
    exact_args("toDouble", 1);
    
    // Attempt to make the cast
    bi_tmp_scalar = new_scalar();
    set_double_value(bi_tmp_scalar, double_value(interpret(n, env)));
    
    // See if an error flag was set
    if (scalar_error != SE_NONE)
	sem_err("'toDouble' was unable to cast argument to a double", n->line);
    
    return bi_tmp_scalar;
}

SCALAR *bi_tostring(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    extern SCALAR_ERROR scalar_error;
 
    // Check for the correct number of arguments
    exact_args("toString", 1);
    
    // Attempt to make the cast
    bi_tmp_scalar = new_scalar();
    bi_tmp_scalar2 = interpret(n, env);
    bi_tmp_str = string_value(bi_tmp_scalar2);
    set_string_value(bi_tmp_scalar, bi_tmp_str, strlen(bi_tmp_str));
    
    // See if an error flag was set
    if (scalar_error != SE_NONE)
	sem_err("'toString' was unable to cast argument to a string", n->line);
    
    return bi_tmp_scalar;
}

SCALAR *bi_tosymbol(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    extern SCALAR_ERROR scalar_error;
 
    // Check for the correct number of arguments
    exact_args("toSymbol", 1);
    
    // Attempt to make the cast
    bi_tmp_scalar = new_scalar();
    set_symbol_value(bi_tmp_scalar, string_value(interpret(n, env)));
    
    // See if an error flag was set
    if (scalar_error != SE_NONE)
	sem_err("'toSymbol' was unable to cast argument to a symbol", n->line);
    
    return bi_tmp_scalar;
}

SCALAR *bi_ischaracter(PARSE_NODE *n, ENVIRONMENT *env)
{
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there is exactly 1 argument
    exact_args("isCharacter", 1);
    bi_tmp_scalar = interpret(n, env);
    
    // Determine if the evaluation is a symbol
    if (bi_tmp_scalar == NULL)
	bi_tmp_int = 0;
    else if (bi_tmp_scalar->type == ST_CHARACTER)
	bi_tmp_int = 1;
    else
	bi_tmp_int = 0;
    
    // Construct the return scalar
    bi_tmp_scalar = new_scalar();
    set_bool_value(bi_tmp_scalar, bi_tmp_int);
    
    // Return the value
    return bi_tmp_scalar;
}

SCALAR *bi_getcharacter(PARSE_NODE *n, ENVIRONMENT *env)
{
    char tg;
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    
    // Make sure there are no arguments
    exact_args("getCharacter", 0);
    
    // Grab the symbol
    scanf("%c", &tg);
    
    // Construct the scalar
    bi_tmp_scalar = new_scalar();
    set_char_value(bi_tmp_scalar, tg);
    
    // Finally return the scalar
    return bi_tmp_scalar;
}

SCALAR *bi_rand(PARSE_NODE *n, ENVIRONMENT *env)
{
    char tg;
    char bi_buf[512];
    char *bi_tmp_str = NULL;
    SCALAR *bi_tmp_scalar = NULL;
    SCALAR *bi_tmp_scalar2 = NULL;
    int bi_tmp_int = 0;
    long low = 0;
    long high = 0;
    long res = 0;
    long res2 = 0;
    
    // Rand needs exactly 2 arguments, low and high
    exact_args("rand", 2);

    // See of we need to seed
    if (!seeded)
    {
	seeded = 1;
	srand((unsigned)time( NULL ));
    }
    
    
    bi_tmp_scalar = interpret(n, env);
    bi_tmp_scalar2 = interpret(n->next_sibling, env);
    
    if (bi_tmp_scalar->type != ST_LONG || bi_tmp_scalar2->type != ST_LONG)
	sem_err("Function 'rand' requires that both arguments be of type integer", n->line);
	
    low =  long_value(bi_tmp_scalar);
    high = long_value(bi_tmp_scalar2);
    
    if (low >= high)
	sem_err("Function 'rand' requires the first argument to be less than the second argument", n->line);
	
    if (low < 0)
	sem_err("Function 'rand' requires the first argument to be greater than or equal to zero", n->line);
    
    if (high <= 0)
	sem_err("Function 'rand' requires the second argument to be greater than zero", n->line);
    
    
    // Okay, so everything checks out, do the actual number generation
    res = rand();
    res = res >> 16;
    res2 = rand();
    res2 = res2 >> 16;
    res2 = res2 << 16;
    
    res = ((res | res2) % high) + low;

    bi_tmp_scalar = new_scalar();
    set_long_value(bi_tmp_scalar, res);
    return bi_tmp_scalar;
}

