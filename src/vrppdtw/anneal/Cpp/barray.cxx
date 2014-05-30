// barray.cpp	Implementation of bare Float,Double or Complex Array class

//                      (c) Copyright 1995, Everett F. Carter Jr.
//                      Permission is granted by the author to use
//			this software for any application provided this
//			copyright notice is preserved.


#include <barray.hpp>

static const char rcsid[] = "@(#)barray.c++	1.17 12:27:14 6/3/96   EFC";

// #define DEBUG
// #define DEBUG_RESIZE

#ifdef DEBUG
// #include <debug.hpp>
#include <traceback.hpp>

// extern Debug debug;

#endif

#ifdef __TURBOC__
#include <signal.h>
#include <float.h>

static void handler(int sig)
{
#ifdef DEBUG
	cerr << "error handler, signal = " << sig << '\n';
#endif

	// clear the error
	_fpreset();
	
	signal(SIGFPE, handler);
	
}

#endif

BasicArray::BasicArray() : n(sze), sze(1), wdth(1), rsize(1)
{
#ifdef DEBUG
	TraceBack tb( "BasicArray::BasicArray()");
#endif
	x = new scalar_type[ 1 ];

}

BasicArray::BasicArray(const int ni) : n(sze), sze(ni), wdth(ni+1), rsize(ni)
{
#ifdef DEBUG
	TraceBack tb( "BasicArray::BasicArray(int)");
	tb << "n = " << n << endl;
#endif
	x = new scalar_type[ ni ];

}

BasicArray::BasicArray(const BasicArray& f2) : n(sze)
{
#ifdef DEBUG
	TraceBack tb( "BasicArray::BasicArray(BasicArray&)");
#endif
	rsize = sze = f2.size();
	wdth = f2.wdth;
	
	x = new scalar_type[rsize];

	for (int k = 0; k < rsize; k++)
		x[k] = f2.x[k];

}

BasicArray::~BasicArray()
{
#ifdef DEBUG
	TraceBack tb( "BasicArray::~BasicArray()");
#endif
#ifdef __ZTC__
		if ( x ) delete [n]x;
#else
	        if ( x ) delete []x;
#endif

		x = NULL;
		rsize = sze = 0;                 
}

BasicArray& BasicArray::operator = (const BasicArray& f2)
{
#ifdef DEBUG
	TraceBack tb( "BasicArray::operator=(BasicArray)");
#endif
	if ( this == &f2 )
		return *this;

	if ( x != NULL && rsize != f2.size() )
	{
#ifdef __ZTC__
		delete [n]x;
#else                        
		delete []x;
#endif

		rsize = f2.size();
		x = new scalar_type[rsize];
	}

	sze = rsize;
	// wdth = f2.wdth;


	for (int k = 0; k < rsize; k++)
		x[k] = f2.x[k];

	return *this;	
}

void BasicArray::trim(const int nsize)	// make the array SEEM to be smaller
{
#ifdef DEBUG
	TraceBack tb( "BasicArray::trim()" );
	tb << "BasicArray::trim( " << nsize << " ), trimming from " << sze;
	tb << " , rsize = " << rsize << '\n';
#endif
	if ( nsize >= 0 && nsize <= rsize )
		sze = nsize;
}

void BasicArray::reset(const scalar_type val)
{
#ifdef DEBUG
	TraceBack tb( "BasicArray::reset(scalar assignment)" );
	tb << "sze = " << sze << endl;
#endif
	for (int k = 0; k <sze; k++)
        		x[k] = val;
}

void BasicArray::resize(const int nsize)
{
#ifdef DEBUG_RESIZE
	TraceBack tb( "BasicArray::rsize()" );
	tb << "nsize = " << nsize << "  sze = " << sze << "  rsize = " << rsize
	   << endl;
	tb << "*x = " << x << endl;
#endif
	 int k, kend;

	sze = nsize;

	if ( nsize == rsize )
		return;			// nothing to do

	 kend = rsize;

	  if ( nsize < kend )			// keep the smaller size
			kend = nsize;

	  scalar_type *xn = new scalar_type[ nsize ];

	  if ( x )
	  {
	 	 // copy the old data
	 	 for (k = 0; k < kend; k++)
				xn[k] = x[k];

#ifdef __ZTC__
	  	delete [kend]x;
#else          
	  	delete []x;
#endif
	  }
	  else
		for (k = 0; k < nsize; k++)
				xn[k] = 0.0;

	  rsize = nsize;
	  x = xn;

	  wdth = rsize + 1;
}

int BasicArray::write(ofstream &ofs, int count,const int first) const
{
	if ( count < 0 )
		count = sze - first;

	if ( count <= 0 )
		return -1;

	ofs.write( (char *)&(x[first]), count * sizeof( scalar_type ) );

	if ( ofs.fail() )
		return -1;
	else
		return 0;
 
}

int BasicArray::read(ifstream &ifs, int count,const int first)
{

	if ( count < 0 )
		count = sze;

	if ( (first + count) > sze )
		resize( first+count );

 
	ifs.read( (char *)&(x[first]), count * sizeof( scalar_type ) );


	if ( ifs.fail() )
		return -1;
	else
		return 0;

}

istream& operator>> (istream& is, BasicArray& arry)
{
	scalar_type new_x;
	int k, kend;

#ifdef __TURBOC__
	static int first = 1;

	
	if ( first )
	{
		signal( SIGFPE, handler );
		first = 0;
	}
#endif

	if ( is.eof() )
		return is;


	kend = arry.size();

	for (k = 0; k < kend; k++)
	{		
		is >> new_x;
		

 		// end of file reached, abort the read
		if ( is.eof() || is.fail() )
			return is;

		arry[k] = new_x;		
	}
	
	return is;
	
}

ostream& operator<< (ostream& os,const BasicArray& arry)
{
	int iend = arry.size();

	for (int i = 0; i < iend;  )
	{
		os << arry[i];
		if ( (++i % arry.wdth) == 0 )
				os << '\n';
		else
				os << "   "; 
	}

//	if ( (i % arry.wdth) != 0 )	os << '\n';
	
	return os;
	
}


int operator!=(const BasicArray &a,const BasicArray &b) //      int i = (a != b)
{
        if ( a.size() != b.size() )
                return 1;

        const int sz = a.size();
        int l = 1;

        for (int i = 0; i < sz; i++)
                        l = l && ( a[i] != b[i] );

        return l;

}

int operator==(const BasicArray &a,const BasicArray &b) //      int i = (a == b)
{
        if ( a.size() != b.size() )
                return 0;

        const int sz = a.size();
        int l = 1;

        for (int i = 0; i < sz; i++)
                        l = l && ( a[i] == b[i] );

        return l;

}






