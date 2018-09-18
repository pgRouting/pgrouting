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

/*! @file */

#ifndef INCLUDE_CPP_COMMON_PGR_ALLOC_HPP_
#define INCLUDE_CPP_COMMON_PGR_ALLOC_HPP_
#pragma once


#include <string>
#include <cstring>


extern "C" {

extern
void* SPI_palloc(size_t size);

extern void *
SPI_repalloc(void *pointer, size_t size);

extern void
SPI_pfree(void *pointer);

}


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
pgr_alloc(std::size_t size, T* ptr) {
    if (!ptr) {
        ptr = static_cast<T*>(SPI_palloc(size * sizeof(T)));
    } else {
        ptr = static_cast<T*>(SPI_repalloc(ptr, size * sizeof(T)));
    }
    return ptr;
}

template <typename T>
T*
pgr_free(T* ptr) {
    if (ptr) {
        SPI_pfree(ptr);
    }
    return nullptr;
}

char *
pgr_msg(const std::string &msg);

#endif  // INCLUDE_CPP_COMMON_PGR_ALLOC_HPP_
