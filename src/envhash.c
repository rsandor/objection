/*
    FILE:	envhash.c
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards
    
    This is the implementation of the the envhash library
    for an environment variable hash.
*/

#include "envhash.h"

void display_env(ENVIRONMENT *env)
{
    printf("Env Contents:\n");
    
    int i;
    for (i = 0; i < MAX_ENTRIES; i++)
    {
	if (env->entries[i] != NULL)
	{
	    int j = 0;
	    BUCKET_ENTRY *b = env->entries[i];
	    while (b != NULL)
	    {
		SCALAR *s = b->value;
		
		if (s != NULL)
		{
		    printf(
			"\tBucket(%4d,%4d): Key: %10s, Memory Address (HEX): %10X Type: ",
			i,
			j, 
			b->key, 
			s->value
			);
		
		    printf("Type: ");
		    display_scalar_type(s);
		    
		    if (s->type != ST_BUILT_IN && s->type != ST_CONTINUATION)
			printf("\t\tvalue: %s", string_value(s));
		
		    printf("\n");
		}
		
		
		j++;
		b = b->next;
	    }
	}
    }
}

/**
    env_error_out() -
    
    Description:
	Outputs an error string to stdout if the error
	variable is set.
	
    Parameters:
	char *str - String to print before the error, a
	    colon and space are concatinated after for
	    convience.
*/
void env_error_out(char *str)
{
    if (str != NULL)
	printf("%s: ", str);
	
    switch (env_error)
    {
	case (EE_NONE):
	    return;
	case (EE_NULL_ARGUMENT):
	    printf("Null argument encountered\n");
	    return;
	case (EE_NULL_BUCKET):
	    printf("Null bucket entry\n");
	    return;
	case (EE_NULL_BUCKET_VALUE):
	    printf("Null bucket entry value\n");
	    return;
	case (EE_NULL_ENVIRONMENT):
	    printf("Null environment\n");
	    return;
	case (EE_SCALAR_NOT_FOUND):
	    printf("Scalar not found\n");
	    return;
    }
    
    env_error = SE_NONE;
}


// Just for testing, remove later
void eo(char *s)
{
    switch (env_error)
    {
	case (EE_NONE):
	    sprintf(s, "");
	    return;
	case (EE_NULL_ARGUMENT):
	    sprintf(s, "Null argument encountered");
	    return;
	case (EE_NULL_BUCKET):
	    sprintf(s, "Null bucket entry");
	    return;
	case (EE_NULL_BUCKET_VALUE):
	    sprintf(s, "Null bucket entry value");
	    return;
	case (EE_NULL_ENVIRONMENT):
	    sprintf(s, "Null environment");
	    return;
	case (EE_SCALAR_NOT_FOUND):
	    sprintf(s, "Scalar not found");
	    return;
    }
    
    env_error = SE_NONE;
}


/**
    hash() -
 
    Description:
	Finds hashcodes using cyclic shifts, then converts the 
	hashcodes to bucket array positions using the MAD method 
	(multiply add and divide).
 
	For best performance, choose a max bucket array size
	to be a prime number (see the MAX_ENTRIES definition in
	envhash.h).
	
	This function shouldn't be called outside of the envhash
	library.
 
    Parameters:
	const char *hash_string - String to be hashed
    
    Return:
	position in the bucket array the hash_string maps to
*/
unsigned int hash(const char *hash_string)
{
    if (hash_string == NULL)
    {
	env_error = EE_NULL_ARGUMENT;
	return -1;
    }

    // Set the hash code initially to zero
    int h = 0;
    int pos;
    
    // Perform the cyclic shift of the running sum of the
    // typecasted characters to integers.
    for (pos = 0; hash_string[pos] != 0; pos++)
    {
	h = (h << 6);
	h += (unsigned int)hash_string[pos];
    }
    
    // Figure the bucket array position
    h = h % MAX_ENTRIES;
    
    if (h < 0)
    {
	h = -h;
	//printf("hash(): negative hash position... interesting...");
    }
    
    // Finally return the hash code
    env_error = SE_NONE;
    
    
    return h;
}


