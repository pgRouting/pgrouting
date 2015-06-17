/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

#ifndef SRC_COMMON_SRC_BASE_PATH_SSCE_H_
#define SRC_COMMON_SRC_BASE_PATH_SSCE_H_

#include <deque>
#include <iostream>
#include <algorithm>
// #include <fstream>
#include "postgres.h"
#include "./pgr_types.h"

class Path {
 public:
    std::deque< pgr_path_element3_t > path;
    float8 cost;

    Path(): cost(0) {}

    void push_front(pgr_path_element3_t data) {
        cost += data.cost;
        path.push_back(data);
    }

    void push_back(pgr_path_element3_t data) {
        cost += data.cost;
        path.push_back(data);
    }

    pgr_path_element3_t set_data(
         int d_seq, 
         int64_t d_from, 
         int64_t d_to,
         int64_t d_vertex,
         int64_t d_edge, 
         float8 d_cost,
         float8 d_tot_cost) {
      pgr_path_element3_t data;
      data.seq = d_seq;
      data.from = d_from;
      data.to = d_to;
      data.vertex = d_vertex;
      data.edge = d_edge;
      data.cost = d_cost;
      data.tot_cost = d_tot_cost;
      return data;
    }

    void push_front(
         int d_seq, 
         int64_t d_from, 
         int64_t d_to,
         int64_t d_vertex,
         int64_t d_edge, 
         float8 d_cost,
         float8 d_tot_cost) {
      path.push_front(set_data(d_seq, d_from, d_to, d_vertex, d_edge, d_cost, d_tot_cost));
      cost += path[0].cost;
    }

    void push_back(
         int d_seq, 
         int64_t d_from, 
         int64_t d_to,
         int64_t d_vertex,
         int64_t d_edge, 
         float8 d_cost,
         float8 d_tot_cost) {
      path.push_back(set_data(d_seq, d_from, d_to, d_vertex, d_edge, d_cost, d_tot_cost));
      cost += path[path.size() - 1].cost;
    }

    void clear() {
        path.clear();
        cost = 0;
    }

    void print_path(std::ostream& log) const {
       log << "seq\tfrom\tto\tvertex\tedge\tcost\ttot_Cost\n";
       for (unsigned int i = 0; i < path.size(); ++i)
          log << path[i].seq << "\t"
              << path[i].from << "\t"
              << path[i].to << "\t"
              << path[i].vertex << "\t"
              << path[i].edge << "\t"
              << path[i].cost << "\t"
              << path[i].tot_cost << "\n";
    }

    void print_path() const {
       print_path(std::cout);
    }


    Path  getSubpath(unsigned int j) const {
        Path  result;
        if (j == 0)  return result;
        for (auto i = path.begin(); i != path.begin() + j; ++i) result.push_back((*i));
        return result;
    }


    bool isEqual(const Path &subpath) const {
        if (subpath.path.empty()) return true;
        if (subpath.path.size() >= path.size()) return false;
        std::deque<pgr_path_element3_t>::const_iterator i, j;
        for (i = path.begin(),  j = subpath.path.begin();
             j != subpath.path.end();
             ++i, ++j)
             if ((*i).vertex !=  (*j).vertex) return false;
        return true;
    }

    void appendPath(const Path &o_path) {
        path.insert(path.end(), o_path.path.begin(), o_path.path.end());
        cost +=  o_path.cost;
    }

    void empty_path(unsigned int d_vertex) {
        path.push_back(set_data(1, d_vertex, d_vertex, d_vertex, -1, 0, 0));
    }

 void dpPrint(
        pgr_path_element3_t **ret_path,
        int &sequence) const {

    for (unsigned int i = 0; i < path.size(); i++) {
      (*ret_path)[sequence] = path[i];
      sequence++;
    }
  }

   void dpPrint(
        pgr_path_element3_t **ret_path,
        int &sequence, int routeId) const {

    for (unsigned int i = 0; i < path.size(); i++) {
      (*ret_path)[sequence] = path[i];
      (*ret_path)[sequence].from = (uint64_t)routeId;
      sequence++;
    }
  }


/* delete it because it belongs to the other one
                        THE OPUTPUT
seq     from    node    edge    cost
0       12      1       1       5 *
1       12      2       2       4 *
2       12      3       3       3 
3       12      4       16      2
4       12      5       8       3 *
5       12      6       9       2
6       12      11      11      3
7       12      10      10      4 *
8       12      12      -1      0
9       12      13      14      5 *
10      12      9       15      1
11      12      7       6       5 *
12      12      8       7       4 *

0       7       1       1       4
1       7       2       4       3
2       7       3       3       6 *
3       7       4       16      5 *
4       7       5       7       2 
5       7       6       8       3 *
6       7       11      12      4 *
7       7       10      10      3
8       7       12      15      5 *
9       7       13      14      4 
10      7       9       9       4 *
11      7       7       -1      0
12      7       8       6       1

                        THE OPUTPUT
seq     from    node    edge    cost
2       12      3       3       3 
3       12      4       16      2
5       12      6       9       2
6       12      11      11      3
8       12      12      -1      0
10      12      9       15      1

0       7       1       1       4
1       7       2       4       3
4       7       5       7       2 
7       7       10      10      3
9       7       13      14      4 
11      7       7       -1      0
12      7       8       6       1

                        THE OPUTPUT
seq     from    node    edge    cost

0       7       1       1       4
1       7       2       4       3
2       12      3       3       3 
3       12      4       16      2
4       7       5       7       2 
5       12      6       9       2
11      7       7       -1      0
12      7       8       6       1
10      12      9       15      1
7       7       10      10      3
6       12      11      11      3
8       12      12      -1      0
9       7       13      14      4 

  // when they are equal choose one
  // ordenar por numero de nodo
*/
  
  friend Path equi_cost(const Path &p1, const Path &p2) {
    Path result(p1);
    sort(result.path.begin(), result.path.end(), 
      [](const pgr_path_element3_t &e1, const pgr_path_element3_t &e2)->bool { 
         return e1.vertex < e2.vertex; 
      });

    for (auto const &e : p2.path) {
      auto pos = find_if(result.path.begin(), result.path.end(),
                 [&e](const pgr_path_element3_t &e1)->bool { 
                   return e.vertex == e1.vertex; 
                 });
      if (pos != result.path.end()) {
        if (pos->cost > e.cost) {
           (*pos) = e;
        }  
      } else {
        result.push_back(e);
      }
    }
    return result;
  }

  friend Path equi_cost(const std::deque< Path > &paths) {
    Path result;
    for (const auto &p1 : paths) {
      result = equi_cost(result, p1);
    }
    return result;
  }
};


#endif  // SRC_COMMON_SRC_BASE_PATH_SSCE_H_
