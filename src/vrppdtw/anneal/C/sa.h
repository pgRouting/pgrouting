/* sa.h	  	Prototypes for Simulated Annealing library */

/* rcsid: @(#)sa.h	1.2 15:54:42 3/30/93   EFC   */

#ifndef SA_PACKAGE_H_
#define SA_PACKAGE_H_ 1.2

#ifdef NO_PROTO
typedef float (*CostFunction)();
#else
typedef float (*CostFunction)(float*);
#endif

/* the value that causes the "set/query" functions to just query */
#define NO_VALUE	-1
#define NO_VALUE_INT   -1
#define NO_VALUE_FLOAT -1.0

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO_PROTO

int SAInit();
void SAfree();
int SAiterations();
int SAdwell();
float SABoltzmann();
float SAlearning_rate();
float SAtemperature();
float SAjump();
float SArange();
void SAinitial();
void SAcurrent();
void SAoptimum();
float SAmelt();
float SAanneal();


#else

int SAInit(CostFunction f, int d);
void SAfree();
int SAiterations(int it);
int SAdwell(int d);
float SABoltzmann(float k);
float SAlearning_rate(float r);
float SAtemperature(float t);
float SAjump(float j);
float SArange(float r);
void SAinitial(float* xi);
void SAcurrent(float* xc);
void SAoptimum(float* xb);
float SAmelt(int iters);
float SAanneal(int iters);

#endif

#ifdef __cplusplus
}
#endif

#endif

