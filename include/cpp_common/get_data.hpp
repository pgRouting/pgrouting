/*PGR-GNU*****************************************************************
File: get_data.hpp

Copyright (c) 2023 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Celia Virginia Vergara Castillo
mail: vicky at erosion.dev

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

#ifndef INCLUDE_CPP_COMMON_GET_DATA_HPP_
#define INCLUDE_CPP_COMMON_GET_DATA_HPP_
#pragma once

#include "c_common/postgres_connection.h"

#include <vector>
#include <string>
#include <cstdint>

#include "cpp_common/info_t.hpp"
#include "cpp_common/get_check_data.hpp"
#include "cpp_common/alloc.hpp"

namespace pgrouting {

/** @brief Retrieves the tuples
 * @tparam Data_type Scructure of data
 * @tparam Func fetcher function
 * @param[in] sql  Query to be processed
 * @param[out] pgtuples C array of data
 * @param[out] total_pgtuples C array size
 * @param[in] flag useful flag depending on data
 * @param[in] info information about the data
 * @param[in] func fetcher function to be used
 */
template <typename Data_type, typename Func>
void get_data(
        char *sql,
        Data_type **pgtuples,
        size_t *total_pgtuples,
        bool flag,
        std::vector<Column_info_t> info,
        Func func) {
    const int tuple_limit = 1000000;

    size_t total_tuples;
    size_t valid_pgtuples;

    auto SPIplan = pgr_SPI_prepare(sql);
    auto SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = true;
    (*total_pgtuples) = total_tuples = valid_pgtuples = 0;

    int64_t default_id = 0;

    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        auto tuptable = SPI_tuptable;
        auto tupdesc = SPI_tuptable->tupdesc;
        if (total_tuples == 0) fetch_column_info(tupdesc, info);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            (*pgtuples) = pgr_alloc(total_tuples, *pgtuples);
            if ((*pgtuples) == NULL) {
                throw std::string("Out of memory!");
            }

            for (size_t t = 0; t < ntuples; t++) {
                func(tuptable->vals[t], tupdesc, info,
                        &default_id,
                        &(*pgtuples)[total_tuples - ntuples + t],
                        &valid_pgtuples, flag);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);
    (*total_pgtuples) = total_tuples;
}

namespace pgget {
/** @brief Retrieves the tuples
 * @tparam Data_type Scructure of data
 * @tparam Func fetcher function
 * @param[in] sql  Query to be processed
 * @param[in] flag useful flag depending on data
 * @param[in] info information about the data
 * @param[in] func fetcher function to be used
 */
template <typename Data_type, typename Func>
std::vector<Data_type> get_data(
        const std::string& sql,
        bool flag,
        std::vector<Column_info_t> info,
        Func func) {
    const int tuple_limit = 1000000;

    size_t total_tuples;
    size_t valid_pgtuples;

    auto SPIplan = pgr_SPI_prepare(sql.c_str());
    auto SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = true;
    total_tuples = valid_pgtuples = 0;

    int64_t default_id = 0;
    std::vector<Data_type> tuples;

    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        auto tuptable = SPI_tuptable;
        auto tupdesc = SPI_tuptable->tupdesc;
        if (total_tuples == 0) fetch_column_info(tupdesc, info);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            tuples.reserve(total_tuples);
            for (size_t t = 0; t < ntuples; t++) {
                tuples.push_back(func(tuptable->vals[t], tupdesc, info,
                        &default_id,
                        &valid_pgtuples, flag));
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);
    return tuples;
}

}  // namespace pgget
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_GET_DATA_HPP_
