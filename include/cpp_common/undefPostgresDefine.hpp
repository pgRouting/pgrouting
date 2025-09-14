/*PGR-GNU*****************************************************************
File: undefPostgresDefine.hpp

Copyright (c) 2024 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2024 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

#ifndef INCLUDE_CPP_COMMON_UNDEFPOSTGRESDEFINE_HPP_
#define INCLUDE_CPP_COMMON_UNDEFPOSTGRESDEFINE_HPP_

/** @file undefPostgresDefine.hpp

https://doxygen.postgresql.org/port_8h.html
Has the line
`#define    snprintf   pg_snprintf`

Being `snprintf` part of [stdio.h](https://en.cppreference.com/w/cpp/header/cstdio) since C++11
Work around is to undef it.

It's affecting boost 83+

Placing after including extern C postgres files
Example
~~~ c
extern "C" {
#include <postgres.h>
#include <utils/array.h>
}

#include <system files>

#include "cpp_common/undefPostgresDefine.hpp"
~~~

*/
#ifdef __cplusplus
#ifdef sprintf
#undef sprintf
#endif

#ifdef snprintf
#undef snprintf
#endif

#ifdef vsprintf
#undef vsprintf
#endif

#ifdef vsnprintf
#undef vsnprintf
#endif

#ifdef unlink
#undef unlink
#endif

#ifdef bind
#undef bind
#endif

#endif

#endif  // INCLUDE_CPP_COMMON_UNDEFPOSTGRESDEFINE_HPP_
