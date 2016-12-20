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
std::ostream&
Pgr_messages::get_log(std::ostream &p_log) const {
    p_log << log.str() << "\n";
    log.str("");
    log.clear();
    return p_log;
}

/*! \brief get notice messages
 *
 * \returns the current contents of the log and clears the log
 *
 */
std::ostream&
Pgr_messages::get_notice(std::ostream &p_log)
    const {
        p_log << notice.str() << "\n";
        notice.str("");
        notice.clear();
        return p_log;
    }

/*! \brief get error messages
 *
 * \returns the current contents of the log and clears the log
 *
 */
std::ostream&
Pgr_messages::get_error(std::ostream &p_log) const {
    p_log << error.str() << "\n";
    error.str("");
    error.clear();
    return p_log;
}

#ifndef NDEBUG
/*! \brief get debuging logs
 *
 * \returns the current contents of the log and clears the log
 *
 */
std::ostream&
Pgr_messages::get_dbglog(std::ostream &p_log) const {
    p_log << dbg_log.str() << "\n";
    dbg_log.str("");
    dbg_log.clear();
    return p_log;
}
#endif


}  //  namespace vrp
}  //  namespace pgrouting

