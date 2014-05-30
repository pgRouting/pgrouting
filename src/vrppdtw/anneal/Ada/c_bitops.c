/*  bitops.c              A collection of Bitwise operations
 *                        -- for importation into Ada

    (c) Copyright 1997 Everett F. Carter Jr.   Permission is granted
    by the author to use this software for any application provided
    this copyright notice is preserved.

 $Author:   skip  $
 $Workfile:   c_bitops.c  $
 $Revision:   1.0  $
 $Date:   20 Jul 1997 19:17:12  $


 */

#include <stdio.h>

int or_bits(int a, int b)
{
       return a | b;
}

int and_bits(int a, int b)
{
       return a & b;
}


int xor_bits(int a, int b)
{
       return a ^ b;
}

int not_bits(int a)
{
       return ~a;
}

