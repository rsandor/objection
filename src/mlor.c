
/*
    File: mlor.c
    Project: Objection Stage 2 
    Author: Ryan Sandor Richards
    
    Copyright  Ryan Sandor Richards, All rights reserved.
    
    THis is the implementation of functions for the Medium 
    Level Objection Representation. See mlor.h for more
    details about the MLOR.
*/

// Include the MLOR header file
#include "mlor.h"

// Creates a "register" scalar of the appropriate register number
SCALAR *reg_scalar(int r)
{
    SCALAR *sc = new_scalar();
    set_long_value(sc, r);
    sc->type = ST_REGISTER;
    return sc;
}


// Creates a new MLOR block
MLOR_BLOCK *new_block()
{
    MLOR_BLOCK *block = (MLOR_BLOCK *)malloc( sizeof(MLOR_BLOCK) );
    block->first = NULL;
    block->last = NULL;
    block->label = 1;
    return block;
}


// Creates a new MLOR instruction and returns a pointer to it
MLOR_INST *new_instruction(short op, long line, SCALAR *operands)
{
    MLOR_INST *i = (MLOR_INST *)GC_MALLOC( sizeof(MLOR_INST) );
    i->op = op;
    i->line = line;
    i->operands = operands;
    i->last_operand = operands;
    return i;
}


/*
    Translates the functions tree and the definitions tree
    into a sequence of MLOR instructions.
    
    Returns the first MLOR instruction to be interpreted.
*/
MLOR_INST *translate(PARSE_NODE *funcs, PARSE_NODE *defs)
{
    MLOR_BLOCK *program = NULL;
    MLOR_BLOCK *functions_block = NULL;
    MLOR_BLOCK *tmpfunc = NULL;
    PARSE_NODE *tmp_pn = NULL;
    
    // First order of business is to get the functions
    // created in MLOR instruction representations
   
    
    // Now that we have the functions, get the definitions
    // as MLOR representations
    
    
    // Finally we can string the entire bunch together to
    // have a final form of the program in MLOR
    
    
    return NULL;
}

