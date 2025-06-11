/*PGR-GNU*****************************************************************
File: bandwidth_driver.cpp

Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

// Function's developer:
Copyright (c) 2025 Saloni Kumari
Mail: chaudharysaloni2510@gmail.com

Generated with Template by:
Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
********************************************************************PGR-GNU*/

#include "drivers/metrics/bandwidth_driver.hpp"

#include <vector>
#include <sstream>
#include <string>
#include <algorithm>  

#include "cpp_common/assert.hpp"
#include "cpp_common/to_postgres.hpp"

struct Bandwidth_result {
    int64_t id;
    double bandwidth;
};

void do_bandwidth(
    const Edge_t* edges,
    size_t total_edges,
    GraphBandwidth_rt **result,
    size_t *result_count,
    char **log_msg,
    char **err_msg) {

    std::ostringstream log;
    std::ostringstream err;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));
        pgassert(!(*result));
        pgassert(*result_count == 0);

        if (total_edges == 0) {
            err << "No edges provided";
            *err_msg = pgrouting::to_pg_msg(err);
            return;
        }

        std::vector<Bandwidth_result> results;
        for (size_t i = 0; i < total_edges; ++i) {
            Bandwidth_result r;
            r.id = edges[i].id;
            r.bandwidth = std::min(edges[i].cost, edges[i].reverse_cost);
            results.push_back(r);
        }

        *result_count = results.size();
        *result = pgrouting::pgr_alloc(*result_count, (*result));

        for (size_t i = 0; i < *result_count; ++i) {
            (*result)[i].id = results[i].id;
            (*result)[i].bandwidth = results[i].bandwidth;
        }

        log << "Successfully processed " << *result_count << " edges.\n";
        *log_msg = pgrouting::to_pg_msg(log);
    } catch (const std::exception &e) {
        *result = nullptr;
        *result_count = 0;
        err << "Exception: " << e.what();
        *err_msg = pgrouting::to_pg_msg(err);
        *log_msg = pgrouting::to_pg_msg(log);
    } catch (...) {
        *result = nullptr;
        *result_count = 0;
        err << "Unknown error occurred";
        *err_msg = pgrouting::to_pg_msg(err);
        *log_msg = pgrouting::to_pg_msg(log);
    }
}
