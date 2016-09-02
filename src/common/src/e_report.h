/*PGR-GNU*****************************************************************
File: error_reporting.h

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

#pragma once

/**
 * Report notice & error to postgreSQL
 *
 *  The char* messages are cleared.
 *  New messages are made with palloc
 *
 *  when there is ERROR then postgreSQL takes over control
 *
 *  @warning Free all data not created with palloc before calling
 *  ~~~~{.c}
 *  if (err_msg) {
 *      pfree(<data>);
 *  }
 *  pgr_error_report(log_msg, notice_msg, error_msg);
 *  ~~~~
 */

void
pgr_error_report(
    char **log_msg,
    char **notice_msg,
    char **err_msg);
