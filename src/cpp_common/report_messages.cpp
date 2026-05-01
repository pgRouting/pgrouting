/*PGR-GNU*****************************************************************
File: report_messages.cpp

Developer:
Copyright (c) 2026 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

extern "C" {
#include "c_common/postgres_connection.h"
#include "c_common/e_report.h"
}

#include <sstream>

#include "cpp_common/report_messages.hpp"

#include "cpp_common/alloc.hpp"

namespace pgrouting {

void
report_messages(
        std::ostringstream &log,
        std::ostringstream &notice,
        std::ostringstream &err) {
    /*
     * Nothing to do
     */
    if (log.str().empty() && notice.str().empty() && err.str().empty()) return;

    auto err_msg = to_pg_msg(err);
    auto log_msg = to_pg_msg(log);
    auto notice_msg = to_pg_msg(notice);

    pgr_global_report(&log_msg, &notice_msg, &err_msg);
}

}  //  namespace pgrouting