/**
    new_bucket_entry() -
    
    Description:
	Allocates space for a new bucket entry and
    initializes it with a new scalar. This is an internal
    library function and never needs to be called outside
    the library.
    
    Param:
	const char *key - Key for the bucket entry
    
    Return:
	A pointer to a freshly made bucket entry
    
    Errors:
	EE_NULL_ARGUMENT - 
	If the key supplied was null.
    
	EE_NULL_BUCKET -
	If an error occured when trying to allocate memory
	for the bucket entry.
    
	EE_NULL_BUCKET_VALUE -
	If an error occured when trying to allocate memory
	for the new bucket's value.
*/
BUCKET_ENTRY *new_bucket_entry(const char *key)
{
    BUCKET_ENTRY *b;
    

    /* Make sure the key supplied is valid */
    if (key == NULL)
    {
	env_error = EE_NULL_ARGUMENT;
	return NULL;
    }
    
    /* Create space for the bucket entry */
    assert(b = (BUCKET_ENTRY *)GC_MALLOC( sizeof(BUCKET_ENTRY) ));
    
    
    /* Make sure that the alloc worked */
    if (b == NULL)
    {
	env_error = EE_NULL_BUCKET;
	return NULL;
    }
    
    /* Initialize thine bucket entry */
    b->next = NULL;
    b->previous = NULL;
    assert(b->key = (char *)GC_MALLOC_ATOMIC( sizeof(char) * strlen(key) ));
    strcpy(b->key, key);
    b->value = new_scalar();
    
    /* Make sure the scalar allocation didn't crap out */
    if (b->value == NULL)
    {
	/* it did... */
	env_error = EE_NULL_BUCKET_VALUE;
	return NULL;
    }
    
    /* Clear the error, and finally return the new bucket */
    env_error = EE_NONE;
    return b;
}


/**
    new_env() -
    
    Description:
	Allocates space for and creates a new environment
	with no entries.
    
    Return:
	A new environment with no entries on success,
	on failure a NULL pointer.
    
    Error:
    EE_NULL_ENVIRONMENT -
	If an error occured while attempting to allocate
	space for the new environment.
*/  
ENVIRONMENT *new_env()
{
    /* Attempt to create the new environment */
    ENVIRONMENT *env = (ENVIRONMENT *)GC_MALLOC( sizeof(ENVIRONMENT) );
    
    assert(env);
    
    /* Make sure alloc didn't puke */
    if (env == NULL)
    {
	env_error = EE_NULL_ENVIRONMENT;
	return NULL;
    }
    

    /* Initialize each of the arrays entries to null */
    int i;
    for (i = 0; i < MAX_ENTRIES; i++)
	env->entries[i] = NULL;
    
    env->higher = NULL;
    env->lower = NULL;
    
    /* Clear the error and return the environment */
    env_error = EE_NONE;
    return env;
}

