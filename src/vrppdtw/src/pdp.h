/*PGR-GNU*****************************************************************

Copyright (c) 2014 Manikata Kondeti
mani.iiit123@gmail.com

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

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "postgres.h"

#pragma once

// For C and C++ inclusion

typedef struct {
        int id;
        int x;
        int y;
        int demand;
        int Etime;
        int Ltime;
        int Stime;
        int Pindex;
        int Dindex;
        double Ddist;
        int P;
        int D;
} Customer;

typedef struct  {
         int seq;
         int rid;
         int nid;
         int nseq;
         int cost;
} path_element;



#ifdef __cplusplus
extern "C"
#endif
int Solver(Customer *c,
        int total_tuples,
        int vehicle_count,
        int capacity ,
        char **msg,
        path_element **results,
        int *length_results);

#ifdef __cplusplus
extern "C"
#endif

