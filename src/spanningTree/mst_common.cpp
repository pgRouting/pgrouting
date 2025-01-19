/*PGR-GNU*****************************************************************
File: mst_common.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Vicky Vergara
Mail: vicky at georepublic dot de

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

#include "drivers/spanningTree/mst_common.h"

#include <string>
#include <sstream>

#include "cpp_common/assert.hpp"
#include "cpp_common/alloc.hpp"

int
get_order(char * fn_suffix, char ** err_msg) {
    using pgrouting::to_pg_msg;
    std::ostringstream err;
    try {
        pgassert(!(*err_msg));
        std::string suffix(fn_suffix);
        if (suffix.empty()) return 0;
        if (suffix == "DFS") return 1;
        if (suffix == "BFS") return 2;
        if (suffix == "DD") return 1;
        err << "Unknown function suffix" << suffix;
        *err_msg = to_pg_msg(err);
    } catch (std::exception &except) {
        err << except.what();
        *err_msg = to_pg_msg(err);
    }
    return -1;
}


char *
get_name(int fn_id, char * fn_suffix, char ** err_msg) {
    using pgrouting::to_pg_msg;
    std::ostringstream err;
    try {
        pgassert(!(*err_msg));
        std::string name;
        switch (fn_id) {
            case 0: name = "pgr_kruskal";
                    break;
            case 1: name = "pgr_prim";
                    break;
            default : name = "unknown";
                      err << "Unknown function name";
                      *err_msg = to_pg_msg(err);
        }
        std::string suffix(fn_suffix);
        name += suffix;
        char * full_name = to_pg_msg(name.c_str());
        return full_name;
    } catch (std::exception &except) {
        err << except.what();
        *err_msg = to_pg_msg(err);
    }
    return nullptr;
}
