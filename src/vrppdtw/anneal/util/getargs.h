/*	Getargs.h	Typedefs and defines needed for getargs

	rcsid: @(#)getargs.h	1.2 23:48:46 4/19/94   EFC

*/

#ifndef _GETARGS_H
#define _GETARGS_H 1.2

#ifdef REAL
#undef REAL
#endif

#define	INTEGER		0
#define	BOOLEAN		1	/* Turns ON */
#define	CHARACTER	2
#define	STRING		3
#define	PROC		4
#define REAL		5
#define SWITCH		6	/* Toggles ON-OFF */
#define LONG		7

typedef	struct
{
	unsigned	clarg  : 7  ;	/* Command line switch	*/
	unsigned	type : 4   ;	/* variable type	*/
	int		*variable  ;	/* pointer to variable	*/
	char		*errmsg    ;	/* pointer to error message */
}
ARG;


/* function prototypes */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO_PROTO
int getargs();
void pr_usage();
#else
int getargs(int, char **, ARG *, int);
void pr_usage(ARG *, int);
#endif

#ifdef __cplusplus
}

#include <stdlib.h>

#endif

#endif
