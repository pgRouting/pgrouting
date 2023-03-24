/*PGR-GNU*****************************************************************
File: combinations_input.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Mahmoud SAKR and Esteban ZIMANYI
mail: m_attia_sakr@yahoo.com, estebanzimanyi@gmail.com

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


#include <math.h>
#include <float.h>
#include <limits.h>
#ifdef __cplusplus
#   include <cstddef>
#else
#   include <stddef.h>
#endif

#include "c_common/pgdata_getters.h"
#include "c_types/column_info_t.h"
#include "c_types/ii_t_rt.h"
#include "c_common/debug_macro.h"
#include "c_common/get_check_data.h"
#include "c_common/time_msg.h"


static
void fetch_combination(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[2],
        II_t_rt *combination,
        size_t *valid_combinations) {
    combination->d1.source = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    combination->d2.target = pgr_SPI_getBigInt(tuple, tupdesc, info[1]);

    *valid_combinations = *valid_combinations + 1;
}



static
void
get_combinations_2_columns(
        char *sql,
        II_t_rt **combinations,
        size_t *totalTuples) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

    size_t total_tuples;
    size_t valid_combinations;

    Column_info_t info[2];

    int i;
    for (i = 0; i < 2; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_INTEGER;
    }
    info[0].name = "source";
    info[1].name = "target";

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = true;
    (*totalTuples) = total_tuples = valid_combinations = 0;

    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 2);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*combinations) == NULL)
                (*combinations) = (II_t_rt *)
                    palloc0(total_tuples * sizeof(II_t_rt));
            else
                (*combinations) = (II_t_rt *)
                    repalloc((*combinations), total_tuples * sizeof(II_t_rt));

            if ((*combinations) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_combination(&tuple, &tupdesc, info,
                                  &(*combinations)[total_tuples - ntuples + t],
                                  &valid_combinations);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0 || valid_combinations == 0) {
        PGR_DBG("No combinations found");
    }

    (*totalTuples) = total_tuples;
    PGR_DBG("Reading %ld combinations", total_tuples);
    time_msg("reading combinations", start_t, clock());
}

/**
 * @param [in] combinations_sql
 * @param [out] combinations II_t_rt
 * @param [out] total_combinations
 */
void
pgr_get_combinations(
        char *combinations_sql,
        II_t_rt **combinations,
        size_t *total_combinations) {
    get_combinations_2_columns(combinations_sql, combinations, total_combinations);
}
