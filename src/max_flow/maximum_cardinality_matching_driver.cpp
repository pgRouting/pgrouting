/*PGR-GNU*****************************************************************
File: maximum_cardinality_matching_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Ignroing directed flag & works only for undirected graph
Copyright (c) 2022 Celia Vriginia Vergara Castillo
Mail: vicky at georepublic.mail

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#include "drivers/max_flow/maximum_cardinality_matching_driver.h"

#include <sstream>
#include <vector>

#include "max_flow/pgr_maximumcardinalitymatching.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "c_types/edge_bool_t_rt.h"

void
do_pgr_maximum_cardinality_matching(
    Edge_bool_t *data_edges,
    size_t total_tuples,
    /* TODO(v4) flag directed is to be removed */
    bool,

    Only_int_rt **return_tuples,
    size_t *return_count,

    char** log_msg,
    char** notice_msg,
    char **err_msg) {
    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;

    try {
        pgrouting::flow::PgrCardinalityGraph G(data_edges, total_tuples);
        auto matched_vertices = G.get_matched_vertices();

        (*return_tuples) = pgr_alloc(matched_vertices.size(), (*return_tuples));
        for (size_t i = 0; i < matched_vertices.size(); ++i) {
            (*return_tuples)[i] = matched_vertices[i];
        }
        *return_count = matched_vertices.size();

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
