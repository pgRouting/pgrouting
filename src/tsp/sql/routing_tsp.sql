/*PGR-MIT*****************************************************************

--
-- Copyright (c) 2013 Stephen Woodbridge
--
-- This files is released under an MIT-X license.

------
MIT/X license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

********************************************************************PGR-MIT*/


-----------------------------------------------------------------------
-- Core function for TSP
-----------------------------------------------------------------------
/*
 * select seq, id from pgr_tsp(matrix float8[][], start int,
 *                             OUT seq int, OUT id int);
*/
-- endpt does not work, and is ignored in the code at the moment
-- we hope to support it in the future but the tsp algorithm needs to
-- change or be replaced to support this functionality.
CREATE OR REPLACE FUNCTION pgr_tsp(matrix float8[][], startpt integer, endpt integer DEFAULT -1, OUT seq integer, OUT id integer)

--CREATE OR REPLACE FUNCTION pgr_tsp(matrix float8[][], startpt integer, OUT seq integer, OUT id integer)
    RETURNS SETOF record
    AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'tsp_matrix'
    LANGUAGE c IMMUTABLE STRICT;
