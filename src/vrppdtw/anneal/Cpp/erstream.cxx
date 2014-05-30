// erstream.c++		Implementation of general error reporting class

//                      (c) Copyright 1995, Everett F. Carter Jr.
//                      Permission is granted by the author to use
//			this software for any application provided this
//			copyright notice is preserved.


static const char rcsid[] = "@(#)erstream.c++	1.6 10:19:05 6/9/95   EFC";

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include <bool.h>

#include "erstream.hpp"

// #define DEBUG

#ifdef DEBUG
#include <taygeta/traceback.hpp>
#endif

static const char SOH = 1;       // internally used to delimit each message
static const char WARN = 2;
static const char FATAL = 3;
static const char SILENT = 4;
static const char FAIL = 5;

long int ErrorStream::efirst = 0;
long int ErrorStream::elast = LONG_MAX - 1;
long int ErrorStream::msgnum = 0;

Errorbuf::~Errorbuf()
{
#ifdef DEBUG
	TraceBack tb( "Errorbuf::~Errorbuf()" );
#endif
		 sync();

		 if ( space )
			delete []space;
}

int Errorbuf::doallocate()
{
#ifdef DEBUG
	TraceBack tb( "Errorbuf::doallocate()" );
#endif
	if ( (space = new char[129]) == '\0' )
			return EOF;

	setb(space, space + 129, 0);
	setp(space, space + 128);	// there is one extra space for overflow

	return 0;
}


int Errorbuf::sync()
{
#ifdef DEBUG
	TraceBack tb( "Errorbuf::sync()" );
#endif
	if ( pptr() && pptr() > pbase() )
		return overflow( EOF );		// flush waiting output
	else
		return 0;
}


#ifdef INHERIT_ERRORBUF
int ErrorStream::overflow(int ch)
#else
int Errorbuf::overflow(int ch)
#endif
{
#ifdef DEBUG
#ifdef INHERIT_ERRORBUF
	TraceBack tb( "ErrorStream::overflow()" );
#else
	TraceBack tb( "Errorbuf::overflow()" );
#endif
#endif

	if ( allocate() == EOF )
			return EOF;

	if ( !pptr() ) setp( base(), base() );

	int w = pptr() - pbase();

	if ( ch != EOF )
	{

		*pptr() = ch;
		pbump(1);
		++w;
	}


#ifdef INHERIT_ERRORBUF	
	if (   write_buf(pbase(), w, ch) )	// ok
#else
	assert( ehp );

	if ( ehp && ehp->write_buf(pbase(), w, ch) )	// ok
#endif
	{
		setp( pbase(), ebuf() - 1);
		return 0;
	}
	else			// error
	{
		setp( 0, 0 );
		return EOF;
	}

}

ErrorStream::ErrorStream(const char* title, ostream& user_os) : errstatus(0),
#ifdef INHERIT_ERRORBUF
		ios( (streambuf*)this),
#else
		ios(&buffr),buffr(this),
#endif
                 my_os(user_os), errcount(0)
{
#ifdef DEBUG
	TraceBack tb( "ErrorStream::ErrorStream(char*,ostream&)" );
#endif
	prog = new char[ strlen( title ) + 1 ];
	strcpy(prog, title);

}

ErrorStream::ErrorStream(const ErrorStream& er) : 
#ifdef INHERIT_ERRORBUF
		ios( (streambuf*)this),
#else
		ios(&buffr),buffr(this),
#endif
                 my_os(er.my_os)
{
#ifdef DEBUG
	TraceBack tb( "ErrorStream::ErrorStream(ErrorStream&)" );
#endif

	prog = new char[ strlen( er.prog ) + 1 ];
	strcpy(prog, er.prog);

	errstatus = er.status();
	errcount  = er.count();
}

ErrorStream::~ErrorStream()
{
#ifdef DEBUG
	TraceBack tb( "ErrorStream::~ErrorStream()" );
#endif
	 close();
	 if ( prog ) delete []prog;

	 prog = NULL;
}


