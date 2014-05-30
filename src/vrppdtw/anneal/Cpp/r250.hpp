/* r250.hpp	prototypes for r250 random number generator,

                      (c) Copyright 1994, Everett F. Carter Jr.
                      Permission is granted by the author to use
                      this software for any application provided this
		      copyright notice is preserved.

		see:

		Kirkpatrick, S., and E. Stoll, 1981; A Very Fast Shift-Register
	        Sequence Random Number Generator, Journal of Computational Physics,
		V. 40.

		Maier, W.L., 1991; A Fast Pseudo Random Number Generator,
		Dr. Dobb's Journal, May, pp. 152 - 157


rcsid: @(#)r250.hpp	1.7 14:37:41 7/25/94   EFC

*/

#ifndef R250_HPP_
#define R250_HPP_ 1.7

#include <randgen.hpp>

class R250 : public RandomGenerator
{
	private:
	   static unsigned long int *r250_buffer;
	   static int r250_index;
           static int instances;
           long int seed_val;
           void init(const long int sd);
        public:
           R250();
           R250(const long sd) { instances++;	seed(sd); }

          ~R250() { if ( --instances == 0 )
#if defined( _ZTC__ ) && ( __ZTC__ <= 0x300 )
		 	delete [250]r250_buffer;
#else
		 	delete []r250_buffer;
#endif
 		  }
 
           long seed(const long sd = 0);

           unsigned long int rani( void );
	   Real       ranf( void ); 

};

#endif

