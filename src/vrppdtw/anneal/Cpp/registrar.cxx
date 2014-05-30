// registrar.c++

static const char rcsid[] = "@(#)registrar.c++	1.1 13:43:58 10/14/93   EFC";

#include <registrar.hpp>

unsigned int Registrar::seq_no = 0;

Registrar registrar;

Registrar::Registrar()
{
	++seq_no;
}

unsigned int Registrar::serial_number()
{
	return seq_no++;
}