/**
    put_in_env() -
    
    Description:
	Attempts to add a scalar to an environment via a
	string key. If the key is already being used by
	another scalar, then the value is replaced and
	the scalar replaced is de-allocated. If the scalar
	found at the specified key position is the same
	scalar that is sent to this function then the
	function will do nothing.
	
    Parameters:
	ENVIRONMENT *env - Environment to put the scalar in
	char *key - Key for the scalar
	SCALAR *scalar - Scalar to put into the environment
    
    Errors:
	EE_NULL_ARGUMENT -
	If any of the pointers passed to the function
	are NULL.
*/
void put_in_env(ENVIRONMENT *env, char *key, SCALAR *scalar)
{
    int pos;
    BUCKET_ENTRY *b;
    
    /* Make sure none of the arguments are null */
    if (env == NULL || key == NULL)
    {
	env_error = EE_NULL_ARGUMENT;
	return;
    }
    
    // This is a special case for when a null scalar
    // is passed (which can happen). So it creates
    // a new scalar with the null type an null value
    // in stead (so not to fuzz with the interpreter)
    if (scalar == NULL)
    {
	scalar = new_scalar();
	scalar->type = ST_NULL;
	scalar->value = NULL;
    }
    
    /* Find the bucket to add the scalar to */
    pos = hash(key);
    
    /* Check for conflicts */
    if (env->entries[pos] != NULL)
    {
	BUCKET_ENTRY *entry = env->entries[pos];
	
	do
	{
	    if (!strcmp(key, entry->key))
	    {
		if (scalar != entry->value)
		{
		    entry->value = scalar;
		}
		
		return;
	    }
	    
	    if (entry->next == NULL)
	    {
		b = new_bucket_entry(key);
		b->value = scalar;
		b->previous = entry;
		entry->next = b;
		return;
	    }
	}
	while (entry = entry->next);
    }
    
    b = new_bucket_entry(key);
    b->value = scalar;
    env->entries[pos] = b;
    
    env_error = SE_NONE;
}


/**
    get_from_env() -
    
    Description:
	Determines a scalar via a string key.
    
    Parameters:
	ENVIRONMENT *env - envrionment to search in
	
	char *key - key for the scalar value
    
    Return:
	Returns a pointer to the scalar in the environment 
	represented by the string key. If a scalar with the 
	specified key was not found then NULL will be returned.
	
	The scalar returned can be used as if you were directly
	addressing the bucket entry, no need to use the a "put"
	function afterwards.
	
    Errors:
	EE_NULL_ARGUMENT -
	If any of the arguments are NULL
*/
SCALAR *get_from_env(ENVIRONMENT *env, char *key)
{
    int pos;

    /* Make sure the arguments are not NULL */
    if (env == NULL || key == NULL)
    {
	env_error = EE_NULL_ARGUMENT;
	return NULL;
    }
        
    /* Determine the bucket they value is in */
    pos = hash(key);


    /* If the bucket is empty simply return NULL */
    if (env->entries[pos] == NULL)
	return NULL;
    
    //printf("HASH ENTRY at position %d: %X\n", pos, env->entries[pos]);
    
    /* Search the bucket due to possible conflicts */
    BUCKET_ENTRY *entry = env->entries[pos];
	
    while (entry != NULL)
    {
	if (!strcmp(key, entry->key))
	    return entry->value;
	
	if (entry->next == NULL)
	    return NULL;
	    
	entry = entry->next;
    }
    
    env_error = SE_NONE;
    return NULL;
}


/**
    remove_from_env() -
    
    Description:
	Removes a value from the environemnt via a
	string key. This function will not remove
	values from environments that are of lower
	precidence than the environment specified.
    
    Parameters:
	ENVIRONMENT *env - environment to remove the
	    scalar value from.
	
	char *key - Key for the scalar value.
    
    Errors:
	EE_NULL_ARGUMENT -
	If any of the arguments are NULL.
	
	EE_SCALAR_NOT_FOUND -
	If a scalar with the specified key could
	not be located in the environment.
*/
void remove_from_env(ENVIRONMENT *env, char *key)
{
    int pos;
    BUCKET_ENTRY *bucket = NULL;
    
    /* Make sure the arguments are not null */
    if (env == NULL || key == NULL)
    {
	env_error = EE_NULL_ARGUMENT;
	return;
    }    
    
    /* Find the bucket position */
    pos = hash(key);
    
    /* If the bucket is empty error out */
    if (env->entries[pos] == NULL)
    {
	env_error = EE_SCALAR_NOT_FOUND;
	return;
    }
    
    /* Make sure we are removing the right entry */
    if (!strcmp(key, env->entries[pos]->key))
	bucket = env->entries[pos];
	
    /* Search the bucket due to possible conflicts */
    if (env->entries[pos]->next != NULL && bucket == NULL)
    {
	BUCKET_ENTRY *entry = env->entries[pos];
	
	do
	{
	    if (!strcmp(key, entry->key))
	    {
		bucket = entry;
		break;
	    }
	}
	while (entry = entry->next);
    }
    
    /* More figgin error checking */
    if (bucket == NULL) 
    {
	env_error = EE_SCALAR_NOT_FOUND;
	return;
    }
    
    /* Make sure the list stays consistent */
    BUCKET_ENTRY *next = bucket->next;
    BUCKET_ENTRY *prev = bucket->previous;

    if (prev != NULL)
	prev->next = next;

    if (next != NULL)
	next->previous = prev;
    
    env_error = SE_NONE;
    return;
}

