/* PGR-GNU*****************************************************************
 * File: euclideanTSP_driver.cpp
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


#include "drivers/tsp/euclideanTSP_driver.h"

#include <string.h>
#include <sstream>
#include <vector>
#include <algorithm>

#include "tsp/pgr_tsp.hpp"
#include "tsp/euclideanDmatrix.h"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

void
do_pgr_euclideanTSP(
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
        char **notice_msg,
        char **err_msg) {
    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;

    try {
        std::vector< Coordinate_t > coordinates(
                coordinates_data,
                coordinates_data + total_coordinates);

        pgrouting::tsp::EuclideanDmatrix costs(coordinates);


        size_t idx_start = costs.has_id(start_vid) ?
            costs.get_index(start_vid) : 0;

        size_t idx_end = costs.has_id(end_vid) ?
            costs.get_index(end_vid) : 0;

        /* The ending vertex needs to be by the starting vertex */
        double real_cost = 0;
        if (costs.has_id(start_vid)
                && costs.has_id(end_vid)
                && start_vid != end_vid) {
            /*
             * Saving the real cost (distance)  between the start_vid and end_vid
             */
            real_cost = costs.distance(idx_start, idx_end);
            /*
             * Temporarily setting the cost between the start_vid and end_vid to 0
             */
            costs.set(idx_start, idx_end, 0);
        }


        log << "Processing Information\n"
            << "Initializing tsp class --->";
        pgrouting::tsp::TSP<pgrouting::tsp::EuclideanDmatrix> tsp(costs);


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

        /* The ending vertex needs to be by the starting vertex */
        if (costs.has_id(start_vid)
                && costs.has_id(end_vid)
                && start_vid != end_vid) {
            /*
             * Restoring the real cost (distance)  between the start_vid and end_vid
             */
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

        if (costs.has_id(start_vid)
                && costs.has_id(end_vid)
                && start_vid != end_vid) {
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

        pgassert(!log.str().empty());
        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}
