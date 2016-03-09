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

    WITH results AS
    (SELECT seq, route_id, node_id, cost from pgr_gsoc_vrppdtw(
        'select * from customer order by id'::text, 25,200)
    )
    SELECT cost < 4000 FROM results WHERE seq = 0;

    /*
    WITH results AS
    (SELECT seq, route_id, node_id, cost from pgr_gsoc_vrppdtw(
        'select * from customer order by id'::text, 25,200)
    )
    SELECT count(*) FROM results;

    WITH results AS
    (SELECT seq, route_id, node_id, cost from pgr_gsoc_vrppdtw(
        'select * from customer order by id'::text, 25,200)
    )
    SELECT route_id <= 14 FROM results WHERE seq = 136;
*/
    ROLLBACK;
