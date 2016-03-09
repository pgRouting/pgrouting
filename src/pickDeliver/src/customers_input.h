/*PGR-GNU*****************************************************************
File: edges_input.h

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#include "./pgr_types.h"
#include "./pdp.h"


/*!
  int64_t id;
  double x;
  double y;
  double demand;
  double Etime;
  double Ltime;
  double Stime;
  int64_t Pindex;
  int64_t Dindex;
  double Ddist;
*/
void pgr_get_customers(
        char *sql,
        Customer **customers,
        size_t *total_customers);
