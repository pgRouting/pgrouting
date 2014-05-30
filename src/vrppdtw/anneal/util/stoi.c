/* stoi.c	-- More powerful version of atoi	*/

static char rcsid[] = "@(#)stoi.c	1.1 13:15:03 7/1/92   EFC";

#define islower(c)	( 'a' <= (c) && (c) <= 'z' )
#define toupper(c)	( islower(c) ? (c) - ('a' - 'A') : (c) )

long int stol(instr)
register char **instr;
/*	Convert string to integer.  If string starts with 0x it is interpreted
*	as a hex number, else if ti starts with a 0 it is octal, else it is
*	decimal.  Conversion stops on encountering the first character that
*	is not a digit in the indicated radix.  *instr is updated to point past
*	the end of the number.
*/
{
	register long int	num = 0;
	register char	*str;
	int		sign = 1;

	str = *instr;

	while (*str == ' ' || *str == '\t' || *str == '\n' )
		str++;

	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	
	if (*str == '0')
	{
		++str;
		if (*str == 'x' || *str == 'X')
		{
			str++;
			while( ('0' <= *str && *str <= '9') ||
			       ('a' <= *str && *str <= 'f') ||
			       ('A' <= *str && *str <= 'F')	)
			{
				num *= 16;
				num += ('0' <= *str && *str <= '9') ?
					*str - '0'	:
					toupper(*str) - 'A' + 10;
				str++;
			}
		}
		else
		{
			while( '0' <= *str && *str <= '7' )
			{
				num *= 8;
				num += *str++ - '0';
			}
		}
	}
	else
	{
		while( '0' <= *str && *str <= '9')
		{
			num *= 10;
			num += *str++ - '0';
		}
	}

	*instr = str;
	return ( num * sign );
}

int stoi(instr)                     /* INTeger version of stol */
char **instr;
{

         return (int)stol(instr);
}