// write out the error line
int ErrorStream::write_buf(const char* s,const int len, const int eof)
{

	static bool fatal_error = false;

	for (int k = 0; k < len; ++k)
        {
        	switch ( s[k] )
                {
                	case SOH:	// start of a message
				if ( prog )
					my_os << prog;
                		break;
                        case WARN:
                        	my_os << " Warning (" << errstatus
                                      << "): ";
                                fatal_error = false;
                                break;
                        case FAIL:
                        	my_os << " Fail (" << errstatus
                                      << "): ";
                                fatal_error = false;
                                break;
                        case FATAL:
                        	my_os << " Fatal Error (" << errstatus
                                	<< "): ";
                                fatal_error = true;
                                break;
			 case SILENT:
				return 1;
                         default:
				my_os << s[k];
                                break;
                  }
	}
        

	if ( my_os.fail() )
        	return 0;


	if ( eof == EOF )
        {
        	my_os.flush();
		if ( fatal_error )
                	exit( errstatus );
        }
                        
	return 1;	// return 1 for OK
}

void ErrorStream::close()
{
#ifdef DEBUG
	TraceBack tb( "ErrorStream::close()" );
#endif
#ifdef INHERIT_ERRORBUF
			sync();
#else
			buffr.sync();                                  
#endif
}          


// a nonmember function, mostly so I remember how to do a manipulator
ostream& terminate(ostream& os)
{
	os.flush();
	exit(1);

	return os;		// keeps the compiler happy
}

// increment the error status (post-increment)
#ifndef __ATT2__
#ifndef __GNUC__
ErrorStream ErrorStream::operator++(int)		   // DOES NOT WORK WITH GCC
{
	 ErrorStream retval(*this);
	 errstatus++;
         return retval;
}
#endif
#endif

void ErrorStream::nomore()
{
	flush();
	(ostream )*this << SOH << WARN
	      << "maximum number of warning messages (" << elast-1
	      << ") exceeded,\n\t\tno more warning messages will be issued"
	      << endl << SILENT;
}

void ErrorStream::warning(const char *msg)
{

	if ( (msgnum >= efirst) && (msgnum < elast) )
	{
        	*this << SOH << WARN;

        	if ( msg )
        		*this << msg;

	}
	else if ( msgnum == elast )
			nomore();
	else
	    *this << SILENT;

	msgnum++;
	errcount++;
}

void ErrorStream::fail(const char *msg)
{

	if ( (msgnum >= efirst) && (msgnum < elast) )
	{
        	*this << SOH << FAIL;

        	if ( msg )
        		*this << msg;

	}
	else if ( msgnum == elast )
			nomore();
	else
	    *this << SILENT;

	msgnum++;
	errcount++;
}

void ErrorStream::fatal(const char *msg)
{
	*this << SOH << FATAL;

        if ( msg )
        {
        	*this << msg;
		flush();
        }
}

void ErrorStream::memory(const void *p)
{
	if ( p == (void *)NULL )
		*this << SOH << FATAL << " Memory allocation error" << endl;

}


void ErrorStream::warning(const int eval, const char *msg)
{

	if ( (msgnum >= efirst) && (msgnum < elast) )
	{
		*this = eval;
        	*this << SOH << WARN;

        	if ( msg )
        		*this << msg;

	}
	else if ( msgnum == elast )
			nomore();
	else
	    *this << SILENT;

	msgnum++;
	errcount++;

}

void ErrorStream::fail(const int eval, const char *msg)
{

	if ( (msgnum >= efirst) && (msgnum < elast) )
	{
		*this = eval;
        	*this << SOH << FAIL;

        	if ( msg )
        		*this << msg;

	}
	else if ( msgnum == elast )
			nomore();
	else
	    *this << SILENT;

	msgnum++;
	errcount++;

}

void ErrorStream::fatal(const int eval, const char *msg)
{
	*this = eval;
	*this << SOH << FATAL;

        if ( msg )
        {
        	*this << msg;
		flush();
        }
}

void ErrorStream::memory(const int eval, const void *p)
{
	if ( p == (void *)NULL )
        {
		*this = eval;
		*this << SOH << FATAL << " Memory allocation error" << endl;
	}
}

void Warning(ErrorStream& ehr, const int eval)
{
	ehr.warning( eval );
}

void Fail(ErrorStream& ehr, const int eval)
{
	ehr.fail( eval );
}

void Fatal(ErrorStream& ehr, const int eval)
{
	ehr.fatal( eval );
}

ErrorStream& ErrorStream::operator=(const int err)
{
	flush();
	errstatus = err;
        return *this;
}

ErrorStream& ErrorStream::operator|=(const int err)
{
	flush();
	errstatus |= err;
        return *this;
}

