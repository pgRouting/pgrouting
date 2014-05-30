// rndlcg.hpp		The "minimal standard" linear congruential generator

//                      (c) Copyright 1994, Everett F. Carter Jr.
//                      Permission is granted by the author to use
//			this software for any application provided this
//			copyright notice is preserved.

//			see Park & Miller, 1988; Comm. of the ACM, 31(10)
//			pp. 1192 - 1201
// rcsid: @(#)rndlcg.hpp	1.2 14:38:36 7/25/94   EFC

#ifndef RND_LCG_HPP_
#define RND_LCG_HPP_ 1.2

#include <randgen.hpp>

class RandLCG : public RandomGenerator
{
	protected:
	   long int seed_val;
           static int instances;
	   static const long int quotient;
	   static const long int remainder;
	public:
	   RandLCG();
	   RandLCG(const long sd) : seed_val(sd) { instances++; }
	   ~RandLCG() { --instances; }

	   long seed(const long sd);
	   unsigned long int rani( void );
           Real         ranf( void );

 	   
};

#endif
