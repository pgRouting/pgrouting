/*PGR-GNU*****************************************************************
File: pgr_palloc.hpp

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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
#pragma once
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#ifdef open
#undef open
#endif
#endif

#include <stdlib.h>

/*! \fn pgr_alloc(std::size_t size, T *ptr)
 
\brief allocates memory

- Does a malloc or realloc depending on the ptr value
- To be used only on C++ code
- To be used when returning results to postgres
- free must occur in the C code

\param[in] size
\param[in] ptr
\returns pointer to the first byte of allocated space

 */

template <typename T>
T*
pgr_alloc(std::size_t size, T *ptr) {
    if (!ptr) {
        ptr = (T*) malloc(size * sizeof(T));
    } else {
        ptr = (T*) realloc(ptr, size * sizeof(T));
    }
    return (T*) ptr;
}
