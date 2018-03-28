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
drop table if exists "ways";

-- The following should be OK

	select pgr_labelGraph('ways');
	select pgr_labelGraph('Ways');
	select pgr_labelGraph('ways', 'id');
	select pgr_labelGraph('ways', 'id', 'source');
	select pgr_labelGraph('ways', 'id', 'source', 'target');
	select pgr_labelGraph('ways', 'id', 'source', 'target', 'subgraph');
	select pgr_labelGraph('ways', 'id', 'source', 'target', 'subgraph', 'id<100');

	-- When table located in another schema e03
	select pgr_labelGraph('e03.ways');
	select pgr_labelGraph('e03.Ways');
	select pgr_labelGraph('e03.ways', 'id');
	select pgr_labelGraph('e03.ways', 'id', 'source');
	select pgr_labelGraph('e03.ways', 'id', 'source', 'target');
	select pgr_labelGraph('e03.ways', 'id', 'source', 'target', 'subgraph');
	select pgr_labelGraph('e03.ways', 'id', 'source', 'target', 'subgraph', 'id<100');

	-- When using the named notation
	select pgr_labelGraph('e03.calles', target:='destino', subgraph:='subgraph', id:='gido', source:='salida');
	select pgr_labelGraph('e03.calles', rows_where:='gido<100', id:='gido', source:='salida', target:='destino', subgraph:='subgraph');

-- The following should FAIL

	select pgr_labelGraph('id', 'ways');
	select pgr_labelGraph('ways', 'id', 'sourc', 'target');
	select pgr_labelGraph('ways', 'id', 'source', 'Target');
	select pgr_labelGraph('ways', 'id', 'source', 'target', 'subgraph', 'id<');

	-- When table located in another schema e03
	select pgr_labelGraph('e03.calles');
	select pgr_labelGraph('e03.Calles');
	select pgr_labelGraph('id', 'e03.calles');
	select pgr_labelGraph('e03.calles', 'id', 'sourc', 'target');
	select pgr_labelGraph('e03.calles', 'gido', 'source', 'target', 'subgraph', 'id<');
	select pgr_labelGraph('e03.calles', 'gid', 'salida', 'target', 'subgraph', 'id<10');
	select pgr_labelGraph('e03.calles', 'gid', 'salida', 'destino', 'subgraph', 'id<10 AND id>100');

	-- When using the named notation
	select pgr_labelGraph('e03.calles', target:='destino', subgraph:='subgraph', id:='gido');
	select pgr_labelGraph('e03.calles', target:='destino', subgraph:='subgraph', id:='gido', source:='salido');
	select pgr_labelGraph(rows_where:='gido<100', id:='gido', source:='salida', 'e03.calles', target:='destino', subgraph:='subgraph');

-- The following should return "rows_where condition generated 0 rows"

	select pgr_labelGraph('ways', 'id', 'source', 'target', 'subgraph', 'id<10 AND id>100');
	select pgr_labelGraph('e03.calles', id:='gido', rows_where:='gido<100 AND gido>200', source:='salida', target:='destino', subgraph:='subgraph');
