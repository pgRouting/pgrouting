// random.hpp		A collection of random number generator objects

//                      (c) Copyright 1995, Everett F. Carter Jr.
//                      Permission is granted by the author to use
//			this software for any application provided this
//			copyright notice is preserved.

// rcsid: @(#)random.hpp	1.10 11:33:57 6/9/95

#ifndef RANDOM_HPP_
#define RANDOM_HPP_ 1.10

#include <randgen.hpp>
#include <r250.hpp>

// This is a base generator class to handle all the details of the encapsulation
// of an actual base generator that is used by all the application level
// generators.  It has a protected ctor since the user never uses this directly.
class RandomGeneratorBase : public RandomGenerator
{
	private:
           int internal;
	protected:
           RandomGenerator *base;           // The base generator. NOTE: If an
                                            // external generator is attached
                                            // the application still owns it and
                                            // must handle its deletion

           virtual Real ranf()              { return base->ranf(); }
	   virtual unsigned long int rani() { return base->rani(); }

           // protected ctor, uses R250 as default base generator
	   RandomGeneratorBase() : internal(1) { base = new R250; }
	public:
           virtual ~RandomGeneratorBase() { if ( internal ) delete base; }
	   virtual long seed(const long int ik = 0)  { return base->seed(ik); }

	   // attach a base generator to this one
           void attach(RandomGenerator& rgen)   { if ( internal ) delete base;
						  base = &rgen;
                                                  internal = 0; }

           // attach a base generator via an operator
           RandomGeneratorBase& operator<<(RandomGenerator& rgen)
                                { attach(rgen); return *this; }


};

class RUniform : public RandomGeneratorBase
{
	public:
	  RUniform() {}
	  RUniform(const long useed) { seed(useed); }
	 ~RUniform() {}
	  Real number(const Real a = 0.0,const Real b = 1.0);
};

class IUniform : public RandomGeneratorBase
{
	public:
	  IUniform() {}
	  IUniform(const long useed) { seed(useed); }
	 ~IUniform() {}
	  int number(const int a,const int b);
};

class Expntl : public RandomGeneratorBase
{
	public:
	  Expntl() {}
	  Expntl(const long useed) { seed(useed); }
	 ~Expntl() {}
	  Real number(const Real a);
};

class Erlang : public RandomGeneratorBase
{
	public:
	  Erlang() {}
	  Erlang(const long useed) { seed(useed); }
	 ~Erlang() {}
	  Real number(const Real a,const Real b);
};

class Hyperx : public RandomGeneratorBase
{
	public:
	  Hyperx() {}
	  Hyperx(const long useed) { seed(useed); }
	 ~Hyperx() {}
	  Real number(const Real a,const Real b);
};

class Normal : public RandomGeneratorBase
{
	private:
	  Real z2;
	  int use_z2;
	public:
	  Normal() : z2(0.0), use_z2(1) {}
	  Normal(const long useed) : z2(0.0), use_z2(1) { seed(useed); }
	 ~Normal() {}
	  Real number(const Real a,const Real b);
	  void fresh() { use_z2 = 0; }
};

#endif




