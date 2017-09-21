/*PGR-GNU*****************************************************************
File: e_report.h

Function's developer:
Copyright (c) 2016 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#ifndef INCLUDE_C_COMMON_E_REPORT_H_
#define INCLUDE_C_COMMON_E_REPORT_H_
#pragma once

/*! @name Postgres ereport
 *  Send notice or error to postgreSQL (cleans the char *)
 *
 *  From the C++ err_msg, log_msg, notice_msg are returned as a (char *),
 *
 *  Before exiting the C code the pointers need to be freed:
 *   - This task can be done when there is no error,
 *   - Call to ERROR, gives the control back to postgreSQL 
 *     - leaves a leak
 *
 * on C++ side, the message to be returned;
 * ~~~~{.c}
 * std::ostringstream log;
 * log << "the message";
 * *log_msg = pgr_msg(log.str().c_str());
 * ~~~~
 *
 *
 *
 *  when there is ERROR then postgreSQL takes over control
 *    The char* messages are cleared automaticalyy with pfree.
 *
 *  @warning When error: Free all data not created with palloc before calling
 *
 */

///@{
/*! @brief notice & error
 *
 *  ~~~~{.c}
 *  pgr_notice(&log_msg, &notice_msg, &error_msg);
 *
 *  precondition: before calling ereport
 *      assert(!*log_msg);
 *      assert(!*notice_msg);
 *      assert(!*error_msg);
 *  ~~~~
 */
void
pgr_global_report(
        char* log_msg,
        char* notice_msg,
        char* error_msg);

/*! @brief notice with no hint
 *
 *  ~~~~{.c}
 *  pgr_notice(&log_msg, &notice_msg);
 *
 *  precondition: before calling ereport
 *      assert(!log_msg);
 *      assert(!notice_msg);
 *  ~~~~
 */
void
pgr_notice(
        char* notice_msg);

/*! @brief notice with hint
 *
 *  ~~~~{.c}
 *  pgr_notice(&log_msg, &notice_msg);
 *
 *  precondition: before calling ereport
 *      assert(!log_msg);
 *      assert(!notice_msg);
 *  ~~~~
 */
void
pgr_notice2(
        char* log_msg,
        char* notice_msg);

/*! @brief error with no hint
 *
 * 
 *  ~~~~{.c}
 *  if (err_msg) {
 *      pfree(<data>);
 *  }
 *  pgr_error(&error_msg);
 *
 *  precondition: before calling ereport
 *      assert(!*error_msg);
 *  ~~~~
 */
void
pgr_error(char* error_msg);

/*! @brief error with hint
 *
 *  ~~~~{.c}
 *  if (err_msg) {
 *      pfree(<data>);
 *  }
 *  pgr_error(&log_msg, &error_msg);
 *
 *  precondition: before calling ereport
 *      assert(!*log_msg);
 *      assert(!*error_msg);
 *  ~~~~
 */
void
pgr_error2(
        char* log_msg,
        char* error_msg);
///@}

#endif  // INCLUDE_C_COMMON_E_REPORT_H_
