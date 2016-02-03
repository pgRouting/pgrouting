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


#include <vector>
#include <algorithm>

#include "./tsp_driver.h"

#include "./Dmatrix.hpp"
#include "./pgr_tsp.hpp"
/*
 * Defs
 */

typedef std::vector< int64_t > Ids;

int
do_pgr_tsp(
        Matrix_cell_t *distances,
        int64_t total_distances,
        int64_t start_vid,
        int64_t end_vid,
        General_path_element_t **path,
        size_t *total_results,
        char **err_msg) {

    std::vector < Matrix_cell_t > data_costs(distances, distances + total_distances);
    Dmatrix costs(data_costs);

    if (!costs.has_no_infinity()) {
        return 5;
    }
    if (!costs.obeys_triangle_inequality()) {
        return 6;
    }
    costs = costs.get_symetric();


#if 0
    int   i, j;
    int   istart = 0;
    int   jstart = 0;
    int   iend = -1;
    int   jend = -1;
    int   rev = 0;
    long  seed = -314159L;
    size_t blength;
#endif

    // PGR_DBG("sizeof(long)=%d", (int)sizeof(long));

#if 0
    // moving forward
    initRand(seed);
#endif 

    /* initialize tsp struct */
    TSP tsp(costs);
#if 0
    tsp.n = num;
    tsp.dist   = NULL;
    tsp.iorder = NULL;
    tsp.jorder = NULL;
    tsp.border = NULL;


    if (!(tsp.iorder = (int*) palloc (tsp.n * sizeof(int)))   ||
            !(tsp.jorder = (int*) palloc (tsp.n * sizeof(int)))   ||
            !(tsp.border = (int*) palloc (tsp.n * sizeof(int)))   ) {
        elog(FATAL, "Memory allocation failed!");
        return -1;
    }

    tsp.maxd = 0;
    for (i=0; i<tsp.n*tsp.n; i++) {
        tsp.maxd = MAX(tsp.maxd, cost[i]);
    }

    /* identity permutation */
    for (i = 0; i < tsp.n; i++) tsp.iorder[i] = i;

    tsp.bestlen = pathLength(&tsp);
    for (i = 0; i < tsp.n; i++) tsp.border[i] = tsp.iorder[i];
#endif

    // PGR_DBG("Initial Path Length: %.4f", tsp.bestlen);

    /*
     * Set up first eulerian path iorder to be improved by
     * simulated annealing.
     */
    if (!tsp.findEulerianPath())
        return 8;

#if 0
    // this is done automatically in tsp.findEulerianPath call
    blength = pathLength(&tsp);
    if (blength < tsp.bestlen) {
        tsp.bestlen = blength;
        for (i = 0; i < tsp.n; i++) tsp.border[i] = tsp.iorder[i];
    }
#endif

    // PGR_DBG("Approximated Path Length: %.4f", tsp.bestlen);

    tsp.annealing();

    *total_results = tsp.bestlen;
    // PGR_DBG("Final Path Length: %.4f", tsp.bestlen);

    tsp.iorder = tsp.border;
#if 0
    for (i=0; i<tsp.n; i++) tsp.iorder[i] = tsp.border[i];
    // PGR_DBG("Best Path Length: %.4f", *total_len);
#endif

    // reorder ids[] with start as first

#ifdef DEBUG
    for (i=0; i<tsp.n; i++) {
        // PGR_DBG("i: %d, ids[i]: %d, io[i]: %d, jo[i]: %d, jo[io[i]]: %d",
        i, ids[i], tsp.iorder[i], tsp.jorder[i], tsp.jorder[tsp.iorder[i]]);
    }
#endif

    size_t istart = costs.get_index(start_vid);
    auto start_ptr = std::find(tsp.iorder.begin(), tsp.iorder.end(), istart);

    /*
     *  1 2 3 4 5 6 e s 7 8 9 
     *  =>
     *  s 7 8 9 1 2 3 4 5 6 e
     */
    Ids results(start_ptr, tsp.iorder.end());
    std::copy(tsp.iorder.begin(), start_ptr, results.end());

    std::vector< General_path_element_t > result;
    result.reserve(tsp.iorder.size());
    auto prev_id = tsp.iorder.front();
    double agg_cost = 0;
    for (const auto &id : tsp.iorder) {
        General_path_element_t data;
        data.node = costs.get_id(id);
        data.cost   = id == prev_id? 0: costs[prev_id][id];
        agg_cost += data.cost;
        data.agg_cost = agg_cost;
        result.push_back(data);
    }

    *path = static_cast<General_path_element_t *>(malloc(sizeof(General_path_element_t) * (result.size())));

    //store the results
    int seq = 0;
    if (end_vid == -1) {};
    for (const auto &row : result) {
        (*path)[seq] = row;
        ++seq;
    }

    *total_results = result.size();

    (*err_msg) = NULL;
    return 0;

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


