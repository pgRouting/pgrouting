/*PGR-MIT******************************************************************
 *
 * file pgr_assert.cpp
 *
 * Copyright 2014 Stephen Woodbridge <woodbri@imaptools.com>
 * Copyright 2014 Vicky Vergara <vicky_vergara@hotmail.com>
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the MIT License. Please file MIT-LICENSE for details.
 *
 *****************************************************************PGR-MIT*/
#include "./pgr_assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <execinfo.h>
#include <string>
#include <exception>

#if 0
#ifdef assert
#undef assert
#endif

#ifndef __STRING
#define __STRING(x) #x
#endif

#define __TOSTRING(x) __STRING(x)
#endif

std::string get_backtrace() {
        void *trace[16];
        int i, trace_size = 0;

        trace_size = backtrace(trace, 16);
        char** funcNames = backtrace_symbols( trace, trace_size );


        std::string message = "\n*** Execution path***\n";
        for (i = 0; i < trace_size; ++i) {
            message += "[bt]" + static_cast<std::string>(funcNames[i]) + "\n";
        }

        free( funcNames );
        return message;
}




const char* AssertFailedException::what() const throw() {
    return str.c_str();
}

AssertFailedException::AssertFailedException(std::string msg) :
    str(msg) {}

