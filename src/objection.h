/*
   objection.h
   This is the main header file for the objection engine
   it should be included in every file that is associated
   with the objection engine. It includes all the files
   that are integral to the engine (such as gc.h, etc.).

   Written by Ryan Sandor Richards
   Copyright 2005 Ryan Sandor Richards, All rights reserved
*/

#ifndef OBJECTION_H
#define OBJECTION_H

// This is included for the garbage collector

//#define GC_DEBUG 1

#include <gc/gc.h>
#include <assert.h>

// Main options for the engine
#define OBJ_OPTION_TREE_ONLY		1
#define OBJ_OPTION_COMPILE_JAVA		2


#endif
