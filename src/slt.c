
#include <stdio.h>
#include <stdlib.h>
#include "scalar.h"

int test_number = 0;
int errors = 0;
#define divider() printf("--------------------------------\n");

void pr(char *t, char *e, char *g)
{
    printf("Test #%d: %s\n", ++test_number, t);
    printf("\tExpected: %s\n", e);
    printf("\tGot:      %s\n", g);
    printf("\tResult:   %s\n", !strcmp(e, g) ? "OKAY" : "ERROR");
    
    errors += !strcmp(e, g) ? 0 : 1;
    
    divider();
}

void pr_dif(char *t, char *e, char *g)
{
    printf("Test #%d: %s\n", ++test_number, t);
    printf("\tNot Expected: %s\n", e);
    printf("\tGot:          %s\n", g);
    printf("\tResult:   %s\n", strcmp(e, g) ? "OKAY" : "ERROR");
    errors += strcmp(e, g) ? 0 : 1;
    divider();
}

int main(int argc, char **argv)
{
    SCALAR *s1, *s2, *s3, *s4, *s5, *s6;
    SCALAR_LIST *slst, *slst2, *slst3, *slst4;
    char buf[256], buf2[256], buf3[256], buf4[256];
 
    
    printf("Scalar Lists Test Program\n");
    divider();
    
    // 1) empty_list() macro
    slst = new_scalar_list();
    sprintf(buf, "%s", empty_list(slst) ? "true" : "false");
    pr("empty_list() macro", "true", buf);
    
    // 2) empty_list() macro
    append(slst, new_scalar());
    sprintf(buf, "%s", empty_list(slst) ? "true" : "false");
    pr("empty_list() macro", "false", buf);
    
    // 3) append()
    slst = new_scalar_list();
    s1 = new_scalar();
    s2 = new_scalar();
    s3 = new_scalar();
    set_long_value(s1, 1);
    set_long_value(s2, 2);
    set_long_value(s3, 3);
    append(slst, s1);
    append(slst, s2);
    append(slst, s3);
    
    sprintf(buf, "HEAD, 1, 2, 3, FOOT");
    sprintf(buf2, "%s, %s, %s, %s, %s",
	is_head(slst) ? "HEAD" : "XXXX", 
	is_data(slst->next) ? string_value(sc_value(slst->next)) : "X",
	is_data(slst->next->next) ? string_value(sc_value(slst->next->next)) : "X",
	is_data(slst->next->next->next) ? string_value(sc_value(slst->next->next->next)) : "X",
	is_foot(slst->next->next->next->next) ? "FOOT" : "XXXX" 
	);
    
    pr("append()", buf, buf2);
    
    
    // 4) set_list_value()
    s4 = new_scalar();
    set_list_value(s4, slst);
    
    sprintf(buf,  "ADDR: %X\tTYPE: %d", slst, ST_LIST);
    sprintf(buf2, "ADDR: %X\tTYPE: %d", s4->value, sc_type(s4));
    pr("set_list_value()", buf, buf2);
    
    // 5) copy_list()
    slst2 = copy_list(slst);
    sprintf(buf, "%X", slst);
    sprintf(buf2, "%X", slst2);
    pr_dif("copy_list()", buf, buf2);
    
    
    // 6) copy_list()
    sprintf(buf, "%s, %s, %s, %s, %s",
	is_head(slst) ? "HEAD" : "XXXX", 
	is_data(slst->next) ? string_value(sc_value(slst->next)) : "X",
	is_data(slst->next->next) ? string_value(sc_value(slst->next->next)) : "X",
	is_data(slst->next->next->next) ? string_value(sc_value(slst->next->next->next)) : "X",
	is_foot(slst->next->next->next->next) ? "FOOT" : "XXXX" 
	);
    
    sprintf(buf2, "%s, %s, %s, %s, %s",
	is_head(slst2) ? "HEAD" : "XXXX", 
	is_data(slst2->next) ? string_value(sc_value(slst2->next)) : "X",
	is_data(slst2->next->next) ? string_value(sc_value(slst2->next->next)) : "X",
	is_data(slst2->next->next->next) ? string_value(sc_value(slst2->next->next->next)) : "X",
	is_foot(slst2->next->next->next->next) ? "FOOT" : "XXXX" 
	);
    
    pr("copy_list()", buf, buf2);
    
    
    // 7) string_value()
    sprintf(buf, "(1 2 3)");
    sprintf(buf2, string_value(s4));
    pr("string_value()", buf, buf2);
    
    // 8) string_value()
    slst2 = new_scalar_list();
    slst3 = new_scalar_list();
    s2 = new_scalar();
    s4 = new_scalar();
    s5 = new_scalar();
    s6 = new_scalar();
    set_list_value(s2, slst2);
    set_list_value(s3, slst3);
    set_symbol_value(s4, "poopie");
    set_string_value(s5, "poopie", strlen("poopie"));
    set_bool_value(s6, 1);
    
    // So yeah, this doesn't look like lisp :-P
    s2 = new_scalar();
    slst4 = new_scalar_list();
    set_list_value(s2, slst4);
    append(append(append(append(slst3, s4), s5), s6), s2);
    set_list_value(s3, slst3);
    
    SCALAR_LIST *ols = new_scalar_list();
    SCALAR *one = new_scalar();
    SCALAR *two = new_scalar();
    SCALAR *thr = new_scalar();
    
    set_long_value(one, 1);
    set_double_value(two, 2.4);
    set_symbol_value(thr, "apples");
    
    append(append(append(append(ols, one), two), thr), s3);
    
    SCALAR *tlst = new_scalar();
    set_list_value(tlst, ols);
    
    sprintf(buf, "(1 2.4 apples (poopie \"poopie\" true ()))");
    sprintf(buf2, string_value(tlst));
    pr("string_value()", buf, buf2);
    
    // sl_length()
    sprintf(buf,  "4");
    sprintf(buf2, "%d", sl_length(ols));
    pr("length()", buf, buf2);
         
	 
    // get_from_list()
    
    int x;
    for (x = -1; x < 5; x++)
    {
	SCALAR *sval = get_from_list(ols, x);
	
	if (x == -1)	    sprintf(buf, "NULL");
	if (x == 0)	    sprintf(buf, "1");
	else if (x == 1)    sprintf(buf, "2.4");
	else if (x == 2)    sprintf(buf, "apples");
	else if (x == 3)    sprintf(buf, "(poopie \"poopie\" true ())");
	else		    sprintf(buf, "NULL");
	
	if (sval != NULL)
	    sprintf(buf2, string_value(sval));
	else
	    sprintf(buf2, "NULL");
	
	pr("get_from_list()", buf, buf2);
    }
	 
    printf("Completed Testing, there were %d error%s\n", errors, (errors > 1) ? "" : "s");
    
    
    
    return 0;
}



