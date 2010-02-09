/*
    FILE:	scalar.c
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards
    
    This is the implementation of the scalar library,
    see scalar.h for details on how to use the scalar 
    library.
    
    Copyright 2005 Ryan Sandor Richards, All rights reserved.
*/

#include "scalar.h"

void display_scalar_type(SCALAR *s)
{ 
    switch(s->type)
    {
	case ST_UNDEFINED:
	    printf("ST_UNDEFINED");
	    return;
	    
	case ST_LONG: 
	    printf("ST_LONG");
	    return;
	    
	case ST_DOUBLE:
	    printf("ST_DOUBLE");
	    return;
	    
	case ST_STRING:
	    printf("ST_STRING");
	    return;
	    
	case ST_BOOLEAN:
	    printf("ST_BOOLEAN");
	    return;
	    
	case ST_CONTINUATION:
	    printf("ST_CONTINUATION");
	    return;
	    
	case ST_BUILT_IN:
	    printf("ST_BUILT_IN");
	    return;
	    
	case ST_SYMBOL:
	    printf("ST_SYMBOL");
	    return;
	
	case ST_LIST:
	    printf("ST_LIST");
	    return;
    }
    
    printf("Unknown");
}

/**
 * If scalar_error is set (i.e. not equal to
 * SE_NONE), then the appropriate error string is
 * sent to stderr. If the pre parameter is not
 * NULL then the pre is sent followed by a colon
 * and space with the error string output after
 * that. Resets the scalar_error as well.
 *
 * @param pre Concatenated before the actual error string.
 *
 */
void scalar_error_out(const char *pre)
{
    /* Do not print anything if scalar_error is not set */
    if (scalar_error == SE_NONE)
	return;

    /* Print the pre if it is not null or empty*/
    if (pre != NULL)
	if (*pre)
	    printf("%s: ", pre);

    /* Print the appropriate error string */
    switch (scalar_error)
    {
	case SE_NULL_SCALAR:
	    printf( "NULL scalar");
	    break; 
	case SE_NULL_VALUE:
	    printf( "NULL scalar value");
	    break; 
	case SE_VALUE_REALLOC:
	    printf( "Value re-allocation failed");
	    break;
	case SE_NULL_STRING:
	    printf( "Attempted use of a NULL string");
	    break;
	case SE_CIRCULAR_REFERENCE:
	    printf( "Circular scalar reference");
	    break;
	case SE_RANGE:
	    printf( "Value out of scalar range");
	    break;
	case SE_INVALID_TYPE_CAST:
	    printf( "Invalid dynamic cast");
	    break;
    }
    
    scalar_error = SE_NONE;
}


/**
 * new_scalar()
 *
 * Description:
 *	Allocates memory for and initializes a new 
 * pointer to a scalar.
 *
 * Returns:
 *	A valid pointer to a new scalar, or NULL if
 * an error occured while attempting to allocate or 
 * initialize the new scalar.
 *	
 * Errors:
 * SE_NULL_SCALAR -
 *	if memory for the scalar could not be allocated. 
 *
 * SE_NULL_VALUE -
 *	if memory for the value of the scalar could not 
 *	be allocated.
 *
 */
SCALAR *new_scalar()
{
    /* Declare the error handling externals */
    extern SCALAR_ERROR scalar_error;
    SCALAR *s;
    
    /* Attempt to allocate memory for the scalar */
    assert((s = (SCALAR *)GC_MALLOC(sizeof( SCALAR ))) != NULL);
    
    /* Make sure that the memory has been allocated properly */
    if (s == NULL)
    {
	/* It did not, set the scalar_error and return NULL */
	scalar_error = SE_NULL_SCALAR;
	return NULL;
    }
    
    /* Attempt to allocate memory for the scalar value */
    assert(s->value = GC_MALLOC( sizeof(long) ));
    
    /* Make sure the value allocated correctly */
    if (s->value == NULL)
    {
	/* It did not, set the scalar_erro and return NULL */
	scalar_error = SE_NULL_VALUE;
	return NULL;
    }
    
    *(long *)s->value = 0;
    
    /* Initialize the fields of the structure */
    s->type = ST_LONG;
    
    // Needs to be null, only used by MLOR operations
    s->next_op = NULL;
    
    /* Clear the scalar error and return the new scalar */
    scalar_error = SE_NONE;
    return s;    
}


