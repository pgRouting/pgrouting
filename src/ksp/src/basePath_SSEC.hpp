#ifndef SRC_PGR_PATH_T_H
#define SRC_PGR_PATH_T_H

#include <deque>
#include "pgr_types.h"

class Path {
 public:
    std::deque< pgr_path_t > path;
    float8 cost;

    Path(): cost(0)  {}

    void push_back(pgr_path_t data) {
        path.push_back(data);
        cost += data.cost;
    }

    void push_front(int d_seq, int64_t d_source, int64_t d_edge, float8 d_cost) {
      pgr_path_t data;
      data.seq = d_seq;
      data.source = d_source;
      data.edge = d_edge;
      data.cost = d_cost;
      cost += data.cost;
      path.push_front(data);
    }

    void push_back(int d_seq, int64_t d_source, int64_t d_edge, float8 d_cost) {
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

    void print_path() {
       for (unsigned int i = 0; i < path.size(); ++i)
          std::cout << path[i].seq << "\t" << path[i].source << "\t" << path[i].edge << "\t" << path[i].cost << "\n";
    }



    Path  getSubpath(unsigned int j) {
        Path  result;
        if (j == 0)  return result;
        std::deque<pgr_path_t>::iterator i; 
        for (i = path.begin(); i != path.begin() + j; i++ )
            result.push_back( (*i) );
        return result;
   }


   bool isEqual(const Path &subpath) const{
       if (subpath.path.empty()) return true;
       if (subpath.path.size() >= path.size()) return false;
       std::deque<pgr_path_t>::const_iterator i, j; 
       for (i = path.begin(), j = subpath.path.begin();
            j != subpath.path.end() ;
            ++i, ++j )
            if ((*i).source !=  (*j).source) return false;
       return true;
   }

   void appendPath(const Path &o_path){
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


};
#endif  // SRC_PGR_PATH_T_H
