/*	GETARGS.C	Command line processor for C programs
*
*       -- added long integer parameter         -- EFC 26 Feb   1990
*	-- added toggle boolean (switch)	-- EFC 27 June  1988
*	-- with reals (double) allowed		-- EFC 16 April 1987
*
*		for original program, see DDJ #103
*		(C) Copyright 1985, Allen I. Holub.  All rights reserved
*
*/

/*
   define NO_EXIT if you want this routine to return after parsing legal
   arguments and to return all remaining arguments for the user to deal
   with separately
*/


static char rcsid[] = "@(#)getargs.c	1.2 23:49:01 4/19/94   EFC";

#include <stdio.h>
#include <string.h>
#include "getargs.h"

#define islower(c)	( 'a' <= (c) && (c) <= 'z' )
#define toupper(c)	( islower(c) ? (c) - ('a' - 'A') : (c) )

#ifdef NO_PROTO
extern int stoi();
extern int stol();
#else
extern int stoi(char **);
extern long stol(char **);
#endif

typedef int	(*PFI)();
double atof();
long atol();

static char	*setarg( argp, linep )
ARG		*argp;
char		*linep;
{
		/* Set an argument.  argp points at the argument table entry
		*  corresponding to *linep.  Return linep, updated to point
		*  past the argument being set.
		*/

	++linep;

	switch( argp->type )
	{
	case INTEGER:
		*argp->variable = stoi( &linep );
		break;

	case BOOLEAN:
		*argp->variable = 1;
		break;

	case CHARACTER:
		*argp->variable = *linep++;
		break;

	case STRING:
		/* *(char **)argp->variable = linep; */
		*(char **)argp->variable = strdup(linep);
		linep = "";
		break;
	case PROC:
		(* (PFI)(argp->variable) )( linep );
		linep = "";
		break;
	case REAL:
		*(double *)(argp->variable) = atof( linep );
 		/* skip past number */
		while ((*linep >= '0' && *linep <= '9') || *linep == '.' ||
			*linep == 'e' || *linep == 'E'  || *linep == '+' ||
			*linep == '-' )
				linep++;
		break;
	case SWITCH:
		*argp->variable = *argp->variable ? 0 : 1;
		break;
	case LONG:
		*(long *)argp->variable = stol( &linep );
		break;
	default:
		fprintf(stderr,"INTERNAL ERROR: BAD ARGUMENT TYPE\n");
		break;
	}
	return( linep );
}

/*--------------------------------------------------------------------*/

static ARG	*findarg( c, tabp, tabsize )
int		c, tabsize;
ARG		*tabp;
{
	/*	Return pointer to argument table entry corresponding
	 *	to c (or 0 if c isn't in table).
	 */

	for(; --tabsize >= 0 ; tabp++ )
		if( toupper(tabp->clarg) == toupper(c) )
			return tabp;
	return 0;
}
void pr_usage( tabp, tabsize )
ARG	*tabp;
int	tabsize;
{
	/*	Print the argtab in the form:
	 *		-<arg> <errmsg>		(default is <*variable>)
	 */

	for(; --tabsize >= 0 ; tabp++ )
	{
		switch( tabp->type )
		{
		case INTEGER:
			fprintf(stderr,"-%c<num>  %-40s (default is ",
					tabp->clarg, tabp->errmsg);
			fprintf(stderr,"%-5d)\n",*(tabp->variable) );
			break;

		case BOOLEAN:
			fprintf(stderr,"-%c       %-40s (default is ",
					tabp->clarg, tabp->errmsg);
			fprintf(stderr,"%-5s)\n",*(tabp->variable)
						? "TRUE": "FALSE" );
			break;

		case CHARACTER:
			fprintf(stderr,"-%c<c>    %-40s (default is ",
					tabp->clarg, tabp->errmsg);
			fprintf(stderr,"%-5c)\n",*(tabp->variable) );
			break;

		case STRING:
			fprintf(stderr,"-%c<str>  %-40s (default is ",
					tabp->clarg, tabp->errmsg);
			fprintf(stderr,"<%s>)\n",
					*(char **)tabp->variable);
			break;
		case REAL:
			fprintf(stderr,"-%c<real> %-40s (default is ",
					tabp->clarg, tabp->errmsg);
			fprintf(stderr,"%-5g)\n",*(double *)(tabp->variable) );
			break;

		case PROC:
			fprintf(stderr,"-%c<str>  %-40s\n",
					tabp->clarg, tabp->errmsg);
			break;
		case SWITCH:
			fprintf(stderr,"-%c       %-40s (toggle is  ",
					tabp->clarg, tabp->errmsg);
			fprintf(stderr,"%-5s)\n",*(tabp->variable)
						? "TRUE": "FALSE" );
			break;
		case LONG:
			fprintf(stderr,"-%c<long> %-40s (default is ",
					tabp->clarg, tabp->errmsg);
			fprintf(stderr,"%-5ld)\n",*(long *)(tabp->variable) );
			break;


		}
	}
}
#define	ERRMSG	"Illegal argument <%c>.  Legal arguments are:\n\n"

int	getargs(argc, argv, tabp, tabsize )
int	argc, tabsize;
char	**argv;
ARG	*tabp;
{
	/* Process command line arguments.  Stripping all command line
	 * switches out of argv.  Return a new argc.  If an error is found
	 * exit(1) is called (getargs won't return) and a usage message
	 * is printed showing all arguments in the table.
	 */

	register int	nargc	    ;
	register char	**nargv, *p ;
	register ARG	*argp	    ;

	nargc = 1;
	for(nargv = ++argv; --argc > 0; argv++)
	{
		if( **argv != '-' )
		{
			*nargv++ = *argv;
			nargc++;
		}
		else
		{
			p = (*argv) + 1;

			while ( *p )
			{
				if(argp = findarg(*p, tabp, tabsize))
					p = setarg( argp, p );
				else
				{
#ifdef NO_EXIT
				        *nargv++ = *argv;
				         nargc++;
				         break;
#else
					fprintf(stderr, ERRMSG, *p);
					pr_usage( tabp, tabsize );
					exit( 1 );
#endif
				}
			}
		}
	}
	return nargc;
}