/**
 * set_long_value()
 *
 * Description:
 *	Sets the value of a scalar to a long value,
 * and the scalars type field to that of ST_LONG.
 *
 * Parameters:
 *	SCALAR *scalar - Scalar to set the value of
 *	long i - long integer value to set the scalar to
 *
 * Returns:
 *	A non-zero integer on success, and zero on failure.
 *	
 * Errors:
 * SE_NULL_SCALAR -
 *	If the supplied scalar to set the value of
 *	is null.
 *
 * SE_NULL_VALUE -
 *	If the value of the scalar supplied was NULL
 * 
 * SE_VALUE_REALLOC -
 *	If an error occured while trying to reallocate
 *	the memory for the scalar value.
 */
int set_long_value(SCALAR *scalar, long i)
{
    /* Declare the error handling externals */
    extern SCALAR_ERROR scalar_error;
    
    assert(scalar);
    assert(scalar->value);
    
    if (!scalar->value == ST_LONG)
    {
	GC_FREE(scalar->value);
	assert( scalar->value = GC_MALLOC_ATOMIC( sizeof(long) ) );
    }
    
    /* Set the scalar's value and type */
    *(long *)scalar->value = i;
    scalar->type = ST_LONG;
    
    /* Clear the scalar error and return a non-zero number */
    scalar_error = SE_NONE;
    return 1;
}


int set_bool_value(SCALAR *scalar, long b)
{
    set_long_value(scalar, b);
    scalar->type = ST_BOOLEAN;
    scalar_error = SE_NONE;
    return 0;
}


int set_double_value(SCALAR *scalar, double d)
{
    extern SCALAR_ERROR scalar_error;

    assert(scalar);
    assert(scalar->value);
    
    if (scalar->type != ST_DOUBLE)
    {
	GC_FREE(scalar->value);
	assert(scalar->value = GC_MALLOC_ATOMIC( sizeof(double) ));
    }
    
    /* Set the scalar's value and type */
    *(double *)scalar->value = d;
    scalar->type = ST_DOUBLE;
    
    return 1;
}


int set_string_value(SCALAR *scalar, char *s, unsigned int len)
{
    /* Declare the error handling externals */
    extern SCALAR_ERROR scalar_error;
    
    assert(scalar);
    assert(s);
    
    GC_FREE(scalar->value);
    assert(scalar->value = GC_MALLOC_ATOMIC( sizeof(char) * len ));
    
    /* Set the value and type of the scalar */
    char *stv = (char *)scalar->value;
    
    strcpy(stv, s); 
    scalar->type = ST_STRING;
    
    return 1;
}


// Sets a scalar to the value of a character
int set_char_value(SCALAR *scalar, char s)
{
    /* Declare the error handling externals */
    extern SCALAR_ERROR scalar_error;
    
    assert(scalar);
    assert(scalar->value);
	
    if (scalar->type != ST_CHARACTER)
    {
	GC_FREE(scalar->value);
	assert(scalar->value = GC_MALLOC_ATOMIC( sizeof(char) ));
    }
    
    *(char *)scalar->value = s;
    scalar->type = ST_CHARACTER;
    return 1;
}


/**
 * long_value()
 *
 * Description:
 *	This function first determines what type of value
 * is being stored in the scalar, then makes the appropriate
 * typecasts and returns the value being stored's "long
 * equivilent" (a long integer number representation of the
 * scalar's actual value).
 *
 * Please note that error checking with this function is
 * dependant on seeing whether or not the scalar_error 
 * external has been set or not.
 *
 * Parameters:
 *	SCALAR *s - Scalar to retrieve the long value from
 *
 * Returns:
 *	The scalar's "long equivilent", if the scalar is
 * of the type ST_STRING and no value can be determined
 * then zero is simply returned. If the value of the scalar
 * is a string of numbers that is out of range of the normal
 * long integer, then the value returned is cut off at the
 * minimum or maximum value possible. If the scalar's type
 * cannot be determined, then the function simply returns
 * zero.
 *	
 * Errors:
 * SE_NULL_SCALAR -
 *	If the scalar provided is NULL
 * 
 * SE_NULL_VALUE -
 *	If the value of the scalar provide is NULL
 *
 * SE_RANGE -
 *	If the value of the scalar was outside the
 *	range of a long value.
 *
 * SE_INVALID_TYPE_CAST -
 *	If the scalar's type could not be converted to
 *	a long value.
 */
