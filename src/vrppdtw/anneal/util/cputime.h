/* cputime.h           declarations for CPU timing call  */
/* rcsid: %W% %U% %G%   EFC */
/* if this is used while in SYSV you need to add     -lbsd   to the link step */

#ifndef CPU_TIME_H_
#define CPU_TIME_H_ %I%

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO_PROTO
void report_cpu();
void get_cpu_time();
#else
void report_cpu(FILE *, char *);
void get_cpu_time(double *usertime, double *systime);
#endif

#ifdef __cplusplus
}
#endif


#endif
