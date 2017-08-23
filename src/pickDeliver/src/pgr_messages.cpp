/*PGR-GNU*****************************************************************

FILE: pgr_pickDeliver.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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


#include <string>
#include <sstream>
#include "cpp_common/pgr_messages.h"

namespace pgrouting {



std::string
Pgr_messages::get_log() const {
    auto str = log.str();
    return str;
}

std::string
Pgr_messages::get_notice() const {
    auto str = notice.str();
    return str;
}

bool
Pgr_messages::has_error() const {
    return  !error.str().empty();
}

std::string
Pgr_messages::get_error() const {
    auto str = error.str();
    return str;
}

#ifndef NDEBUG
std::string
Pgr_messages::get_dbglog() const {
    return dbg_log.str();
}
#endif

void
Pgr_messages::clear() {
    log.str("");
    log.clear();

    notice.str("");
    notice.clear();

    error.str("");
    error.clear();

#ifndef NDEBUG
    dbg_log.str("");
    dbg_log.clear();
#endif
}


}  //  namespace pgrouting
