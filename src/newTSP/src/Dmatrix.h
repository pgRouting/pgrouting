
#pragma once


#include "../../common/src/pgr_types.h"


#include <iostream>
#include <vector>


namespace pgRouting {
namespace tsp {

class Tour;  // for tourCost

class Dmatrix {
 public:
    typedef std::vector < std::vector < double > > Costs;

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

    double tourCost(const Tour &tour) const;
    double pathCost(const std::vector<size_t> &path) const;
    const std::vector< double >& operator[] (size_t i) const {return costs[i];}
    std::vector< double >& operator[] (size_t i) {return costs[i];}
    
    friend std::ostream& operator<<(std::ostream &log, const Dmatrix &matrix);
    
    Costs costs;
    std::vector<size_t> ids;
};

}  // namespace tsp
}  // namespace pgRouting
