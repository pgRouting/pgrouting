/*PGR-GNU*****************************************************************
File: postgres_connection.h

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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
#ifndef INCLUDE_C_COMMON_POSTGRES_CONNECTION_H_
#define INCLUDE_C_COMMON_POSTGRES_CONNECTION_H_
#pragma once


#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpedantic"
#pragma clang diagnostic ignored "-Wignored-attributes"
#else
#pragma GCC diagnostic ignored "-Wpedantic"
#endif


#ifdef __MSVC__
#pragma warning(disable : 4200)
#endif

#include <postgres.h>


#ifdef __clang__
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wsign-conversion"
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic push
#  pragma GCC diagnostic ignored "-Wconversion"

#else
#  ifdef __GNUC__
#    pragma GCC diagnostic ignored "-Wsign-conversion"
#    pragma GCC diagnostic ignored "-Wconversion"
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#  endif
#endif


#include <executor/spi.h>
#include <funcapi.h>

#ifdef __clang__
#pragma clang diagnostic pop
#pragma clang diagnostic pop
#pragma clang diagnostic pop
#pragma clang diagnostic pop
#else
#ifdef __GNUC__
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif
#endif


#include "utils/builtins.h"  // for text_to_cstring

#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#else
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#endif

#include <fmgr.h>

#ifdef __clang__
#pragma clang diagnostic pop
#else
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#endif



void pgr_send_error(int errcode);
void pgr_SPI_finish(void);
void pgr_SPI_connect(void);
SPIPlanPtr pgr_SPI_prepare(char* sql);
Portal pgr_SPI_cursor_open(SPIPlanPtr SPIplan);
char* pgr_cstring2char(const char *inStr);

#endif  // INCLUDE_C_COMMON_POSTGRES_CONNECTION_H_
