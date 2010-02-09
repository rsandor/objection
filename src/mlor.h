
/*
    File: mlor.h
    Project: Objection Stage 2
    Author: Ryan Sandor Richards
    
    Copyright  Ryan Sandor Richards, All rights reserved.
    
    This is the main header file for structure definitions,
    and function prototypes for the Medium Level Objection
    Representation.
    
    The basic idea is that after parsing the input into a
    high level parse tree, we can convert the tree into a
    serise of medium level instructions that a special 
    interpreter can evaluate. When changing the parse tree
    to the medium level form we can also accomplish many
    semantic checks to ensure that the program is valid.
    By performing the checks when translating the highlevel
    form to the lower level form we need not worry about
    handling many of the checks during run time, and thus
    making the program operate more quickly.
    
    This is the obvious next step after raw interpretation
    of the parse tree.
*/


#ifndef MLOR_H
#define MLOR_H

/* Includes needed for the MLOR */
#include "objection.h"
#include "parse_node.h"
#include "scalar.h"
#include "envhash.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



/* 
    These are the op codes for MLOR instructions
    
    Type: Flow/Structure/Scoping Operations
    
    Name	Description
    -----------------------------------------------------------------
    SCOPE	Causes the interpreter to push a new scope
		frame onto the stack
    
    ENDSCOPE	Pops the recent scope off of the stack frame
    
    BIND	Binds a name with a value in the current
		scope
    
    RETURN	Stops normal interpreation path and goes to
		the return address and commences interpretation
		from that point on
    
    TERM	Immediately terminates interpretation with no
		message.
    
    BRANCH	Branches if the operand supplied is true

    CALL	Sets the return address to the next instruction,
    
    CALL_BI	For calls on builtins
    
    LIST	Creates a list out of the operands
    
    ID		Retrieves a value from the current environment
    
    
    
    Type: Arithmetic Operations
    
    Name	Description
    -----------------------------------------------------------------
    ADD		Adds the first operand to the second
    
    SUB		Subtracts the first operand from the second
    
    MUL		Multiplies the first operand to the second
    
    DIV		Divides the first operand by the second operand
    
    MOD		Gives the Modulus Division of the first operand by
		the second operand
		
    
    
    Type: Boolean Operations
    
    Name	Description
    -----------------------------------------------------------------
    AND		Determines if all operands evaluate to boolean true
    
    OR		Determines if any operand evaluates to a true value
    
    NOT		Takes to logical negation of an operand
    
    XOR		Determines if only one operand is true
    
    IMP		Logical implication
    
    DIMP	Logical double implication
    
    EQ		Tests for equality of operands
    
    NEQ		Tests for inequality of operands
    
    LT		Operand1 < Operand2
    
    GT		Operand1 > Operand2
    
    LTE		Operand1 <= Operand2
    
    GTE		Operand1 >= Operand2
    
    EQV		Tests if two operands are equivilent
    
    
    Type: List Operations
    
    Name	    Description
    -----------------------------------------------------------------
    APPEND	    Appends a list with a value
    
    CONS	    Appends to the beginning of a list with a value
    
    CAR		    Finds the first argument of the operand
    
    CDR		    Finds the list containing all but the first
		    element of the operand
    
    INTERSECTION    Finds the list containing elements contained in
		    both the first operand and the second operand
    
    UNION	    Finds the list containing elements from both
		    operands
    
    COMPLEMENT	    Finds the complement of operand1 in respect to
		    operand2
    
    BICOMPLEMENT    Finds the compelment of operand1 in repect to
		    operand2 and adds elements of that list to the
		    complement of operand2 in respect to operand1
    
    MEMBER	    Finds out if the first operand is an element of
		    the second operand
    
    SUBSET	    Finds out if the first operand is an element of
		    the second operand
    
    ELEMENT_AT	    Finds the element at the first operand's value in
		    the second operand.
*/

