/*PGR-GNU*****************************************************************
File: trsp_pgget.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Celia Virginia Vergara Castillo
mail: vicky at erosion.dev
Copyright (c) 2020 Mahmoud SAKR and Esteban ZIMANYI
mail: m_attia_sakrcw at yahoo.com, estebanzimanyicw at gmail.com
Copyright (c) 2016 Rohith Reddy
Copyright (c) 2016 Andrea Nardelli
mail: nrd.nardelli at gmail.com
Copyright (c) 2015 Celia Virginia Vergara Castillo
mail: vicky at erosion.dev


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
/** file

This is a copy of pgdata_getters.cpp

The old version of TRSP
- Is the only function that has its own edge reader within its code,
  hence the name of the file.
- The old version of TRSP will exist during 3.x series


@note
- The only function that remains is `fetch_edge`
- The old version of TRSP will be removed on V4

 @todo: V4 TODO Delete this file
*/

#include "c_common/trsp_pgget.h"
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>

#include "cpp_common/get_data.hpp"
#include "cpp_common/get_check_data.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/trsp_pgfetch.hpp"

#include "cpp_common/info_t.hpp"
#include "c_types/edge_rt.h"


/**
  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, cost, [reverse_cost] FROM edge_table;
  ~~~~

  @param[in] sql The query
  @param[out] rows the edges array
  @param[out] total_rows size of edges
  @param[in] normal when false then the graph is reversed
  @param[in] ignore_id when true id value of edge is ignored
  @param[out] err_msg when not null, there was an error and contains the message
  */
void
pgr_get_edges(
        char *sql,
        Edge_t **rows,
        size_t *total_rows,
        bool normal,
        bool ignore_id,
        char **err_msg) {
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{
        {-1, 0, !ignore_id, "id", pgrouting::ANY_INTEGER},
        {-1, 0, true, "source", pgrouting::ANY_INTEGER},
        {-1, 0, true, "target", pgrouting::ANY_INTEGER},
        {-1, 0, true, "cost", pgrouting::ANY_NUMERICAL},
        {-1, 0, false, "reverse_cost", pgrouting::ANY_NUMERICAL}};

        pgrouting::get_data(sql, rows, total_rows, normal, info, &pgrouting::fetch_edge);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = to_pg_msg(ex);
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = to_pg_msg("Caught unknown exception!");
    }
}

