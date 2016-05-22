#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <algorithm>
#include <vector>
#include "../../common/src/pgr_assert.h"

#include "./eucledianDmatrix.h"
#include "./tour.h"

namespace pgRouting {
namespace tsp {


const std::vector<double>
eucledianDmatrix::get_row(size_t i) const {
    std::vector<double> result;
    size_t j(0);
    for (auto coordinate : coordinates) {
        if (i == j) result.push_back(0);
        else result.push_back(distance(i, j));
    }
    if (special_distance >= 0) {
       if(row == i && column == j) {
           result[j] = special_distance;
       } else {
           result[i] = special_distance;
       }
    }   
    return result;
};


double
eucledianDmatrix::comparable_distance(size_t i, size_t j) const {
    if (special_distance >= 0 &&
            ((row == i && column == j)
             ||(row == j && column == i))) {
        return special_distance * special_distance;
    }
    auto dx = coordinates[i].x - coordinates[j].x;
    auto dy = coordinates[i].y - coordinates[j].y;
    return dx * dx + dy * dy;
}

double
eucledianDmatrix::distance(size_t i, size_t j) const {
    if (special_distance >= 0 &&
            ((row == i && column == j)
             ||(row == j && column == i))) {
        return special_distance;
    }
    return std::sqrt(comparable_distance(i, j));
}

double
eucledianDmatrix::tourCost(const Tour &tour) const {
    double total_cost(0);
    if (tour.cities.empty()) return total_cost;

    auto prev_id = tour.cities.front();
    for (const auto &id : tour.cities) {
        if (id == tour.cities.front()) continue;

        total_cost += distance(prev_id, id);
        prev_id = id;
    }
    total_cost += distance(prev_id, tour.cities.front());
    return total_cost;
}



size_t
eucledianDmatrix::get_index(int64_t id) const {
    auto pos = std::lower_bound(ids.begin(), ids.end(), id);
    return pos - ids.begin();
}

int64_t
eucledianDmatrix::get_id(size_t id) const{
    return ids[id];
}

/*
 * constructor
 */
eucledianDmatrix::eucledianDmatrix(const std::vector < Coordinate_t > &data_coordinates)
    : coordinates(data_coordinates) {
        set_ids();
        std::sort(coordinates.begin(), coordinates.end(),
                [](const Coordinate_t &lhs, const Coordinate_t &rhs)
                {return lhs.id < rhs.id;});
    }

void
eucledianDmatrix::set_ids() {
    ids.reserve(coordinates.size());
    for (const auto &data : coordinates) {
        ids.push_back(data.id);
    }
    std::sort(ids.begin(), ids.end());
    auto total = ids.size();
    ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
    pgassertwm(total == ids.size(), "Duplicated id found");
}

bool
eucledianDmatrix::has_no_infinity() const {
    return true;
}


bool
eucledianDmatrix::obeys_triangle_inequality() const {
    return true;
}

bool
eucledianDmatrix::is_symetric() const{
    return true;
}


std::ostream& operator<<(std::ostream &log, const eucledianDmatrix &matrix){
    for (const auto id : matrix.ids) {
        log << "\t" << id;
    }
    log << "\n";
    for (const auto row : matrix.coordinates) {
        log << row.id << "(" << row.x << "," << row.y << ")\n";
    }
    return log;
}


}  // namespace tsp
}  // namespace pgRouting
