/*// PGR-GNU*****************************************************************
 * File: tspi_driver.cpp
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

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <string.h>
#include <vector>
#include <algorithm>

#include "./tsp_driver.h"

#include "./Dmatrix.h"
#include "./pgr_tsp.hpp"
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_alloc.hpp"

/*
 * Defs
 */

typedef std::vector< int64_t > Ids;

int
do_pgr_tsp(
        Matrix_cell_t *distances,
        size_t total_distances,
        int64_t start_vid,
        int64_t end_vid,

        double initial_temperature,
        double final_temperature,
        double cooling_factor,
        int64_t tries_per_temperature,
        int64_t change_per_temperature,
        bool fix_random,

        General_path_element_t **return_tuples,
        size_t *return_count,
        char **log_msg, 
        char **err_msg) {
    std::ostringstream err;
    std::ostringstream log;

    try {

        std::vector < Matrix_cell_t > data_costs(distances, distances + total_distances);
        pgRouting::tsp::Dmatrix costs(data_costs);
        double real_cost = -1;
        if (end_vid > 0) {
            /* An ending vertex needs to be by the starting vertex */
            size_t idx_start = costs.get_index(start_vid);
            size_t idx_end = costs.get_index(end_vid);
            real_cost = costs[idx_start][idx_end];
            costs[idx_start][idx_end] = costs[idx_end][idx_start] = 0.0001;
        }

        // log << costs;

        if (!costs.has_no_infinity()) {

            err << "An Infinity value was found on the Matrix";
            *err_msg = strdup(err.str().c_str());
            *log_msg = strdup(log.str().c_str());
            return 5;
        }
#if 0
        if (!costs.obeys_triangle_inequality()) {
            err << "The Matrix dos not comply with triangle inequality";
            *err_msg = strdup(err.str().c_str());
            *log_msg = strdup(log.str().c_str());
            return 6;
        }
#endif



        /* initialize tsp struct */
        log << "Initializing tsp class --->";
        pgRouting::tsp::TSP tsp(costs);
        log << "OK\n";


        /*
         * Initial solution
         */
        log << "tsp.greedyInitial --->";
        tsp.greedyInitial();
        log << "OK\n";



        log << "tsp.annealing --->";
        tsp.annealing(
                initial_temperature,
                final_temperature,
                cooling_factor,
                tries_per_temperature,
                change_per_temperature,
                fix_random);
        log << tsp.get_log();
        log << "OK\n";

        auto bestTour(tsp.get_cities());
        log << "\nbest tour\n";
        for (const auto &id : bestTour) {
            log << id << ", ";
        }

        if (end_vid > 0) {
            size_t idx_start = costs.get_index(start_vid);
            size_t idx_end = costs.get_index(end_vid);
            costs[idx_start][idx_end] = costs[idx_end][idx_start] = real_cost;
        }

        log << "\nbest cost" << costs.pathCost(bestTour) << ", ";
        size_t istart = costs.get_index(start_vid);
        auto start_ptr = std::find(bestTour.begin(), bestTour.end(), istart);
        std::rotate(bestTour.begin(), start_ptr, bestTour.end());

        std::vector< General_path_element_t > result;
        result.reserve(bestTour.size() + 1);
        pgassert(bestTour.size() == costs.size());

        auto prev_id = bestTour.front();
        double agg_cost = 0;
        for (const auto &id : bestTour) {
            General_path_element_t data;
            data.node = costs.get_id(id);
            data.edge = id;
            data.cost = id == prev_id? 0: costs[prev_id][id];
            agg_cost += data.cost;
            data.agg_cost = agg_cost;
            result.push_back(data);
            prev_id = id;
        }
        {
            /* inserting the returning to starting point */
            General_path_element_t data;
            data.node = costs.get_id(bestTour.front());
            data.edge = bestTour.front();
            data.cost = costs[prev_id][bestTour.front()];
            agg_cost += data.cost;
            data.agg_cost = agg_cost;
            result.push_back(data);
        }

        pgassert(result.size() == bestTour.size() + 1);
        *return_count = bestTour.size() + 1;
        (*return_tuples) = pgr_alloc(result.size(), (*return_tuples));

        //store the results
        int seq = 0;
        if (end_vid == -1) {};
        for (const auto &row : result) {
            (*return_tuples)[seq] = row;
            ++seq;
        }

        *log_msg = strdup(log.str().c_str());
        (*err_msg) = NULL;
        return 0;

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
    return 1000;


#if 0
    /*
     * If the end is specified and the end point and it follow start
     * then we swap start and end and extract the list backwards
     * and later we reverse the list for the desired order.
     */
    if ((jend > 0 && jend == jstart + 1)
            || (jend == 0 && jstart == tsp.n-1)) {
        int tmp = jend;
        jend = jstart;
        jstart = tmp;
        rev = 1;
        // PGR_DBG("reversed start and end: jstart: %d, jend: %d", jstart, jend);
    }

    // copy ids to tsp.jorder so we can rewrite ids
    memcpy(tsp.jorder, ids, tsp.n * sizeof(int));

    // write reordered ids into ids[]
    // remember at this point jorder is our list if ids
    for (i=jstart, j=0; i < tsp.n; i++, j++)
        ids[j] = tsp.jorder[tsp.iorder[i]];

    for (i=0; i < jstart; i++, j++)
        ids[j] =tsp.jorder[tsp.iorder[i]];

    // if we reversed the order above, now put it correct.
    if (rev) {
        int tmp = jend;
        jend = jstart;
        jstart = tmp;
        reverse(tsp.n, ids);
    }
#endif

}


