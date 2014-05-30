/* for cpu timing uses getrusage() unless TIMES is defined */

static char rcsid[] = "%W% %U% %G%   EFC";

#ifdef TIMES
#include <sys/tupes.h>
#include <sys/times.h>
#include <sys/param.h>

#ifndef HZ
#define HZ       60.0
#endif

#else

#include <sys/time.h>
#include <sys/resource.h>

#endif

#include <cputime.h>

#ifdef NO_PROTO
void report_cpu( fp, str )
FILE *fp;
char *str;
#else
void report_cpu(FILE *fp, char *str)
#endif
{
      static call_no = 0;
      double utime, stime;

      get_cpu_time(&utime, &stime);
  
      if ( str == NULL )
	{
	  if ( call_no++ == 0 )
                fprintf(fp,"Preprocessing");
	  else
	        fprintf(fp,"Total CPU");
	}
      else
	        fprintf(fp,"%s", str);

      fprintf(fp," time:\t%2.2fu  %2.2fs\t%2.2f (sec)\n", utime, stime,
	                                                   utime + stime);

     
}

#ifdef NO_PROTO
void get_cpu_time(usertime, systime)
double *usertime;
double *systime;
#else
void get_cpu_time(double *usertime, double *systime)
#endif
{
#ifdef TIMES
     struct tms time;

     times( &time );

     *usertime = (double)time.tms_utime / (double)HZ;
     *systime  = (double)time.tms_stime / (double)HZ;

#else

     struct rusage usage;

     getrusage( RUSAGE_SELF, &usage );

        *usertime = (double)usage.ru_utime.tv_sec +
                        (double)usage.ru_utime.tv_usec / 1000000.;
        *systime = (double)usage.ru_stime.tv_sec +
                        (double)usage.ru_stime.tv_usec / 1000000.;

#endif
     
}

