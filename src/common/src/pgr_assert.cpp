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
#include "cpp_common/pgr_assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef __GLIBC__
#include <execinfo.h>
#endif

#include <string>
#include <exception>


std::string get_backtrace() {
#ifdef __GLIBC__
        void *trace[16];
        int i, trace_size = 0;

        trace_size = backtrace(trace, 16);
        char** funcNames = backtrace_symbols(trace, trace_size);


        std::string message = "\n*** Execution path***\n";
        for (i = 0; i < trace_size; ++i) {
            message += "[bt]" + static_cast<std::string>(funcNames[i]) + "\n";
        }

        free(funcNames);
        return message;
#else
        return "";
#endif
}

std::string get_backtrace(const std::string &msg) {
    return std::string("\n") + msg + "\n" + get_backtrace();
}



const char* AssertFailedException::what() const throw() {
    return str.c_str();
}

AssertFailedException::AssertFailedException(std::string msg) :
    str(msg) {}

