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
// #include <fstream>
#include "postgres.h"
#include "./pgr_types.h"

class Path {
 public:
    std::deque< pgr_path_t > path;
    float8 cost;

    Path(): cost(0) {}

    void push_back(pgr_path_t data) {
        path.push_back(data);
        cost += data.cost;
    }

    void push_front(int d_seq, int64_t d_source,
                    int64_t d_edge, float8 d_cost) {
      pgr_path_t data;
      data.seq = d_seq;
      data.source = d_source;
      data.edge = d_edge;
      data.cost = d_cost;
      cost += data.cost;
      path.push_front(data);
    }

    void push_back(int d_seq, int64_t d_source,
                   int64_t d_edge, float8 d_cost) {
      pgr_path_t data;
      data.seq = d_seq;
      data.source = d_source;
      data.edge = d_edge;
      data.cost = d_cost;
      cost += data.cost;
      path.push_back(data);
    }

    void clear() {
        path.clear();
        cost = 0;
    }

    void print_path(std::ostream& log) {
       for (unsigned int i = 0; i < path.size(); ++i)
          log << path[i].seq << "\t" << path[i].source << "\t"
              << path[i].edge << "\t" << path[i].cost << "\n";
    }

    void print_path() {
       print_path(std::cout);
    }


    Path  getSubpath(unsigned int j) {
        Path  result;
        if (j == 0)  return result;
        std::deque<pgr_path_t>::iterator i;
        for (i = path.begin(); i != path.begin() + j; i++)
            result.push_back((*i));
        return result;
    }

    bool isEqual(const Path &subpath) const {
        if (subpath.path.empty()) return true;
        if (subpath.path.size() >= path.size()) return false;
        std::deque<pgr_path_t>::const_iterator i, j;
        for (i = path.begin(), j = subpath.path.begin();
             j != subpath.path.end();
             ++i, ++j)
             if ((*i).source !=  (*j).source) return false;
        return true;
    }

    void appendPath(const Path &o_path) {
        path.insert(path.end(), o_path.path.begin(), o_path.path.end());
        cost +=  o_path.cost;
    }

    void empty_path(unsigned int d_source) {
        pgr_path_t data;
        data.seq = 1;
        data.source = d_source;
        data.edge = -1;
        data.cost = 0;
        path.push_back(data);
    }

 void dpPrint(
        pgr_path_element3_t **ret_path,
        int &sequence, int route_id) const {
        // the row data:  seq, route, nodeid, edgeId, cost
    int64_t nodeId, edgeId;
    double cost;

    for (unsigned int i = 0; i < path.size(); i++) {
      edgeId = path[i].edge;
      nodeId = path[i].source;
      cost = path[i].cost;

      (*ret_path)[sequence].route_id = route_id;
      (*ret_path)[sequence].vertex_id = nodeId;
      (*ret_path)[sequence].edge_id = edgeId;
      (*ret_path)[sequence].cost = cost;
      sequence++;
    }
  }

};
#endif  // SRC_COMMON_SRC_BASE_PATH_SSCE_H_
