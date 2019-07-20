/*PGR-GNU*****************************************************************

FILE: pgr_assert.cpp

Copyright 2015~  Vicky Vergara <vicky_vergara@hotmail.com>
Copyright 2014 Stephen Woodbridge <woodbri@imaptools.com>
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/
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

