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
 * \returns the current contents of the log
 *
 */
std::string
Pgr_messages::get_log() const {
    std::string str = log.str();
    return str;
}

/*! \brief get notice messages
 *
 * \returns the current contents of the log
 *
 */
std::string
Pgr_messages::get_notice() const {
    std::string str = notice.str();
    return str;
}

/*! \brief get error messages
 *
 * \returns the current contents of the log
 *
 */
std::string
Pgr_messages::get_error() const {
    std::string str = error.str();
    return str;
}

#ifndef NDEBUG
/*! \brief get debugging logs
 *
 * \returns the current contents of the log and clears the log
 *
 */
std::string
Pgr_messages::get_dbglog() const{
    std::string str = dbg_log.str();
    return str;
}
#endif

/*! \brief get error messages
 *
 * clears the all logs
 */
void
Pgr_messages::clear_msg() {
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


}  //  namespace vrp
}  //  namespace pgrouting

