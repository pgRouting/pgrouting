/*PGR-GNU*****************************************************************
File: TSP_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/

#include "drivers/tsp/TSP_driver.h"

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <utility>


#include "c_types/tsp_tour_rt.h"
#include "c_types/ii_t_rt.h"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

#include "tsp/tsp.hpp"


void
pgr_do_tsp(
        const char *matrix_sql,
        int64_t start_vid,
        int64_t end_vid,

        TSP_tour_rt **return_tuples,
        size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::tsp;
    using pgrouting::pgget::get_matrixRows;
    using TSP_graph = pgrouting::graph::UndirectedHasCostBG;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    const char *hint = nullptr;

    try {
        hint = matrix_sql;
        auto distances = get_matrixRows(std::string(matrix_sql));

        if (distances.size() == 0) {
            *notice_msg = to_pg_msg("Insufficient data found on inner query");
            *log_msg = hint? to_pg_msg(hint) : nullptr;
            return;
        }
        hint = nullptr;

        TSP_graph graph{distances};

        if (start_vid != 0 && !graph.has_vertex(start_vid)) {
            err << "Parameter 'start_id' do not exist on the data";
            *err_msg = to_pg_msg(err);
            return;
        }

        if (end_vid != 0 && !graph.has_vertex(end_vid)) {
            err << "Parameter 'end_id' do not exist on the data";
            *err_msg = to_pg_msg(err);
            return;
        }

        auto tsp_path = tsp(graph, start_vid, end_vid);

        if (!tsp_path.empty()) {
            *return_count = tsp_path.size();
            (*return_tuples) = pgr_alloc(tsp_path.size(), (*return_tuples));

            size_t seq{0};
            double total{0};
            for (const auto &e : tsp_path) {
                total += e.second;
                TSP_tour_rt data {e.first, e.second, total};
                (*return_tuples)[seq] = data;
                seq++;
            }
        }

        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (const std::pair<std::string, std::string>& ex) {
        (*return_count) = 0;
        *err_msg = to_pg_msg(ex.first.c_str());
        *log_msg = to_pg_msg(ex.second.c_str());
    } catch (const std::string &ex) {
        *err_msg = to_pg_msg(ex);
        *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
}