// Translates a function to MLOR
SCALAR *translate_node(PARSE_NODE *n, MLOR_BLOCK *current_block, int next_reg)
{
    PARSE_NODE *tmp_pn = NULL;
    PARSE_NODE *tmp_pn2 = NULL;
    
    SCALAR *tmp_sc = NULL;
    SCALAR *tmp_sc2 = NULL;
    
    MLOR_INST *tmp_inst = NULL;
    MLOR_INST *tmp_inst2 = NULL;
    MLOR_INST *tmp_inst3 = NULL;

    if(flag_set(n, FLAG_BLOCK))
    {
	for (tmp_pn = n->first_child; tmp_pn; tmp_pn = tmp_pn->next_sibling)
	    if (tmp_pn->next_sibling == NULL)
		return translate_node(tmp_pn, current_block, next_reg);
	    else
		translate_node(tmp_pn, current_block, next_reg);
    }
    else if(flag_set(n, FLAG_LET)) 
    {
	// Add the scoping instruction
	tmp_inst = new_instruction(MLOR_OP_SCOPE, n->line, NULL);
	add_inst(current_block, tmp_inst);
	
	// Go through and make the assignment instructions
	for (tmp_pn = n->args; tmp_pn; tmp_pn = tmp_pn->next_sibling)
	{
	    tmp_sc = translate_node(
			tmp_pn->first_child, 
			current_block, 
			next_reg
			);
		    
	    tmp_inst = new_instruction(
			MLOR_OP_BIND, 
			tmp_pn->line, 
			tmp_pn->args->value
			);
	    
	    add_operand(tmp_inst, tmp_sc);
	    
	    add_inst(current_block, tmp_inst);
	}
	
	// Translate the actual code section
	for (tmp_pn = n->first_child; tmp_pn; tmp_pn = tmp_pn->next_sibling)
	{
	    tmp_sc = translate_node(
			tmp_pn,
			current_block,
			next_reg
			);
	}
	
	// Add the end of scoping block area
	tmp_inst = new_instruction(MLOR_OP_ENDSCOPE, n->line, NULL);
	add_inst(current_block, tmp_inst);
	
	// Return the last value found by the body
	return tmp_sc;
    }
    else if(flag_set(n, FLAG_IF)) 
    {
	/*
	    Dealing with if statements:
	    (see notes)
	*/
	
	// Condition block
	
	tmp_sc = translate_node(n->args, current_block, next_reg);
	
	tmp_inst = new_instruction(
	    MLOR_OP_BRANCH, 
	    n->line, 
	    tmp_sc
	);
	
	
	// l1 holds the label address for the "else" label
	int l1 = current_block->label++;
	tmp_sc = new_scalar();
	set_long_value(tmp_sc, l1);
	add_operand(tmp_inst, tmp_sc);
	
	add_inst(current_block, tmp_inst);
	
	// Then block
	translate_node(n->cond_true, current_block, next_reg);
	
	int l2 = current_block->label++;
	tmp_sc2 = new_scalar();
	set_long_value(tmp_sc2, l2);
	
	tmp_inst = new_instruction(
	    MLOR_OP_GOTO,
	    n->line,
	    tmp_sc2
	);
	
	add_inst(current_block, tmp_inst);
	
	// Else block	
	tmp_inst = new_instruction(
	    MLOR_OP_NOP,
	    n->line,
	    NULL
	);
	tmp_inst->label = l1;
	add_inst(current_block, tmp_inst);
	
	translate_node(n->cond_false, current_block, next_reg);
	
	tmp_inst = new_instruction(
	    MLOR_OP_NOP,
	    n->line,
	    NULL
	);
	tmp_inst->label = l2;
	add_inst(current_block, tmp_inst);
	
	return reg_scalar(next_reg);
    }
    else if(flag_set(n, FLAG_COND)) 
    {
	// Cond is not unlike an if statement, but we need more
	// labels to correctly create it!
	// Children - conditions
	
	// For conditions
	// args - condition
	// child - form
	
	
	for(tmp_pn = n->first_child; tmp_pn; tmp_pn = tmp_pn->next_sibling)
	{
	    if (tmp_pn->args == NULL)
	    {
		// Default node, make sure there is only 1
		if (tmp_pn->args->next_sibling)
		{
		    // Error later...
		    // need to work it out if I am going to do
		    // semantic analysis here or somewhere else
		}
		
		
	    }
	}
    }
    else if(flag_set(n, FLAG_APPLICATION)) 
    {
	// app_expression -> what to apply
	// children -> values to apply to
	
	// First we need to know what we are applying
	tmp_inst = new_instruction(MLOR_OP_CALL, n->line, n->app_expression->value);
	
	// And what values to supply
	int i = next_reg;
	
	for (tmp_pn = n->first_child; tmp_pn; tmp_pn = tmp_pn->next_sibling)
	{
	    tmp_sc2 = translate_node(tmp_pn, current_block, i);
	    add_operand(tmp_inst, tmp_sc2);
	    
	    // If we had a register, simply increment
	    if (tmp_sc2->type == ST_REGISTER)
		i++;
	}
	
	set_reg(tmp_inst, next_reg);
	
	add_inst(current_block, tmp_inst);
	
	return reg_scalar(next_reg);
    }
    else if(flag_set(n, FLAG_LIST)) 
    {
    }
    else if(flag_set(n, FLAG_INTEGER) || flag_set(n, FLAG_DOUBLE) ||
	    flag_set(n, FLAG_STRING)  || flag_set(n, FLAG_SYMBOL) ||
	    flag_set(n, FLAG_BOOLEAN) || flag_set(n, FLAG_CHARACTER)) 
    {
	tmp_inst = new_instruction(MLOR_OP_LI, n->line, n->value);
	set_reg(tmp_inst, next_reg);
	add_inst(current_block, tmp_inst);
	return reg_scalar(next_reg);
    }
    else if(flag_set(n, FLAG_ID)) 
    {
	tmp_inst = new_instruction(MLOR_OP_ID, n->line, n->value);
	set_reg(tmp_inst, next_reg);
	add_inst(current_block, tmp_inst);
	
	return reg_scalar(next_reg);
    }
    else if(flag_set(n, FLAG_ERROR)) 
    {
	tmp_sc = translate_node(
	    n->args,
	    current_block,
	    next_reg
	    );
	
	tmp_inst = new_instruction(MLOR_OP_BRANCH, n->line, tmp_sc);
	
	int l1 = current_block->label++;
	tmp_sc = new_scalar();
	set_long_value(tmp_sc, l1);
	add_operand(tmp_inst, tmp_sc);	
	
	add_inst(current_block, tmp_inst);
	
	tmp_sc2 = translate_node(
	    n->first_child,
	    current_block,
	    next_reg
	    );
	
	tmp_inst = new_instruction(
	    MLOR_OP_TERM,
	    n->line,
	    NULL
	    );
	
	add_inst(current_block, tmp_inst);
	
	tmp_inst = new_instruction(
	    MLOR_OP_NOP,
	    n->line,
	    NULL
	    );
	    
	tmp_inst->label = l1;
	add_inst(current_block, tmp_inst);
	
	return reg_scalar(next_reg);
    }
    else if(flag_set(n, FLAG_BOOLEAN_OP)) 
    {
	tmp_sc = translate_node(
	    n->first_child, 
	    current_block, 
	    next_reg
	    );
	    
	tmp_sc2 = translate_node(
	    n->first_child->next_sibling, 
	    current_block, 
	    (tmp_sc->type == ST_REGISTER) ? (next_reg + 1) : next_reg 
	    );
	    
	int myop;
	
	switch (n->op)
	{
	    case BOOLEAN_OP_AND:    myop = MLOR_OP_AND;	    break;
	    case BOOLEAN_OP_OR:	    myop = MLOR_OP_OR;	    break;
	    case BOOLEAN_OP_NOT:    myop = MLOR_OP_NOT;	    break;
	    case BOOLEAN_OP_XOR:    myop = MLOR_OP_XOR;	    break;
	    case BOOLEAN_OP_IMP:    myop = MLOR_OP_IMP;	    break;
	    case BOOLEAN_OP_DIMP:   myop = MLOR_OP_DIMP;    break;
	    case BOOLEAN_OP_EQ:	    myop = MLOR_OP_EQ;	    break;
	    case BOOLEAN_OP_NEQ:    myop = MLOR_OP_NEQ;	    break;
	    case BOOLEAN_OP_LT:	    myop = MLOR_OP_LT;	    break;
	    case BOOLEAN_OP_GT:	    myop = MLOR_OP_GT;	    break;
	    case BOOLEAN_OP_LTE:    myop = MLOR_OP_LTE;	    break;
	    case BOOLEAN_OP_GTE:    myop = MLOR_OP_GTE;	    break;
	    case BOOLEAN_OP_EQV:    myop = MLOR_OP_EQV;	    break;
	}
	
	tmp_inst = new_instruction(
		    myop, 
		    n->line, 
		    tmp_sc
		    );
	
	tmp_inst->operands->next_op = tmp_sc2;
	tmp_inst->reg = next_reg;
	add_inst(current_block, tmp_inst);
	
	return reg_scalar(next_reg);
    }
    else if(flag_set(n, FLAG_ARITH_OP)) 
    {
	tmp_sc = translate_node(
	    n->first_child, 
	    current_block, 
	    next_reg
	    );
	    
	tmp_sc2 = translate_node(
	    n->first_child->next_sibling, 
	    current_block, 
	    (tmp_sc->type == ST_REGISTER) ? (next_reg + 1) : next_reg 
	    );
	
	int myop;
	
	switch (n->op)
	{
	    case ARITH_OP_ADD:	myop = MLOR_OP_ADD; break;
	    case ARITH_OP_SUB:	myop = MLOR_OP_SUB; break;
	    case ARITH_OP_MUL:	myop = MLOR_OP_MUL; break;
	    case ARITH_OP_DIV:	myop = MLOR_OP_DIV; break;
	    case ARITH_OP_MOD:	myop = MLOR_OP_MOD; break;
	}
	
	tmp_inst = new_instruction(
		    myop, 
		    n->line, 
		    tmp_sc
		    );
	
	tmp_inst->operands->next_op = tmp_sc2;
	tmp_inst->reg = next_reg;
	add_inst(current_block, tmp_inst);
	
	return reg_scalar(next_reg);
    }
    else if(flag_set(n, FLAG_LIST_OP)) 
    {
	int myop;
	switch(n->op)
	{
	    case LIST_OP_APPEND:	myop = MLOR_OP_APPEND;		break;
	    case LIST_OP_CONS:		myop = MLOR_OP_CONS;		break;
	    case LIST_OP_CAR:		myop = MLOR_OP_CAR;		break;
	    case LIST_OP_CDR:		myop = MLOR_OP_CDR;		break;
	    case LIST_OP_INTERSECTION:	myop = MLOR_OP_INTERSECTION;	break;
	    case LIST_OP_UNION:		myop = MLOR_OP_UNION;		break;
	    case LIST_OP_COMPLEMENT:	myop = MLOR_OP_COMPLEMENT;	break;
	    case LIST_OP_BICOMPLEMENT:	myop = MLOR_OP_BICOMPLEMENT;	break;
	    case LIST_OP_MEMBER:	myop = MLOR_OP_MEMBER;		break;
	    case LIST_OP_SUBSET:	myop = MLOR_OP_SUBSET;		break;
	    case LIST_OP_ELEMENT_AT:	myop = MLOR_OP_ELEMENT_AT;	break;
	}
	
	tmp_sc = translate_node(
	    n->first_child, 
	    current_block, 
	    next_reg
	    );
	
	// The car/cdr connection
	if (myop == MLOR_OP_CAR || myop == MLOR_OP_CDR)
	{
	    tmp_inst = new_instruction(
		myop,
		n->line,
		tmp_sc
		);
	    tmp_inst->reg = next_reg;
	    add_inst(current_block, tmp_inst);
	}
	else
	{
	    tmp_sc2 = translate_node(
		n->first_child->next_sibling, 
		current_block, 
		(tmp_sc->type == ST_REGISTER) ? (next_reg + 1) : next_reg 
		);
	    
	    tmp_inst = new_instruction(
		myop,
		n->line,
		tmp_sc
		);
	    
	    add_operand(tmp_inst, tmp_sc2);
	    tmp_inst->reg = next_reg;
	    add_inst(current_block, tmp_inst);
	}
	
	return reg_scalar(next_reg);
    }
    
    return NULL;
}