long long_value(SCALAR *s)
{
    extern SCALAR_ERROR scalar_error;
    extern int errno;
    long v;
    
    if (s == NULL)
    {
	scalar_error = SE_NULL_SCALAR;
	return 0;
    }

    if (s->value == NULL)
    {
	scalar_error = SE_NULL_VALUE;
	return 0;
    }

    switch (s->type)
    {
	case ST_LONG:
	case ST_BUILT_IN:
	case ST_REGISTER:
	    scalar_error = SE_NONE;
	    return *((long *)s->value);
	    
	case ST_DOUBLE:
	    scalar_error = SE_NONE;
	    return (long)(*((double *)s->value));

	case ST_STRING:
	    /* Convert the string into an integer */
	    v = strtol((char *)s->value, NULL, 10);
	    
	    
	    /* Make sure the value is in range */
	    if (errno == ERANGE)
	    {
		scalar_error = SE_RANGE;
	    }
	    else
	    {
		scalar_error = SE_NONE;
	    }
	    
	    return v;
	
	case ST_CHARACTER:
	    scalar_error = SE_NONE;
	    return (long)(*(char *)s->value);
	
	case ST_BOOLEAN:
	    return *(long *)s->value;
    }
    
    /* 
	The scalar's value type is not able to be dynamically
	cast to a long value, set the scalar error and return
	zero.
    */
    scalar_error = SE_INVALID_TYPE_CAST;
    return 0;
}


/**
 * double_value()
 *
 * Description:
 *	This function first determines what type the scalar
 * provided is, and then returns its "double equivilent".
 *
 * Parameters:
 *	SCALAR *s - scalar to find the double equivilent
 *		    of.
 *
 * Returns:
 *	The double equivilent of a scalar value if the scalar
 * can be converted into a double. If the scalar is a string
 * value and no conversion can be done, zero is simply returned.
 * If the scalar represents a type that cannot be converted
 * into a double, then zero is returned.
 *	
 * Errors:
 * SE_NULL_SCALAR -
 *	If the scalar provided is NULL.
 *
 * SE_NULL_VALUE -
 *	If the scalar provided has a NULL value field.
 *
 * SE_RANGE -
 *	If the scalar was a string that represented a number
 *	that is too large to be stored ina scalar.
 *
 * SE_INVALID_TYPE_CAST -
 *	If the scalar represents a value that cannot be
 *	converted to a double.
 */
double double_value(SCALAR *s)
{
    extern SCALAR_ERROR scalar_error;
    extern int errno;
    double v;
    
    if (s == NULL)
    {
	scalar_error = SE_NULL_SCALAR;
	return 0.0;
    }

    if (s->value == NULL)
    {
	scalar_error = SE_NULL_VALUE;
	return 0.0;
    }

    
    switch (s->type)
    {
	case ST_BUILT_IN:
	case ST_CONTINUATION:
	case ST_BOOLEAN:
	case ST_LONG:
	    scalar_error = SE_NONE;
	    return (double)*(long *)s->value;
	    
	case ST_DOUBLE:
	    scalar_error = SE_NONE;
	    return *(double *)s->value;

	case ST_SYMBOL:
	case ST_STRING:
	    /* Convert the string into an integer */
	    v = strtod((char *)s->value, NULL);
	    
	    /* Make sure the value is in range */
	    if (errno == ERANGE)
	    {
		scalar_error = SE_RANGE;
	    }
	    else
	    {
		scalar_error = SE_NONE;
	    }
	    
	    return v;
    }

    /* 
	The scalar's value type is not able to be dynamically
	cast to a double value, set the scalar error and return
	zero.
    */
    scalar_error = SE_INVALID_TYPE_CAST;
    return 0;
}


