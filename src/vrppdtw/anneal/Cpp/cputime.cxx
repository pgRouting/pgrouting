// cputime.cpp	implementation of CPUTime class

static const char rcsid[] = "@(#)cputime.c++	1.7 11:13:35 5/14/93   EFC";

#ifdef CRAY
#ifndef TIMES
#define TIMES
#endif
#endif

#ifdef TIMES
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>

#ifdef CRAY
#define HZ	1000000      /* microseconds */
#endif

#ifndef HZ
#define HZ       60
#endif

#else                             // assume RUSAGE unless TIMES is defined
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include <cputime.hpp>

#include <iomanip.h>

CPUTime::operator double()
{
	get_cpu_time();

	return usertime + systime;
}

void CPUTime::report(const char *label)
{
	get_cpu_time();

	if ( no_print )
		return;

	if ( label == NULL )
		my_os << "CPU";
	else
		my_os << label;

	my_os << " time (sec):\t";
	// my_os << setiosflags(ios::fixed)

	int old_precision = my_os.precision(4);

	my_os << usertime << "u  " << systime << "s\ttotal: "
	      << (usertime + systime) << endl << setprecision( old_precision )
	      << flush;

}

void CPUTime::get_cpu_time()
{
#ifdef TIMES
        struct tms time;

        times(&time);

        usertime = (float)time.tms_utime / (float)HZ;
        systime = (float)time.tms_stime / (float)HZ;

#else

        struct rusage usage;

        getrusage(RUSAGE_SELF, &usage);

        usertime = (double)usage.ru_utime.tv_sec +
                        (double)usage.ru_utime.tv_usec / 1000000.;
        systime = (double)usage.ru_stime.tv_sec +
                        (double)usage.ru_stime.tv_usec / 1000000.;

#endif

}






