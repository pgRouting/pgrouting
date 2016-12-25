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
#include "./pgr_messages.h"

namespace pgrouting {
namespace vrp {



/*! \brief get log (hints) messages
 *
 * \returns the current contents of the log and clears the log
 *
 */
std::string
Pgr_messages::get_log() {
    std::string str = log.str();
    log.str("");
    log.clear();
    return str;
}

/*! \brief get notice messages
 *
 * \returns the current contents of the log and clears the log
 *
 */
std::string
Pgr_messages::get_notice() {
    std::string str = notice.str();
    notice.str("");
    notice.clear();
    return str;
}

/*! \brief get error messages
 *
 * \returns the current contents of the log and clears the log
 *
 */
std::string
Pgr_messages::get_error() {
    std::string str = error.str();
    error.str("");
    error.clear();
    return str;
}

#ifndef NDEBUG
/*! \brief get debuging logs
 *
 * \returns the current contents of the log and clears the log
 *
 */
std::string
Pgr_messages::get_dbglog() {
    std::string str = dbg_log.str();
    dbg_log.str("");
    dbg_log.clear();
    return str;
}
#endif


}  //  namespace vrp
}  //  namespace pgrouting