/**
 * string_value()
 *
 * Description:
 *	This function determines a scalar's "String equivilent" if
 * the scalar can be dynamically cast into a string.
 *
 * Parameters:
 *	SCALAR *s - scalar to determine the equivilent of
 *	
 * Returns:
 *	A new pointer to the string value of the scalar.
 *
 * Errors:
 * SE_NULL_SCALAR -
 *	If the supplied scalar was null.
 *
 * SE_NULL_VALUE -
 *	If the supplied scalar's value was null.
 *
 * SE_INVALID_TYPE_CAST -
 *	If the scalar could not be converted to a string.
 *
 */
char *string_value(SCALAR *s)
{
    // Size on this one might need to change...
    char *str = GC_MALLOC_ATOMIC( sizeof(char) * 1024 );
    assert(str != NULL);
    
    extern SCALAR_ERROR scalar_error;
    
    if (s == NULL)
    {
	scalar_error = SE_NULL_SCALAR;
	return NULL;
    }

    if (s->value == NULL)
    {
	scalar_error = SE_NULL_VALUE;
	return NULL;
    }
    
    switch (s->type)
    {
	case ST_NULL:
	    sprintf(str, "NULL");
	    break;
    
	case ST_BUILT_IN:
	    sprintf(str, "<built-in>");
	    break;
	    
	case ST_FILE:
	    sprintf(str, "<file>");
	    break;
	    
	case ST_LONG:
	    scalar_error = SE_NONE;
	    sprintf(str, "%d", *(long *)s->value);
	    break;
	
	case ST_DOUBLE:
	    scalar_error = SE_NONE;
	    sprintf(str, "%g", *(double *)s->value);
	    break;
	
	case ST_SYMBOL:
	case ST_STRING:
	    scalar_error = SE_NONE;
	    strcpy(str, (char *)s->value);
	    break;
	    
	case ST_CONTINUATION:
	    scalar_error = SE_NONE;
	    strcpy(str, "<function>");
	    break;
	
	case ST_BOOLEAN:
	    scalar_error = SE_NONE;
	    sprintf(str, "%s", long_value(s) ? "true" : "false");
	    break;
	
	case ST_LIST:
	{
	    SCALAR_LIST *tmp = (SCALAR_LIST *)s->value;
	    
	    while (!is_head(tmp))
		tmp = tmp->next;
	    
	    tmp = tmp->next;
	    
	    strcpy(str, "(");
	    while (!is_foot(tmp))
	    {
		if (tmp->value->type == ST_STRING)
		    strcat(str, "\"");
		
		strcat(str, string_value(tmp->value));
		
		if (tmp->value->type == ST_STRING)
		    strcat(str, "\"");
			
		if (!is_foot(tmp->next))
		    strcat(str, " ");
		
		tmp = tmp->next;
	    }
	    strcat(str, ")");
	    
	    break;
	}
	
	case ST_CHARACTER:
	    sprintf(str, "%c", char_value(s));
	    break;
	
	case ST_REGISTER:
	    sprintf(str, "$%d", long_value(s));
	    break;
	
	default:
	    scalar_error = SE_INVALID_TYPE_CAST;
	    str = NULL;
	    break;
    }
    
    return str;
}

// Char values can come from only characters
// and integers.
char char_value(SCALAR *s)
{
    if (s->type == ST_CHARACTER)
	return *(char *)s->value;
    else if (s->type == ST_LONG)
	return (char)*(long *)s->value;
    
    scalar_error = SE_INVALID_TYPE_CAST;
    return 0;
}


/**
 * FIX-ME: Add documentation
 */
SCALAR *copy_scalar(SCALAR *s)
{
    if (s == NULL)
	return NULL;

    SCALAR *r = new_scalar();
    r->type = s->type;
    
    switch(s->type)
    {
	case ST_UNDEFINED:
	case ST_NULL:
	    r->value = NULL;
	    break;
	
	case ST_BUILT_IN:
	    r->value = s->value;
	    break;
	    
	case ST_LONG:
	    set_long_value(r, long_value(s));
	    break;
	
	case ST_DOUBLE:
	    set_double_value(r, double_value(s));
	    break;
	
	case ST_STRING:
	    set_string_value(r, string_value(s), strlen((char *)s->value));
	    break;
	
	case ST_SYMBOL:
	    set_string_value(r, string_value(s), strlen((char *)s->value));
	    r->type = ST_SYMBOL;
	    break;
	
	case ST_CONTINUATION:
	    r->value = s->value;
	    break;
	
	case ST_BOOLEAN:
	    set_bool_value(r, long_value(s));
	    break;
	    
	case ST_LIST:
	    r->value = copy_list((SCALAR_LIST *)s->value);
	    break;
	    
	case ST_CHARACTER:
	    set_char_value(r, char_value(s));
	    break;
    }
    
    return r;
}