#define MLOR_OP_SCOPE		1
#define MLOR_OP_ENDSCOPE	2
#define MLOR_OP_BIND		3
#define MLOR_OP_RETURN		4
#define MLOR_OP_GOTO		5
#define MLOR_OP_TERM		6
#define MLOR_OP_BRANCH		7
#define MLOR_OP_CALL		8
#define MLOR_OP_LIST		9
#define MLOR_OP_ID		10
#define MLOR_OP_ADD		11
#define MLOR_OP_SUB		12
#define MLOR_OP_MUL		13
#define MLOR_OP_DIV		14
#define MLOR_OP_MOD		15
#define MLOR_OP_AND		16
#define MLOR_OP_OR		17
#define MLOR_OP_NOT		18
#define MLOR_OP_XOR		19
#define MLOR_OP_IMP		20
#define MLOR_OP_DIMP		21
#define MLOR_OP_EQ		22
#define MLOR_OP_NEQ		23
#define MLOR_OP_LT		24
#define MLOR_OP_GT		25
#define MLOR_OP_LTE		26
#define MLOR_OP_GTE		27
#define MLOR_OP_EQV		29
#define MLOR_OP_APPEND		30
#define MLOR_OP_CONS		31
#define MLOR_OP_CAR		32
#define MLOR_OP_CDR		33
#define MLOR_OP_INTERSECTION	34
#define MLOR_OP_UNION		35
#define MLOR_OP_COMPLEMENT	36
#define MLOR_OP_BICOMPLEMENT	37
#define MLOR_OP_MEMBER		38
#define MLOR_OP_SUBSET		39
#define MLOR_OP_ELEMENT_AT	40
#define MLOR_OP_CALL_BI		41
#define MLOR_OP_NOP		42
#define MLOR_OP_LI		43

/*
    This is the major MLOR instruction structure.
    It holds all the information that the specialized
    MLOR interpreter needs in order to interpret the
    meaning of the instruction.
    
    
    Member	Description
    -------------------------------------------------------
    op		Opcode for the MLOR instruction
    
    line	Line in the original source file this
		MLOR instruction originated from
   
    operands	Operands for the instruction
    
    reg		Register number to store the result
		in if needed. A value of -1 indicates
		the result doesn't need to be stored.
		A value of 0 indicates that the value
		should be stored in the special return
		value register.
    
    label	Number of the label associated with this
		instruction
    
    next	Next MLOR instruction for interpretation
    
*/
typedef struct mlor_inst MLOR_INST;
struct mlor_inst
{
    short	op;
    long	line;
    SCALAR *	operands;
    SCALAR *	last_operand;
    short	reg;
    long	label;
    MLOR_INST *	next;
};

/* Handy dandy MLOR inst macros */

#define set_reg(I, R) \
    (I)->reg = (R)
    
#define add_operand(I, O)				\
    if ((I)->operands == NULL) {			\
	(I)->operands = (O);				\
	(I)->last_operand = (O);			\
    } else if ((I)->operands == (I)->last_operand) {	\
	(I)->operands->next_op = (O);			\
	(I)->last_operand = (O);			\
    } else {						\
	(I)->last_operand->next_op = (O);		\
	(I)->last_operand = (O);			\
    }
	


/*
    This is a structure used by the translators for easy
    reference of the beginning of a block and the end
    of a block of MLOR.
    
    Member	Description
    -------------------------------------------------------
    first	First instruction of the block
    last	Last instruction of the block
    label	Holds the next label value
*/
typedef struct mlor_block MLOR_BLOCK;
struct mlor_block
{
    long label;
    MLOR_INST *first;
    MLOR_INST *last;
};



/*
    Handy Dandy MLOR block macros
*/

#define add_inst(B, I)			    \
    if ((B)->first == NULL) {		    \
	(B)->first = (I);		    \
	(B)->last = (I); }		    \
    else if ((B)->first == (B)->last) {	    \
	(B)->first->next = (I);		    \
	(B)->last = (I); } else {	    \
	(B)->last->next = (I);		    \
	(B)->last = (I); }

#define first_inst(B)	(B)->first

#define last_inst(B)	(B)->last

#define append_block(A, B) \
    if ((A)->first = (A)->last) { \
	(A)->first->next = (B)->first; \
	(A)->last = (B)->last; } else { \
	(A)->last = (B)->last; }

/*
    The following are function prototypes for the
    MLOR library.
*/
MLOR_BLOCK *new_block();
SCALAR *reg_scalar(int r);
MLOR_INST *new_instruction(short op, long line, SCALAR *operands);
MLOR_INST *translate(PARSE_NODE *funcs, PARSE_NODE *defs);
SCALAR *translate_node(PARSE_NODE *n, MLOR_BLOCK *current_block, int next_reg);
void print_mlor(MLOR_INST *first);

#endif