/**
    set_lower() -
    
    Description:
	Sets an environment's lower precidence
	field.
    
    Parameters:
	a - environment to set
	b - environment to set to
	
    Errors:
	EE_NULL_ARGUMENT -
	If the argument a is NULL.
*/
void set_lower(ENVIRONMENT *a, ENVIRONMENT *b)
{
    if (a == NULL)
    {
	env_error = EE_NULL_ARGUMENT;
	return;
    }

    a->lower = b;
}


/**
    set_higher() -
    
    Description:
	Sets an environment's higher precidence
	field.
    
    Parameters:
	a - environment to set
	b - environment to set to
	
    Errors:
	EE_NULL_ARGUMENT -
	If the argument a is NULL
*/
void set_higher(ENVIRONMENT *a, ENVIRONMENT *b)
{
    if (a == NULL)
    {
	env_error = EE_NULL_ARGUMENT;
	return;
    }
    
    a->higher = b;
    env_error = SE_NONE;
}


/**
    get_highest() -
    
    Description:
	Determines the location of the highest level
	scalar with the specified key.
	
    Parameters:
	top - first environment to search in and down
	    from.
	
	key - string key for the scalar.
    
    Returns:
	The scalar at the specified position. 
	
	If an error occurs, the function returns NULL.
	
	The scalar returned can be used as if you were 
	directly addressing the bucket entry, no need 
	to use the a "put" function afterwards.
	
    Errors:
	EE_NULL_ARGUMENT -
	If either of the parameters is null.
*/
SCALAR *get_highest(ENVIRONMENT *top, char *key)
{
    if (top == NULL || key == NULL)
    {
	env_error = EE_NULL_ARGUMENT;
	return NULL;
    }
    
    //printf("Get Highest, key: %s\n", key);
    
    ENVIRONMENT *current = top;
    SCALAR *s = NULL;
    
    while (s == NULL && current != NULL)
    {
	s = get_from_env(current, key);
	
	if (s != NULL) break;
	
	current = current->lower;
    }
    
    //printf("Scalar found, type: ");
    //display_scalar_type(s);
    //printf("\n");
    
    env_error = SE_NONE;
    return s;
}


/**
    remove_from_first() -
    
    Description:
	Attempts to remove the scalar from the environment
	supplied via the string key. If the scalar could
	not be found, it then searches every lower precidence
	environment for a scalar with the same key and
	if found removes it. 
	
    Parameters:
	ENVIRONMENT *env - top level environment to remove the
	    scalar value from.
	
	char *key - Key for the scalar value.
    
    Errors:
	EE_NULL_ARGUMENT -
	If any of the arguments are NULL.
	
	EE_SCALAR_NOT_FOUND -
	If a scalar with the specified key could
	not be located in the environment.
*/
void remove_highest(ENVIRONMENT *top, char *key)
{
    if (top == NULL || key == NULL)
    {
	env_error = EE_NULL_ARGUMENT;
	return;
    }
    
    ENVIRONMENT *current = top;
    int found = 0;
    
    while (!found && current != NULL)
    {
	remove_from_env(current, key);
	
	if (env_error == EE_NONE)
	    return;
	    
	current = current->lower;
    }
    
    env_error = EE_SCALAR_NOT_FOUND;
    return;
}










