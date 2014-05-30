// cputime.hpp		a class for determining the CPU usage
//			starts timing when the object is created
//			and reports the time used when it is deleted
//			The second form of the ctor does not print results
//			it is intended for use with the double() operator
// rcsid: @(#)cputime.hpp	1.7 11:13:22 5/14/93   EFC

//	use 	-lbsd 		when using this class

#ifndef CPU_TIME_HPP_
#define CPU_TIME_HPP_ 1.7

#include <stdlib.h>
#include <iostream.h>

class CPUTime
{
	private:
	  ostream& my_os;
	  int no_print;
	  double usertime, systime;
	  void get_cpu_time();
	public:
	  CPUTime(ostream& user_os = cerr) : my_os(user_os), no_print(0)
				 { report("Preprocessing"); }
	  CPUTime(const int) : my_os(cerr), no_print(1) {}
	 ~CPUTime() 	{  report( "Total CPU" ); }

	  operator double();
	  void report(const char *str = NULL);
};

#endif

