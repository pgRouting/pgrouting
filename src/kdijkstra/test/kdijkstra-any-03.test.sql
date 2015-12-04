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
update kdgrid set cost=-1.0 where  source=2 and target=3;
update kdgrid set cost=-1.0 where  source=12 and target=13;
update kdgrid set cost=-1.0 where  source=22 and target=23;
update kdgrid set cost=-1.0 where  source=32 and target=33;
update kdgrid set cost=-1.0 where  source=42 and target=43;
select * from pgr_kdijkstrapath('select gid as id, source, target, cost from kdgrid', 288, array[1,25,601,625], false, false);
