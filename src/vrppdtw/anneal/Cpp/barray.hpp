// barray.hpp		A Basic floating point or complex array class
//			just a simple array

//                      (c) Copyright 1995, Everett F. Carter Jr.
//                      Permission is granted by the author to use
//			this software for any application provided this
//			copyright notice is preserved.

// rcsid: @(#)barray.hpp	1.17 12:27:27 6/3/96   EFC

#ifndef _BASIC_ARRAY_HPP_
#define _BASIC_ARRAY_HPP_  1.17

#ifdef __ZTC__
#include <fstream.hpp>
#else
#include <fstream.h>
#endif

#include <scalar.hpp>


class BasicArray
{
    private:
	int rsize;				// the actual size of the array
	int wdth;
    protected:
	int sze;				// the known size of the array
	scalar_type *x;
    public:
	const int& n;
	BasicArray();
    	BasicArray(const int ni);
        BasicArray(const BasicArray& f2);
        virtual ~BasicArray();

        BasicArray& operator = (const BasicArray& f2);// assignment from BasicArray

	virtual int size() const { return sze; }
	virtual void resize(const int nsize);

	
	void freeze() { resize( n ); } // fix the actual size to the current size
	void trim(const int nsize);    // make array seem smaller than it is
	void untrim()		{ sze = rsize; }

 	// scalar assignment
        virtual void reset(const scalar_type val = 0.0);
	scalar_type operator=(const scalar_type val) { reset(val); return val; }

	virtual scalar_type& operator[](const int i) { return x[i]; }
	scalar_type  operator[](const int i) const   { return x[i]; }

        friend int operator!=(const BasicArray&,const BasicArray&);    // inequality
        friend int operator==(const BasicArray&,const BasicArray&);    // equality

  	// binary read and write
	virtual int read(ifstream &ifs, int count = -1,const int first = 0);
	virtual int write(ofstream &ofs, int count = -1,const int first = 0) const;

	// set output columns
	void width(const int w) { wdth = w; }
	friend istream& operator>>(istream& is, BasicArray& ary);
	friend ostream& operator<<(ostream& os, const BasicArray& ary);
};



#endif