// Sets a scalar as symbol value
int set_symbol_value(SCALAR *scalar, char *s)
{
    assert(scalar);
    assert(s);
    
    set_string_value(scalar, s, strlen(s));
    scalar->type = ST_SYMBOL;
    return 1;
}

// determines whether a scalar represents a list containing no elements
int empty_list_scalar(SCALAR *scalar)
{
    if (scalar == NULL)
	return 0;
	
    if (scalar->value == NULL)
	return 0;
	
    if (sc_type(scalar) != ST_LIST)
	return 0;

    SCALAR_LIST *tmp_list = to_list(scalar);
    
    return empty_list(tmp_list);
}

// Sets a scalar's value to that of a scalar list
void set_list_value(SCALAR *scalar, SCALAR_LIST *list)
{
    scalar->value = list;
    scalar->type = ST_LIST;
}

// Returns the actual list, use copy scalar to get
// a copy of the list.
SCALAR_LIST *list_value(SCALAR *scalar)
{
    if (scalar->type != ST_LIST)
	return NULL;
    
    return (SCALAR_LIST *)scalar->value;
}

// Creates a new empty scalar list
SCALAR_LIST *new_scalar_list()
{
    SCALAR_LIST *head = (SCALAR_LIST *)GC_MALLOC( sizeof(SCALAR_LIST) );
    SCALAR_LIST *foot = (SCALAR_LIST *)GC_MALLOC( sizeof(SCALAR_LIST) );
    
    assert(head != NULL && foot != NULL);
    
    head->type = SLT_HEADER;
    head->next = foot;
    head->prev = foot;
    head->value = NULL;
    
    foot->type = SLT_FOOTER;
    foot->next = head;   
    foot->prev = head;
    foot->value = NULL;
	        
    return head;
}

// Creates a new list entry
// Can be called outside the
// library, but technically
// shouldn't be needed.
SCALAR_LIST *new_list_entry(SCALAR_LIST *prev, SCALAR_LIST *next, SCALAR *val)
{
    SCALAR_LIST *tmp = (SCALAR_LIST *)GC_MALLOC( sizeof(SCALAR_LIST) );
    assert(tmp != NULL);
    tmp->next = next;
    tmp->prev = prev;
    tmp->value = val;
    tmp->type = SLT_VALUE;
    
    return tmp;
}

// Makes an exact copy of a list
SCALAR_LIST *copy_list(SCALAR_LIST *list)
{
    if (list == NULL)
	return NULL;

    SCALAR_LIST *new_list = new_scalar_list();
    SCALAR_LIST *tmp_li = new_list;
    SCALAR *tmp_sc = NULL;
    
    while (list->type != SLT_HEADER)
	list = list->next;
    
    list = list->next;
    
    while (list->type != SLT_FOOTER)
    {
	tmp_sc = copy_scalar(list->value);
	tmp_li = append(tmp_li, tmp_sc);
	list = list->next;
    }
    
    return new_list;
}


// Appends a value on to a list
// Returns the position of the last
// append, if used properly one can
// construct lists in low asymptotic
// time using this function (takes
// some of the tediousness out of the
// process of coding list constructions)
SCALAR_LIST *append(SCALAR_LIST *list, SCALAR *value)
{
    SCALAR_LIST *tmp;
    
    if (list == NULL)
	return NULL;
    
    while (list->type != SLT_FOOTER)
	list = list->next;
	
    tmp = new_list_entry(list->prev, list, value);
    list->prev->next = tmp;
    list->prev = tmp;
    
    return tmp;
}

