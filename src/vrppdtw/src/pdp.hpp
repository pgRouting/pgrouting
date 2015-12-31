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

#pragma once

#include <math.h>
#include <vector>

// Exclusive for c++ inclusion

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
} Depot;


typedef struct {
        int id;
        int Pid;
        double Ddist;
        int Did;
        int checked;
} Pickup;



// It is used to save some variables and helps if we need to revisit
// previous state.

typedef struct {
        int twv;
        int cv;
        int dis;
        std::vector< int > path;
        std::vector< int > order;
        int path_length;
} State;

template <class T1, class T2>
double
CalculateDistance(const T1 &from, const T2 &to) {
        return sqrt((from.x - to.x) * (from.x - to.x)
                + (from.y - to.y) * (from.y - to.y));
}
