// simanneal.hpp	A general purpose Simulated Annealing Class
//	This version allows vector data

//                      (c) Copyright 1994, Everett F. Carter Jr.
//                      Permission is granted by the author to use
//			this software for any application provided this
//			copyright notice is preserved.

// rcsid: @(#)simann.hpp	1.4 15:02:11 7/25/94   EFC

#ifndef SIM_ANNEAL_HPP_
#define SIM_ANNEAL_HPP_ 1.4

#include <random.hpp>

#ifndef PI
#define PI            3.141592653589793
#endif


typedef float (*CostFunction)(float*);

class SimAnneal
{
	private:
	  RUniform uniform;
          CostFunction func;
	  int dimension, maxit, ddwell;
	  int err;
	  float *x, *xnew, *xbest;
	  float dt;			// temperature increment to use when melting
	  float c_jump;			// phase transition step size
	  float rrange;
	  float K, rho, t0, tscale, y, ybest;
	  int equilibrate(const float t, const int n);
	public:
	  SimAnneal() :	t0(0.0), K(1.0), rho(0.5), dt(0.1), tscale(0.1), ddwell(20),
	      maxit(400), c_jump(100.0), dimension(1), func(NULL), rrange(PI/2.0) {}
	  SimAnneal(CostFunction f,const int d = 1);
#if defined( __ZTC__ ) && ( __ZTC__ <= 0x300 )
	 ~SimAnneal() { delete [dimension]x; delete [dimension]xnew;
         		delete [dimension]xbest; }
#else
	 ~SimAnneal() { delete []x; delete []xnew;
         		delete []xbest; }
#endif

	  int set_up(CostFunction f, const int d = 1);

	  const int operator!() const { return err; }
          
	  float melt(const int iters = -1);
	  float anneal(const int iters = -1);

	  int iterations(const int m = -1) { if ( m > 0 ) maxit = m;
					     return maxit; }
	  int dwell(const int d = -1)      { if ( d > 0 ) ddwell = d;
					     return ddwell; }
	  float Boltzmann(const float k = -1.0)
					   { if ( k > 0.0 ) K = k;
					     return K; }

	  float learning_rate(const float r = -1.0)
					   { if ( r > 0.0 ) rho = r;
					     return rho; }
	  float temperature(const float t = -1.0)
					   { if ( t > 0.0 ) t0 = t;
					     return t0; }
	  float jump(const float j = -1.0)
					   { if ( j > 0.0 ) c_jump = j;
					     return c_jump; }
	  float range(const float r = -1.0)
					   { if ( r > 0.0 ) rrange = r;
						return rrange; }
	  void initial(float* xinit);
          void current(float* xcur);
	  void optimum(float* xopt);
};

#endif

