/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2020 Mohamed Bakli, Esteban Zimányi, Mahmoud Sakr
mohamed_bakli@ulb.ac.be, estebanzimanyi@gmail.com, m_attia_sakr@yahoo.com

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

#ifndef INCLUDE_CPP_COMMON_INTERRUPTION_H_
#define INCLUDE_CPP_COMMON_INTERRUPTION_H_
/*
 * Suppress the -Wpedantic warning temporarily about the postgres file
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C" {
#include <postgres.h>
#include <miscadmin.h>
}
#pragma GCC diagnostic pop
#endif  // INCLUDE_CPP_COMMON_INTERRUPTION_H_
