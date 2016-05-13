
#pragma once


#include "../../common/src/pgr_types.h"


#include <vector>



class Dmatrix {
 public:
    typedef std::vector < std::vector < double > > Costs;
    typedef std::vector < int64_t > Ids;

    Dmatrix() = default;
    Dmatrix(const std::vector < Matrix_cell_t > &data_costs);

    bool has_no_infinity() const;

    bool obeys_triangle_inequality() const;
    bool is_symetric() const;

    Dmatrix get_symetric() const; 

    void set_ids(const std::vector<matrix_cell> &data_costs);

    size_t get_index(int64_t id) const;
    int64_t get_id (size_t id) const;
    size_t size() const {return ids.size();};
    double max() const;
    double pathCost(const Ids &path) const;
    std::vector< double >& operator[] (size_t i) {return costs[i];}

    Costs costs;
    Ids ids;
};

