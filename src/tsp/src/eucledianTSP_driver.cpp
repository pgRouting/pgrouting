/* PGR-GNU*****************************************************************
 * File: eucledianTSP_driver.cpp
 *
 * Generated with Template by:
 * Copyright (c) 2015 pgRouting developers
 * Mail: project@pgrouting.org
 *
 * Function's developer:
 * Copyright (c) 2015 Celia Virginia Vergara Castillo
 * Mail: vicky_vergara@hotmail.com
 *
 * ------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  ******************************************************************** PGR-GNU*/

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif


#include <string.h>
#include <sstream>
#include <vector>
#include <algorithm>

#include "./eucledianTSP_driver.h"
#include "./eucledianDmatrix.h"

#include "./pgr_tsp.hpp"
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_alloc.hpp"

void
do_pgr_eucledianTSP(
        Coordinate_t *coordinates_data,
        size_t total_coordinates,
        int64_t start_vid,
        int64_t end_vid,

        double initial_temperature,
        double final_temperature,
        double cooling_factor,
        int64_t tries_per_temperature,
        int64_t max_changes_per_temperature,
        int64_t max_consecutive_non_changes,
        bool randomize,
        double time_limit,

        General_path_element_t **return_tuples,
        size_t *return_count,
        char **log_msg,
        char **err_msg) {
    std::ostringstream err;
    std::ostringstream log;

    try {
        std::vector< Coordinate_t > coordinates(
                coordinates_data,
                coordinates_data + total_coordinates);

        pgrouting::tsp::eucledianDmatrix costs(coordinates);

        double real_cost = -1;

        size_t idx_start = costs.has_id(start_vid) ?
            costs.get_index(start_vid) : 0;

        size_t idx_end = costs.has_id(end_vid) ?
            costs.get_index(end_vid) : 0;

        if (costs.has_id(start_vid) && costs.has_id(end_vid) && start_vid != end_vid) {
            /* An ending vertex needs to be by the starting vertex */
            real_cost = costs.distance(idx_start, idx_end);
            costs.set(idx_start, idx_end, 0);
        }


        log << "pgr_eucledianTSP Processing Information \nInitializing tsp class --->";
        pgrouting::tsp::TSP<pgrouting::tsp::eucledianDmatrix> tsp(costs);


        log << " tsp.greedyInitial --->";
        tsp.greedyInitial(idx_start);



        log << " tsp.annealing --->";
        tsp.annealing(
                initial_temperature,
                final_temperature,
                cooling_factor,
                tries_per_temperature,
                max_changes_per_temperature,
                max_consecutive_non_changes,
                randomize,
                time_limit);
        log << " OK\n";
        log << tsp.get_log();
        log << tsp.get_stats();

        auto bestTour(tsp.get_tour());

        if (costs.has_id(start_vid) && costs.has_id(end_vid) && start_vid != end_vid) {
            costs.set(idx_start, idx_end, real_cost);
        }

        log << "\nBest cost reached = " << costs.tourCost(bestTour);

        auto start_ptr = std::find(
                bestTour.cities.begin(),
                bestTour.cities.end(),
                idx_start);

        std::rotate(
                bestTour.cities.begin(),
                start_ptr,
                bestTour.cities.end());

        if (costs.has_id(start_vid) && costs.has_id(end_vid) && start_vid != end_vid) {
            if (*(bestTour.cities.begin() + 1) == idx_end) {
                std::reverse(
                        bestTour.cities.begin() + 1,
                        bestTour.cities.end());
            }
        }


        std::vector< General_path_element_t > result;
        result.reserve(bestTour.cities.size() + 1);
        pgassert(bestTour.cities.size() == costs.size());

        bestTour.cities.push_back(bestTour.cities.front());

        auto prev_id = bestTour.cities.front();
        double agg_cost = 0;
        for (const auto &id : bestTour.cities) {
            if (id == prev_id) continue;
            General_path_element_t data;
            data.node = costs.get_id(prev_id);
            data.edge = prev_id;
            data.cost = costs.distance(prev_id, id);
            data.agg_cost = agg_cost;
            result.push_back(data);
            agg_cost += data.cost;
            prev_id = id;
        }

        /* inserting the returning to starting point */
        {
            General_path_element_t data;
            data.node = costs.get_id(bestTour.cities.front());
            data.edge = bestTour.cities.front();
            data.cost = costs.distance(prev_id, bestTour.cities.front());
            agg_cost += data.cost;
            data.agg_cost = agg_cost;
            result.push_back(data);
        }

        pgassert(result.size() == bestTour.cities.size());
        *return_count = bestTour.size();
        (*return_tuples) = pgr_alloc(result.size(), (*return_tuples));

        /* store the results */
        int seq = 0;
        for (const auto &row : result) {
            (*return_tuples)[seq] = row;
            ++seq;
        }

        *log_msg = strdup(log.str().c_str());
        (*err_msg) = NULL;
        return;
    } catch (AssertFailedException &exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << exept.what() << "\n";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch (std::exception& exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << exept.what() << "\n";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch(...) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!\n";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    }
}
