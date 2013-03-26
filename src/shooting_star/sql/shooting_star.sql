--
-- Copyright (c) 2005 Sylvain Pasche,
--               2006-2007 Anton A. Patrushev, Orkney, Inc.
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
--


-----------------------------------------------------------------------
-- Core function for shortest_path_astar computation
-- Simillar to shortest_path in usage but uses the Shooting* algorithm
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION shortest_path_shooting_star(sql text, source_id integer, 
        target_id integer,directed boolean, has_reverse_cost boolean)
         RETURNS SETOF path_result
         AS '$libdir/librouting'
         LANGUAGE c IMMUTABLE STRICT; 
