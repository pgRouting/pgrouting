/*PGR-GNU*****************************************************************
File: pgr_palloc.cpp

Copyright (c) 2015 Celia Virginia Vergara Castillo
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

#include "cpp_common/pgr_alloc.hpp"
#include <cstring>
#include <string>

char *
pgr_msg(const std::string &msg) {
    char* duplicate = NULL;
    duplicate = pgr_alloc(msg.size() + 1, duplicate);
    memcpy(duplicate, msg.c_str(), msg.size());
    duplicate[msg.size()] = '\0';
    return duplicate;
}

