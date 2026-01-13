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
 * *log_msg = to_pg_msg(log.str());
 * ~~~~
 *
 *
 *
 *  when there is ERROR then postgreSQL takes over control
 *    The char* messages are cleared automatically with pfree.
 *
 *  @warning When error: Free all data not created with palloc before calling
 *
 */

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
pgr_global_report(char**, char**, char**);

/*! @brief  Generates a notice on PostgreSQL
 *
 * This is a development helper function
 * `pgr_notice` on code and related code must be deleted
 *
 * To use:
 * Add at the begining of the .cpp/.hpp file:
 *  ~~~~{.c}
 * extern "C" {
 * #include "c_common/postgres_connection.h"
 * #include "c_common/e_report.h"
 * }
 * #include "cpp_common/alloc.hpp"
 *  ~~~~
 *
 * Insert a variable where to store the message
 * Save the message
 * Call the function using the to_pg_msg
 * ~~~~{.c}
 * std::ostringstream msg;
 * int this_variable = 3;
 * msg << "This is the message that shows the value of : " << this_variable;
 * pgr_print_notice(to_pg_msg(msg));
 *  ~~~~
 */
void
pgr_print_notice(char*);

/* @brief throws postgres error when first string is not null */
void pgr_throw_error(const char*, const char*);

#endif  // INCLUDE_C_COMMON_E_REPORT_H_
