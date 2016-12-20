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

#ifndef SRC_MESSAGES_SRC_PGR_PICKDELIVER_H_
#define SRC_MESSAGES_SRC_PGR_PICKDELIVER_H_
#pragma once



#include <string>
#include <sstream>

namespace pgrouting {
namespace vrp {


class Pgr_messages {

 public:
    Pgr_messages() = default;
    Pgr_messages(const Pgr_messages&) = delete;

    /*! \brief get_log
     *
     * \returns the current contents of the log and clears the log
     *
     */
    std::ostream& get_log(std::ostream &p_log) const;

    /*! \brief get_notice
     *
     * \returns the current contents of the log and clears the log
     *
     */
    std::ostream& get_notice(std::ostream &p_log) const;

    /*! \brief get_error
     *
     * \returns the current contents of the log and clears the log
     *
     */
    std::ostream& get_error(std::ostream &p_log) const;

#ifndef NDEBUG
    /*! \brief get_error
     *
     * \returns the current contents of the log and clears the log
     *
     */
    std::ostream& get_dbglog(std::ostream &p_log) const;
#endif


 private:
    mutable std::ostringstream log;
    mutable std::ostringstream notice;
    mutable std::ostringstream error;
#ifndef NDEBUG
    mutable std::ostringstream dbg_log;
#endif
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // SRC_MESSAGES_SRC_PGR_PICKDELIVER_H_
