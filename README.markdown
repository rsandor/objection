Objection
================================================================================
by Ryan Sandor Richards


Introduction, "I OBJECT!"
--------------------------------------------------------------------------------

Objection was my first serious attempt at designing a programming language and
I decided to do it all in C. Late in 2004 I began to experiment with flex /
bison and began writing small toy languages using the utilities. As I became
better with the tools I decided I needed a bigger challenge. Luckily in early
2005 I took a Programming Languages course at my college and being a brash 
Sophomore decided I'd like to meld my favorite language at the time (C) with 
my new found friend (Scheme). The result was objection.

The name actually comes from the original plan which was to find a way to 
fully realize class based object-oriented programming through a functional
language with mainly imperative (specifically C-style) syntax. In the process
I bobbed and weaved around the idea of objects and my lack of understanding
when it came to compiler/interpreter engineering and language design guided
me towards a smaller scope of features.

The project took about a month to complete and at 7,000 lines of ANSI-C it was
the biggest project I had yet undertaken. So much so was my obsession with
programming the language all I can remember from that semester was going to
my courses and programming objection.

After I finished this rendition of the language I put it down and I haven't
really touched it since. But I was rummaging through my old PowerBook G4 and
decided to throw the source up on GitHub :).

An Example Program, "Merge Sort" in Objection:
--------------------------------------------------------------------------------

	function main() {
		echoln {
			"Before Merge Sort:";
			biglist;
			"After:";
			mergeSort(biglist);
		};
	}

	function mergeSort(L) {
		error ( !isList(L) ) echoln("mergeSort(): argument must be a list");
		rec (CL) {
			if ( length(CL) <= 1 )
				CL;
			else {
				let parts = partition(CL); for merge( recur(`parts), recur(`~parts) );
			}
		} on (L)
	}

	function partition(L) {
		error ( !isList(L) ) echoln("partition(): argument must be a list");
		rec (CL, p1, p2, count, max) {
			cond {
				case (isEmpty(CL))
					p1 # p2 # list();
				case (count < (max / 2))
					recur( ~CL, p1 @ `CL, p2, count + 1, max );
				default
					recur( ~CL, p1, p2 @ `CL, count + 1, max );
			}
		} on (L, list(), list(), 0, length(L))
	}

	function merge(L1, L2) {
		error ( !isList(L1) || !isList(L2) ) echoln("merge(): both arguments must be lists");
		rec (p1, p2, L) {
			cond {
				case ( isEmpty(p1) && isEmpty(p2) ) L;
				case ( isEmpty(p1) ) recur(p1, ~p2, L @ `p2);
				case ( isEmpty(p2) ) recur(~p1, p2, L @ `p1);
				default
					if ( `p1 < `p2 )
						recur( ~p1, p2, L @ `p1 );
					else
						recur( p1, ~p2, L @ `p2 );
			}
		} on (L1, L2, list())
	}

The "rec" statement
--------------------------------------------------------------------------------

When looking at the source code above you'll see the user of some familiar
functional programming constructs and also a little syntactic sugar I cooked up 
back in the day called the "rec". The rec allows you to perform tail recursion 
in-line. Since the language was always meant to be "Program Functionally... 
in C?" it was a natural consequence of my vision.

The structure itself is very simple. First there is "declaration" portion,
this consists of declaring the structure with the "rec" keyword followed
by a list of arguments, like so:

	rec (p1, p2, L) 

Then comes the body. Blocks of expressions or statements are always surrounded
by curly braces. here' the body of the rec:
	
	{
		cond {
			case ( isEmpty(p1) && isEmpty(p2) ) L;
			case ( isEmpty(p1) ) recur(p1, ~p2, L @ `p2);
			case ( isEmpty(p2) ) recur(~p1, p2, L @ `p1);
			default
				if ( `p1 < `p2 )
					recur( ~p1, p2, L @ `p1 );
				else
					recur( p1, ~p2, L @ `p2 );
		}
	} 

Note the inner condition, expression group (again with curly braces) and how
single expressions require no such braces (delicious!). Recurring is done by
using the "recur" expression which takes arguments as if the rec were actually
a fully fledged function. Finally comes the application phase of the rec where 
you begin recurring on the function defined in the declaration and the body.
This is done using the "on" keyword like so:
	
	on (L1, L2, list())

And blammo! You've got inline tail-recursion with C-style syntax without 
closures! That was my favorite feature of the language and the one I highlight 
most when talking about it. Not bad for a sophomore with no idea what he was 
doing, eh? ;)

Usage and Compiling Notes:
--------------------------------------------------------------------------------

I have the source compiling on Mac OS 10.3.9 with the Hans Boehm Garbage
collection library installed "somewhere" on my old machine. I have yet to 
update and test the source on my new Mac but should get around to it soonish.

If you want to give it a run, go ahead and knock yourself out. If you get it
compiling on multiple platforms I'd love to hear from you. The last I checked
I was able to get it compiling on my mac and on the linux boxes back at
school. So it should be ready to rock right out of the box (given HBGC).

License and Legal Mumbo-jumbo (MIT License)
--------------------------------------------------------------------------------

Copyright (c) 2005-2010 Ryan Sandor Richards

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.


