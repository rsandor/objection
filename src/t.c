
#include <stdio.h>
#include "scalar.h"
#include "parse_node.h"
#include "envhash.h"

#define MAXLEN 11



int main(int argc, char **argv)
{
	int i;	
	char *names[MAXLEN] = {
		"char",	"short", "int", "long", "float","double",
		"SCALAR", "SCALAR_LIST", "PARSE_NODE",
	       	"BUCKET_ENTRY", "ENVIRONMENT"
	};
	
	int sizes[MAXLEN] = {
		sizeof(char),
		sizeof(short),
		sizeof(int),
		sizeof(long),
		sizeof(float),
		sizeof(double),
		sizeof(SCALAR),
		sizeof(SCALAR_LIST),
		sizeof(PARSE_NODE),
		sizeof(BUCKET_ENTRY),
		sizeof(ENVIRONMENT)
	};

	printf("%-12s %s\n", "Type", "Size (bytes)");
	for(i=0; i<MAXLEN;i++) printf("%-12s %d\n", names[i], sizes[i]);
}

