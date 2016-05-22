
#pragma once


#include "../../common/src/pgr_types.h"


#include <iostream>
#include <vector>


namespace pgRouting {
namespace tsp {

class Tour;  // for tourCost

class eucledianDmatrix {
 public:

    eucledianDmatrix() = default;
    eucledianDmatrix(const std::vector< Coordinate_t > &data_coordinates);

    bool has_no_infinity() const;
    bool obeys_triangle_inequality() const;
    bool is_symetric() const;

    void set(size_t i, size_t j, double dist) {row = i; column = j; special_distance = dist;};

    size_t get_index(int64_t id) const;
    int64_t get_id (size_t id) const;
    size_t size() const {return ids.size();};

    double tourCost(const Tour &tour) const;

    const std::vector<double> get_row(size_t i) const;

    double comparable_distance(size_t i, size_t j) const; 
    double distance(size_t i, size_t j) const; 
    
    friend std::ostream& operator<<(std::ostream &log, const eucledianDmatrix &matrix);
    
 protected:
    void set_ids();
    std::vector<size_t> ids;

 private:
    std::vector < Coordinate_t >  coordinates;
    size_t row;
    size_t column;
    double special_distance;

};

}  // namespace tsp
}  // namespace pgRouting
