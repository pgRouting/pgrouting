// standio.cpp		Implementation of Stdin and Stdout

//                     (c) Copyright 1995, Everett F. Carter Jr.
//                     Permission is granted by the author to use
//		       this software for any application provided this
//		       copyright notice is preserved.

static const char rcsid[] = "$Id$   EFC";

#include "standio.hpp"

#ifdef LOCAL_FILEBUF
Stdin::Stdin(int *argc, char **argv, int mode) : ios(&InputFile), fname(NULL), ifstream()
#else
Stdin::Stdin(int *argc, char **argv, int mode) :  fname(NULL), ifstream()
#endif
{
        open_input(argc, argv, mode);
}

#ifdef LOCAL_FILEBUF
Stdin::Stdin(char *infile, int mode) : ios(&InputFile), fname(NULL), ifstream()
#else
Stdin::Stdin(char *infile, int mode) :  fname(NULL), ifstream()
#endif
{
	if ( infile )
        	do_open_input(infile, mode);
        else		// no file named, so attach to STANDARD IN
        {
#ifdef LOCAL_FILEBUF
		InputFile.attach( 0 );
#endif
#ifdef __GNUC__
		rdbuf()->attach(0);
#else
                attach(0);
#endif
	}

}


const char* Stdin::name() const
{ 
 	if (fname) return fname;
        else	return "stdin";
}

int Stdin::do_open_input(char* infile, int mode)
{
#ifdef LOCAL_FILEBUF
		if ( InputFile.open(infile, ios::in | mode) == (filebuf *)NULL)
#else
		open( infile, ios::in | mode);
		if ( rdstate() )
#endif                
		{
			return -1; 	// an error occurred
		}

#ifdef LOCAL_FILEBUF
		attach( InputFile.fd() );
#endif
        	fname = infile;
                
		return 0;
}


// if argv[1] exists and does not start with '-',
// consider it an input file name
// reduce argc and shift *argv over after a successful open
void Stdin::open_input(int *argc, char **argv, int mode)
{

	if ( *argc >= 2 && *argv[1] != '-')
	{

		if ( do_open_input( argv[1], mode ) )
                		return;		// an error occurred

		// at this point shuffle all the args over one to the left
		for (int i = 0; i < *argc; i++)
			argv[i] = argv[i + 1];

		(*argc)--;
                
	}
        else		// no file named, so attach to STANDARD IN
        {
#ifdef LOCAL_FILEBUF
		InputFile.attach( 0 );
#endif
#ifdef __GNUC__
		rdbuf()->attach(0);
#else
                attach(0);
#endif

	}
                        
	return;			// normal return
}

// ===================================================================

#ifdef LOCAL_FILEBUF
Stdout::Stdout(int *argc, char **argv, int mode) : ios(&OutputFile), fname(NULL), ofstream()
#else
Stdout::Stdout(int *argc, char **argv, int mode) : fname(NULL), ofstream()
#endif
{
//  cerr << "Stdout::Stdout(int*, char**, int)\n";

	open_output(argc, argv, mode);
}

#ifdef LOCAL_FILEBUF
Stdout::Stdout(char* outfile, int mode) : ios(&OutputFile), fname(NULL), ofstream()
#else
Stdout::Stdout(char* outfile, int mode) : fname(NULL), ofstream()
#endif
{
//  cerr << "Stdout::Stdout(char*, int)\n";

	if ( outfile )
		do_open_output( outfile, mode );
	else		// no file named, so attach to STANDARD OUT
        {
#ifdef LOCAL_FILEBUF
		OutputFile.attach(1);
#endif
#ifdef __GNUC__
		rdbuf()->attach(1);
#else
                attach(1);
#endif
	}
}

const char* Stdout::name() const
{
	 if (fname) return fname;
         else	return "stdout";
}

int Stdout::do_open_output(char* outfile, int mode)
{
//  cerr << "Stdout::do_open_output(...)\n";

#ifdef LOCAL_FILEBUF
		if ( OutputFile.open( outfile, ios::out | mode) == (filebuf *)NULL)
#else
                open( outfile, ios::out | mode);

		if ( rdstate() )
#endif
		{
			return -1;		// an error occurred
		}

#ifdef LOCAL_FILEBUF
		attach( OutputFile.fd() );
#endif
        	fname = outfile;
                

		return 0;
}

// if argv[1] exists and does not start with '-',
// consider it an output file name
// reduce argc and shift *argv over after a successful open
void Stdout::open_output(int *argc, char **argv, int mode)
{
//  cerr << "Stdout::open_output(...)\n";

	if ( *argc >= 2 && *argv[1] != '-')
	{
		if ( do_open_output( argv[1], mode ) )
			return;		// an error occurred

		// at this point shuffle all the args over one to the left
		for (int i = 0; i < (*argc-1); i++)
			argv[i] = argv[i + 1];

		(*argc)--;
	}
	else		// no file named, so attach to STANDARD OUT
        {
#ifdef LOCAL_FILEBUF
		OutputFile.attach(1);
#endif
#ifdef __GNUC__
		rdbuf()->attach(1);
#else
                attach(1);
#endif

	}
        
	return;				// normal return
}
