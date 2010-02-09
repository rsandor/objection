/*
    FILE:	lexer.l
    PROJECT:	objection stage 2
    AUTHOR:	Ryan Richards
    
    This file describes a lexical analyzer for objection
    stage 1.
*/

/* Initial C definitions */
%{
    #include "objection.h"
    #include "scalar.h"
    #include "parse_node.h"
    #include "parser.tab.h"
    
    
    // Macro to compress some of the one liners
    #define IVAL(T) yylval.ival = line_count; \
		    return (T)
    
    long line_count = 1;
    long errors = 0;
%}

ESCAPE_SEQUENCE	    "\\".

%%

\&	    { IVAL(LIST_INTERSECTION); }
\|	    { IVAL(LIST_UNION); }
\\	    { IVAL(LIST_COMPLEMENT); }
\^	    { IVAL(LIST_BICOMPLEMENT); }
\?	    { IVAL(LIST_MEMBER); }
\:	    { IVAL(LIST_SUBSET); }
\@	    { IVAL(LIST_APPEND); }
\#	    { IVAL(LIST_CONS); }
\`	    { IVAL(LIST_CAR); }
\~	    { IVAL(LIST_CDR); }
\%	    { IVAL(ARITH_MOD); }
\+	    { IVAL(ARITH_ADD); }
\-	    { IVAL(ARITH_SUB); }
\*	    { IVAL(ARITH_MUL); }
\/	    { IVAL(ARITH_DIV); }
"<"	    { IVAL(BOOLEAN_LT); }
">"	    { IVAL(BOOLEAN_GT); }
"<="	    { IVAL(BOOLEAN_LTE); }
">="	    { IVAL(BOOLEAN_GTE); }
"eqv"	    { IVAL(BOOLEAN_EQV); }
"=="	    { IVAL(BOOLEAN_EQ); }
"!="	    { IVAL(BOOLEAN_NEQ); }
"&&"	    { IVAL(BOOLEAN_AND); }
"||"	    { IVAL(BOOLEAN_OR); }
"xor"	    { IVAL(BOOLEAN_XOR); }
"!"	    { IVAL(BOOLEAN_NOT); }
"->"	    { IVAL(BOOLEAN_IMP); }
"<->"	    { IVAL(BOOLEAN_DIMP); }
\{	    { IVAL(OPEN_BRACE); }
\}	    { IVAL(CLOSE_BRACE); }
\(	    { IVAL(OPEN_PAREN); }
\)	    { IVAL(CLOSE_PAREN); }
\[	    { IVAL(OPEN_BRACKET); }
\]	    { IVAL(CLOSE_BRACKET); }
\,	    { IVAL(COMMA); }
"on"	    { IVAL(ON); }
"recur"	    { IVAL(RECUR); }
"rec"	    { IVAL(REC); }
"define"    { IVAL(DEFINE); }
"as"	    { IVAL(AS); }
"function"  { IVAL(FUNCTION); }
"="	    { IVAL(ASSIGN); }
";"	    { IVAL(SEMICOLON); }
"if"	    { IVAL(IF); }
"let"	    { IVAL(LET); }
"for"	    { IVAL(FOR); }
"lambda"    { IVAL(LAMBDA); }
"cond"	    { IVAL(COND); }
"case"	    { IVAL(CASE); }
"else"	    { IVAL(ELSE); }
"default"   { IVAL(DEFAULT); }
"list"	    { IVAL(LIST); }
"error"	    { IVAL(ERROR); }

\$[a-zA-Z0-9_]+   {
			    /* 
				Handle a symbol 
				This is ALL SUBJECT TO CHANGE
				Considering using '$' like an
				operator, with some fancy lexing
				stuff toboot!
				
				All for later alas
			    */
			    
			    yytext++;
			    
			    PARSE_NODE *t = new_parse_node();
			    t->line = line_count;
			    set_flag(t, FLAG_SYMBOL);
			    set_string_value(t->value, yytext, yyleng-1);
			    t->value->type = ST_SYMBOL;
			    yylval.nval = t;
			    
			    return SYMBOL;
			}

"'"({ESCAPE_SEQUENCE}|.)"'"	{
				    //printf("CHAR FOUND\n");
				    // Handle a character
				    PARSE_NODE *tmp;
				    tmp = new_parse_node();
				    
				    tmp->line = line_count;
				    			    
				    set_char_value(tmp->value, yytext[1]);
				    set_flag(tmp, FLAG_CHARACTER);
				    
				    yylval.nval = tmp;
				    return CHARACTER;
				}


"\""({ESCAPE_SEQUENCE}|[^"\""]|[^"\\"])*"\"" {
				    /* Handle a string */
				    PARSE_NODE *tmp;
				    tmp = new_parse_node();
				    
				    tmp->line = line_count;
				    
				    /* Get rid of the "s */
				    yytext[yyleng-1] = 0;
				    yytext++;
				    				    
				    set_string_value(tmp->value, yytext, yyleng);
				    set_flag(tmp, FLAG_STRING);
				    yylval.nval = tmp;
				    
				    return STRING;
				}


"-"?[0-9]+			{
				    /* Handle an integer */
				    PARSE_NODE *tmp = new_parse_node();
				    tmp->line = line_count;
				    set_long_value(tmp->value, strtol(yytext, NULL, 10));
				    set_flag(tmp, FLAG_INTEGER);
				    yylval.nval = tmp;
				    return INTEGER;
				}


"-"?[0-9]+\.[0-9]+		{
				    /* Handle a double */
				    PARSE_NODE *tmp = new_parse_node();
				    tmp->line = line_count;
				    set_double_value(tmp->value, strtod(yytext, NULL));
				    set_flag(tmp, FLAG_DOUBLE);
				    yylval.nval = tmp;
				    return DOUBLE;
				}

"true"|"false"	{ 
		    /* Handle a boolean */
		    PARSE_NODE *tmp = new_parse_node();
		    tmp->line = line_count;
		    set_bool_value(tmp->value, !strcmp(yytext, "true") ? 1 : 0);
		    set_flag(tmp, FLAG_BOOLEAN);
		    yylval.nval = tmp;
		    return BOOLEAN; 
		}
	

[a-zA-Z_][a-zA-Z0-9_]*	    {
			/* Handle an identifier */
			PARSE_NODE *tmp = new_parse_node();
			tmp->line = line_count;
			set_string_value(tmp->value, yytext, yyleng);
			set_flag(tmp, FLAG_ID);
			yylval.nval = tmp;
			return ID;
		    }
			
"//".*\n	    {
			line_count++;
		    }

[ \t]		    /* Eat whitespace */

\n		    {
			line_count++;
		    }

.		    {
			char buf[256];
			sprintf(buf, "Unrecognized character %s", yytext);
			yyerror(buf);
		    }

%%

// For errors in thine syntax
int yyerror(const char *s)
{
    printf("Syntax Error near line %d:\n\t%s\n", line_count, yytext);
    errors++;
    return 1;
}
