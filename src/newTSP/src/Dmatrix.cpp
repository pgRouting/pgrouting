#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <algorithm>
#include <vector>
#include "../../common/src/pgr_assert.h"

#include "./Dmatrix.h"
#include "./tour.h"

namespace pgRouting {
namespace tsp {

double
Dmatrix::tourCost(const Tour &tour) const {
    // return pathCost(tour.cities);

    double total_cost(0);
    if (tour.cities.empty()) return total_cost;

    auto prev_id = tour.cities.front();
    for (const auto &id : tour.cities) {
        if (id == tour.cities.front()) continue;

        pgassert(costs[prev_id][id] != std::numeric_limits<double>::max());
#if 0
        if (costs[prev_id][id] == std::numeric_limits<double>::max())
            return std::numeric_limits<double>::max();
#endif

        total_cost += costs[prev_id][id];
        prev_id = id;
    }
    total_cost += costs[prev_id][tour.cities.front()];
    return total_cost;
}


double
Dmatrix::pathCost(const std::vector<size_t>  &path) const {
    double len = 0;
    if (path.empty()) return len;
    auto prev_id = path.front();
    for (const auto &id : path) {
        if (id == path.front()) continue;
        pgassert(costs[prev_id][id] != std::numeric_limits<double>::max());
#if 0
        if (costs[prev_id][id] == std::numeric_limits<double>::max())
            return std::numeric_limits<double>::max();
#endif
        len += costs[prev_id][id];
        prev_id = id;
    }
    len += costs[prev_id][path.front()];
    return len;
}



double
Dmatrix::max() const {
    double maxd(0);
    for (const auto &row : costs) {
        auto row_max = std::max_element(row.begin(),row.end());
        maxd = maxd < *row_max? *row_max : maxd;
    }
    return maxd;
}

void
Dmatrix::set_ids(const std::vector < Matrix_cell_t > &data_costs) {
    ids.reserve(data_costs.size() * 2);
    for (const auto &cost : data_costs) {
        ids.push_back(cost.from_vid);
        ids.push_back(cost.to_vid);
    }
    std::sort(ids.begin(), ids.end());
    ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
    /*
     * freeing up unused space
     */
    ids.shrink_to_fit();
}

size_t
Dmatrix::get_index(int64_t id) const {
    auto pos = std::lower_bound(ids.begin(), ids.end(), id);
    return pos - ids.begin();
}

int64_t
Dmatrix::get_id(size_t id) const{
    return ids[id];
}

/*
 * Transforms the input data to a matrix
 */
Dmatrix::Dmatrix(const std::vector < Matrix_cell_t > &data_costs) {
    set_ids(data_costs);
    costs.resize(ids.size());
    for (auto &row : costs) {
        row.resize(ids.size());
        for (auto &cell : row) {
            cell = std::numeric_limits<double>::max();
        }
    }
    for (const auto &data : data_costs) {
        costs[get_index(data.from_vid)][get_index(data.to_vid)] = data.cost;
    }

    for (size_t i = 0; i < costs.size(); ++i) {
        costs[i][i] = 0;
    }
}

bool
Dmatrix::has_no_infinity() const {
    for (const auto &row : costs) {
        for (const auto &val : row) {
            if (val == std::numeric_limits<double>::max()) return false;
        }
    }
    return true;
}


bool
Dmatrix::obeys_triangle_inequality() const {
    /*
     * Triangle Inequality Theorem.
     *  The sum of the lengths of any two sides of a triangle is greater than the length of the third side.
     *  NOTE: can also be equal for streets
     * costs[i][k] != inf
     * costs[i][k] <= costs[i][j] + costs[j][k] 
     */
    for (size_t i = 0; i < costs.size(); ++i) {
        for (size_t j = 0; j < costs.size(); ++j) {
            for (size_t k = 0; k < costs.size(); ++k) {
                if (!(costs[i][k] <= (costs[i][j] + costs[j][k]))) return false;
            }
        }
    }
    return true;
}

bool
Dmatrix::is_symetric() const{
    for (size_t i = 0; i < costs.size(); ++i) {
        for (size_t j = 0; j < costs.size(); ++j) {
            if (costs[i][j] != costs[j][i]) return false;
        }
    }
    return true;
}


Dmatrix
Dmatrix::get_symetric() const {
    double sum(0);
    for (const auto &row : costs) {
        for (const auto &cell : row) {
            sum += cell;
        }
    }
    if (is_symetric()) return *this; 
    Dmatrix new_costs;
    new_costs.costs.resize(costs.size() * 2);
    for (auto &row : new_costs.costs) {
        row.resize(costs.size() * 2);
        for (auto &cell : row) {
            cell = std::numeric_limits<double>::max();
        }
    }

    /*
     * Matrix cuadrants
     *   A= inf           B= transposed original
     *   C=original       D= inf
     *
     *   B & C "semi" diagonals are 0
     */
    for (size_t i = 0; i < costs.size(); ++i) {
        for (size_t j = 0; j < costs.size(); ++j) {
            /*
             *  A & D
             */
            new_costs[i][j] = 
                new_costs[i + costs.size()][j + costs.size()] = 
                std::numeric_limits<double>::max();

            /*
             * B
             */
            new_costs[i + costs.size()][j] =
                i == j? costs[i][j] : 0;

            /*
             * C
             */
            new_costs[i][j + costs.size()] =
                i == j? costs[j][i] : 0;
        }
    }
    new_costs.ids = ids;
    new_costs.ids.insert(new_costs.ids.end(), ids.begin(), ids.end());

    return new_costs;
}


std::ostream& operator<<(std::ostream &log, const Dmatrix &matrix){
    for (const auto id : matrix.ids) {
        log << "\t" << id;
    }
    log << "\n";
    size_t i = 0;
    for (const auto row : matrix.costs) {
        size_t j = 0;
        for (const auto cost : row) {
            log << "(" << i << "," << j << ")"
                << "\t(" << matrix.ids[i] << "," << matrix.ids[j] << ")"
                << "\t(" << matrix.get_index(matrix.ids[i]) << "," << matrix.get_index(matrix.ids[j]) << ")"
                << "\t = " << cost
                << "\t = " << matrix.costs[i][j]
                << "\t = " << matrix.costs[j][i] << "\n";
            ++j;
        }
        ++i;
    }
    for (size_t i = 0; i < matrix.costs.size(); ++i) {
        for (size_t j = 0; j < matrix.costs.size(); ++j) {
            for (size_t k = 0; k < matrix.costs.size(); ++k) {
                log << matrix.costs[i][k] << " <= (" << matrix.costs[i][j] << " + "  << matrix.costs[j][k] << ")"
                    << (matrix.costs[i][k] <= (matrix.costs[i][j] + matrix.costs[j][k])) << "\n";
            }
        }
    }

    return log;
}


}  // namespace tsp
}  // namespace pgRouting
