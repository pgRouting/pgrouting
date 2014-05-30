// registrar.hpp	A (static) object for maintaining object serial numbers
//			for classes that care about such things.  This particulary
//			useful for random number generators, so they don't always
//			generate the same sequence when declared as automatics
//			The registrar object is instantiated in the '.cpp' file
//			the user just calls the serial_number method:
//
//	int my_id = registrar.serial_number();
//
// rcsid: @(#)registrar.hpp	1.1 13:43:51 10/14/93   EFC

#ifndef REGISTRAR_HPP_
#define REGISTRAR_HPP_ 1.1

class Registrar
{
	private:
	  static unsigned int seq_no;
	public:
	  Registrar();
	 ~Registrar() {}

	  unsigned int serial_number();
};


extern Registrar registrar;

#endif
