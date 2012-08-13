/******************************************************************************
 *
 * Project: GSOC 2012 Project 
 * Purpose: pgRouting mild_two_q algorithm
 * Author:  Jinfu Leng
 *

******************************************************************************
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

*****************************************************************************/
 
#ifndef _MILD_TWO_Q_H
#define _MILD_TWO_Q_H

#include "postgres.h"
#include "dijkstra.h"

#ifdef __cplusplus
extern "C"
#endif
int boost_mild_two_q(edge_t *edges, unsigned int count, int start_vertex, int end_vertex,
		   bool directed, bool has_reverse_cost,
                   path_element_t **path, int *path_count, char **err_msg);

#endif
