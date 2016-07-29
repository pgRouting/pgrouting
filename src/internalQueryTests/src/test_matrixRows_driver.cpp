/*PGR-GNU*****************************************************************
File: testXYedges_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: 

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


#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include "./test_matrixRows_driver.h"

#define DEBUG

#include "./../../common/src/pgr_assert.h"

bool do_pgr_test_matrixRows(
        Matrix_cell_t *matrix_rows,
        size_t total_rows,
        char ** log_msg,
        char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));

        std::vector< Matrix_cell_t > matrix(matrix_rows, matrix_rows + total_rows);

        log << "Original: \n" <<
            std::setprecision(32);
        for (const auto row : matrix) {
            log << "start_vid = " << row.from_vid
                << "\tend_vid = " << row.to_vid
                << "\tagg_cost = " << row.cost;
        }

        *err_msg = NULL;
        *log_msg = strdup(log.str().c_str());
        return true;
    } catch (AssertFailedException &exept) {
        log << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
        return false;
    } catch (std::exception& exept) {
        log << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
        return false;
    } catch(...) {
        log << "Caught unknown exception!\n";
        *err_msg = strdup(log.str().c_str());
        return false;
    }
}