#define opstr(O, B)	if (O->type == ST_STRING) \
			    sprintf(B, "\"%s\"", string_value(O)); \
			else \
			    sprintf(B, "%s", string_value(O));
			    
// Displays MLOR in a human readble format
void print_mlor(MLOR_INST *first)
{
    char opr[256];
    char buf[256];
    char buf2[256];
    
    while (first)
    {
	if (first->label)
	    printf("L%d:\n", first->label);
	
	if (first->op != MLOR_OP_NOP)
	    printf("\t");
	    
	if (first->op == MLOR_OP_SCOPE) 
	{
	    printf("%-6s\n", "SCOPE");
	}
	else if (first->op == MLOR_OP_ENDSCOPE) 
	{
	    printf("%-6s\n", "ENDSCO");
	}
	else if (first->op == MLOR_OP_BIND) 
	{
	    opstr(first->operands, buf);
	    opstr(first->operands->next_op, buf2);
	    printf("%-6s %s, %s\n", "BIND", buf, buf2);
	}
	else if (first->op == MLOR_OP_RETURN) 
	{
	    opstr(first->operands, buf);
	    printf("%-6s %s\n", "RETURN", buf);
	}
	else if (first->op == MLOR_OP_GOTO) 
	{
	    opstr(first->operands, buf);
	    printf("%-6s L%s\n", "GOTO", buf);
	}
	else if (first->op == MLOR_OP_TERM) 
	{
	    printf("%-6s\n", "TERM");
	}
	else if (first->op == MLOR_OP_BRANCH) 
	{
	    opstr(first->operands, buf);
	    opstr(first->operands->next_op, buf2);
	    printf("%-6s %s, L%s\n", "BRT", buf, buf2);
	}
	else if (first->op == MLOR_OP_CALL) 
	{
	    printf("%-6s ", "CALL");
	    
	    SCALAR *tmp;
	    for (tmp = first->operands; tmp; tmp = tmp->next_op)
	    {
		printf("%s", string_value(tmp));
		
		if (tmp->next_op)
		    printf(", ");
	    }
	    
	    printf(" -> $%d\n", first->reg);
	}
	else if (first->op == MLOR_OP_LIST) {}
	else if (first->op == MLOR_OP_ID) 
	{
	    opstr(first->operands, buf);
	    printf("%-6s %s -> $%d\n", "ID", buf, first->reg);
	}
	else if (first->op == MLOR_OP_ADD || first->op == MLOR_OP_SUB ||
		 first->op == MLOR_OP_MUL || first->op == MLOR_OP_DIV ||
		 first->op == MLOR_OP_MOD) 
	{
	    switch (first->op)
	    {
		case MLOR_OP_ADD: sprintf(opr, "ADD"); break;
		case MLOR_OP_SUB: sprintf(opr, "SUB"); break;
		case MLOR_OP_MUL: sprintf(opr, "MUL"); break;
		case MLOR_OP_DIV: sprintf(opr, "DIV"); break;
		case MLOR_OP_MOD: sprintf(opr, "MOD"); break;
	    }
	    
	    opstr(first->operands, buf);
	    opstr(first->operands->next_op, buf2);
	    
	    printf("%-6s %s, %s -> $%d\n", opr, buf, buf2, first->reg);
	}
	else if (first->op == MLOR_OP_AND || first->op == MLOR_OP_OR   ||
		 first->op == MLOR_OP_NOT || first->op == MLOR_OP_XOR  ||
		 first->op == MLOR_OP_IMP || first->op == MLOR_OP_DIMP ||
		 first->op == MLOR_OP_EQ  || first->op == MLOR_OP_NEQ  ||
		 first->op == MLOR_OP_LT  || first->op == MLOR_OP_GT   ||
		 first->op == MLOR_OP_LTE || first->op == MLOR_OP_GTE  ||
		 first->op == MLOR_OP_EQV )
	{
	    switch (first->op)
	    {
		case MLOR_OP_AND:    sprintf(opr, "AND"); break;
		case MLOR_OP_OR:	sprintf(opr, "OR"); break;
		case MLOR_OP_XOR:    sprintf(opr, "XOR"); break;
		case MLOR_OP_IMP:    sprintf(opr, "IMP"); break;
		case MLOR_OP_DIMP:   sprintf(opr, "DIMP"); break;
		case MLOR_OP_EQ:	sprintf(opr, "EQ"); break;
		case MLOR_OP_NEQ:    sprintf(opr, "NEQ"); break;
		case MLOR_OP_LT:	sprintf(opr, "LT"); break;
		case MLOR_OP_GT:	sprintf(opr, "GT"); break;
		case MLOR_OP_LTE:    sprintf(opr, "LTE"); break;
		case MLOR_OP_GTE:    sprintf(opr, "GTE"); break;
		case MLOR_OP_EQV:    sprintf(opr, "EQV"); break;
	    }
	    
	    if (first->op == MLOR_OP_NOT)
	    {
		opstr(first->operands, buf);
		printf("%-6s %s -> $%d", "NOT", buf, first->reg);
	    }
	    else
	    {
		opstr(first->operands, buf);
		opstr(first->operands->next_op, buf2);
		
		printf("%-6s %s, %s -> $%d\n", opr, buf, buf2, first->reg);
	    }
	}
	else if (first->op == MLOR_OP_APPEND       || first->op == MLOR_OP_CONS         ||
		 first->op == MLOR_OP_INTERSECTION || first->op == MLOR_OP_UNION        ||
		 first->op == MLOR_OP_COMPLEMENT   || first->op == MLOR_OP_BICOMPLEMENT ||
		 first->op == MLOR_OP_ELEMENT_AT   || first->op == MLOR_OP_MEMBER       ||
		 first->op == MLOR_OP_SUBSET)
	{
	    switch (first->op)
	    {
		case MLOR_OP_APPEND:	    strcpy(opr, "APP"); break;
		case MLOR_OP_CONS:	    strcpy(opr, "CON"); break;
		case MLOR_OP_INTERSECTION:  strcpy(opr, "ITR"); break;
		case MLOR_OP_UNION:	    strcpy(opr, "UNN"); break;
		case MLOR_OP_COMPLEMENT:    strcpy(opr, "COM"); break;
		case MLOR_OP_BICOMPLEMENT:  strcpy(opr, "BCM"); break;
		case MLOR_OP_ELEMENT_AT:    strcpy(opr, "ELA"); break;
		case MLOR_OP_MEMBER:	    strcpy(opr, "MBR"); break;
		case MLOR_OP_SUBSET:	    strcpy(opr, "SBS"); break;
	    }
	    
	    opstr(first->operands, buf);
	    opstr(first->operands->next_op, buf2);
	    
	    printf("%-6s %s, %s -> $%d\n", opr, buf, buf2, first->reg);
	}
	else if (first->op == MLOR_OP_CAR) 
	{
	    opstr(first->operands, buf);
	    printf("%-6s %s -> $%d\n", "CAR", buf, first->reg);
	}
	else if (first->op == MLOR_OP_CDR)
	{
	    opstr(first->operands, buf);
	    printf("%-6s %s -> $%d\n", "CDR", buf, first->reg);
	}
	else if (first->op == MLOR_OP_CALL_BI) {}
	else if (first->op == MLOR_OP_NOP) 
	{
	}
	else if (first->op == MLOR_OP_LI)
	{
	    opstr(first->operands, buf);
	    printf("%-6s %s -> $%d\n", "LI", buf, first->reg);
	}
	else
	{
	    printf("UNKNOWN");
	}
	
	first = first->next;
    }
}







