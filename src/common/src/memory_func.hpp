/*PGR-GNU*****************************************************************
File: memory_func.hpp

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

template <typename T>
T*
get_memory(std::size_t size, T *ptr){
    if( !ptr ){
        ptr = (T*) malloc(size * sizeof(T));
    } else {
        ptr = (T*) realloc( ptr, size * sizeof(T));
    }
    return (T*) ptr;
}

template <typename T>
T*
noResult(std::size_t *count, T *ptr) {
    (*count) = 0;
    if (ptr)
        free(ptr);
    return NULL;
}