// Returns a scalar at a specified index
// in a list, null if the scalar could
// not be found
//
// NOTE: This is NOT a copy, it is the
// actual scalar the list holds, if you
// need a copy the following code should
// suffice:
//
// SCALAR *cpy = copy_scalar(get_from_list(lst, 10));
//
SCALAR *get_from_list(SCALAR_LIST *list, long index)
{
    int i = 0;
    
    if (index < 0)
	return NULL;
    
    if (empty_list(list))
	return NULL;
    
    if (list->type == SLT_HEADER)
	list = list->next;
        	
    while (list->type != SLT_FOOTER && i != index)
    {
	i++;
	list = list->next;
    }
    
    if (list->type == SLT_FOOTER)
	return NULL;
    
    return list->value;
}

// Inserts a value to the front of the scalar list
SCALAR_LIST *insert_front(SCALAR_LIST *list, SCALAR *value)
{
    while(list->type != SLT_HEADER)
	list = list->prev;
    
    SCALAR_LIST *tmp = new_list_entry(list, list->next, value);
    list->next->prev = tmp;
    list->next = tmp;
    
    return tmp;
}


// Returns the length of a list
long sl_length(SCALAR_LIST *list)
{
    int i = 0;
    
    SCALAR_LIST *tmp = list->prev;
        
    while (list != tmp)
    {
	if (!is_head(list) && !is_foot(list))
	    i++;
	
	list = list->next;
    }
    
    return i;
}


// Determines if two scalars are equal
// Revised to be really quite fast
long eq_scalar(SCALAR *a, SCALAR *b)
{
    if (a == NULL || b == NULL)
	return (a == NULL && b == NULL);

    if (a->type != b->type)
	return 0;

    switch (a->type)
    {
	case ST_UNDEFINED:
	    return 0;
	    
	case ST_LONG:
	case ST_BOOLEAN:
	    return (long_value(a) == long_value(b));
	
	case ST_DOUBLE:
	    return (double_value(a) == double_value(b));
	
	case ST_NULL:
	    return 1;
	
	default:
	    return (!strcmp(string_value(a), string_value(b)) ? 1 : 0);
    }
}

// Simple equvilence, works for basically everything
long eqv_scalar(SCALAR *a, SCALAR *b)
{
    if (a == NULL || b == NULL)
	return (a == NULL && b == NULL);
	
    return (!strcmp(string_value(a), string_value(b)) ? 1 : 0);
}

long lt_scalar(SCALAR *a, SCALAR *b)
{
    if (a == NULL || b == NULL)
	return (a == NULL && b == NULL);

    if (a->type != b->type)
	return 0;

    switch (a->type)
    {
	case ST_UNDEFINED:
	    return 0;
	    
	case ST_LONG:
	case ST_BOOLEAN:
	    return (long_value(a) < long_value(b));
	
	case ST_DOUBLE:
	    return (double_value(a) < double_value(b));
	
	case ST_NULL:
	    return 0;
	
	default:
	    return strcmp(string_value(a), string_value(b)) <= 0 ? 1 : 0;
    }
}

long gt_scalar(SCALAR *a, SCALAR *b)
{
    if (a == NULL || b == NULL)
	return (a == NULL && b == NULL);

    if (a->type != b->type)
	return 0;

    switch (a->type)
    {
	case ST_UNDEFINED:
	    return 0;
	    
	case ST_LONG:
	case ST_BOOLEAN:
	    return (long_value(a) > long_value(b));
	
	case ST_DOUBLE:
	    return (double_value(a) > double_value(b));
	
	case ST_NULL:
	    return 0;
	
	default:
	    return strcmp(string_value(a), string_value(b)) > 0 ? 1 : 0;
    }
}

int set_file_value(SCALAR *scalar, FILE *f)
{
    if (scalar == NULL || f == NULL)
    {
	scalar_error = SE_NULL_SCALAR;
	return 0;
    }
    
    scalar->value = (void *)f;
    return 1;
}

FILE *file_value(SCALAR *s)
{
    extern SCALAR_ERROR scalar_error;
    
    if (s->type != ST_FILE)
    {
	scalar_error = SE_INVALID_TYPE_CAST;
	return NULL;
    }
    
    scalar_error = SE_NONE;
    return (FILE *)s->value;
}





