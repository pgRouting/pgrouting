/*PGR-GNU*****************************************************************
File: messages.hpp

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

/*! @file */

#ifndef INCLUDE_CPP_COMMON_MESSAGES_HPP_
#define INCLUDE_CPP_COMMON_MESSAGES_HPP_
#pragma once



#include <string>
#include <sstream>

namespace pgrouting {

class Pgr_messages {
 public:
    Pgr_messages() = default;
    Pgr_messages(const Pgr_messages&) = delete;
    Pgr_messages& operator=(const Pgr_messages&) = delete;

    /*! @brief get_log
     *
     * \returns the current contents of the log and clears the log
     *
     */
    std::string get_log() const;

    /*! @brief get_notice
     *
     * @returns the current contents of the log and clears the log
     *
     */
    std::string get_notice() const;

    /*! @brief get_error
     *
     * @returns the current contents of the log and clears the log
     */
    bool has_error() const;

    /*! @brief get_error
     *
     * @returns the current contents of the log and clears the log
     *
     */
    std::string get_error() const;

    /*! @brief clear
     *
     * Clears All the messages
     *
     */
    void clear();

 public:
    /*! Stores the hint information*/
    mutable std::ostringstream log;
    /*! Stores the notice information*/
    mutable std::ostringstream notice;
    /*! Stores the error information*/
    mutable std::ostringstream error;
};


#if defined(__MINGW32__) || defined(_MSC_VER)
#define ENTERING(x)
#define EXITING(x)
#else
#define ENTERING(x) x.log << "\n--> " << __PRETTY_FUNCTION__ << "\n"
#define EXITING(x) x.log << "\n<-- " << __PRETTY_FUNCTION__ << "\n"
#endif




}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_MESSAGES_HPP_
