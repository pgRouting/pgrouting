/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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
BEGIN;
\echo '---- pgr_pointstodmatrix ----'
select * from pgr_pointstodmatrix(pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0));

\echo '---- pgr_vidstodmatrix ----'
select * from pgr_tsp(
    (select dmatrix::float8[]
       from pgr_vidstodmatrix(
                pgr_pointstovids(
                    pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0),
                    'edge_table'), 
                pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0),
                'edge_table')
    ),
    1
);
ROLLBACK;
