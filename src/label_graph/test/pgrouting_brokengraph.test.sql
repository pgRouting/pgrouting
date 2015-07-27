set client_min_messages to warning;
SET search_path TO public;
drop table if exists "ways";

-- The following should be OK

	select pgr_brokenGraph('ways');
	select pgr_brokenGraph('Ways');
	select pgr_brokenGraph('ways', 'id');
	select pgr_brokenGraph('ways', 'id', 'source');
	select pgr_brokenGraph('ways', 'id', 'source', 'target');
	select pgr_brokenGraph('ways', 'id', 'source', 'target', 'subgraph');
	select pgr_brokenGraph('ways', 'id', 'source', 'target', 'subgraph', 'id<100');

	-- When table located in another schema e03
	select pgr_brokenGraph('e03.ways');
	select pgr_brokenGraph('e03.Ways');
	select pgr_brokenGraph('e03.ways', 'id');
	select pgr_brokenGraph('e03.ways', 'id', 'source');
	select pgr_brokenGraph('e03.ways', 'id', 'source', 'target');
	select pgr_brokenGraph('e03.ways', 'id', 'source', 'target', 'subgraph');
	select pgr_brokenGraph('e03.ways', 'id', 'source', 'target', 'subgraph', 'id<100');

	-- When using the named notation
	select pgr_brokenGraph('e03.calles', target:='destino', subgraph:='subgraph', id:='gido', source:='salida');
	select pgr_brokenGraph('e03.calles', rows_where:='gido<100', id:='gido', source:='salida', target:='destino', subgraph:='subgraph');

-- The following should FAIL
	
	select pgr_brokenGraph('id', 'ways');
	select pgr_brokenGraph('ways', 'id', 'sourc', 'target');
	select pgr_brokenGraph('ways', 'id', 'source', 'Target');
	select pgr_brokenGraph('ways', 'id', 'source', 'target', 'subgraph', 'id<');

	-- When table located in another schema e03
	select pgr_brokenGraph('e03.calles');
	select pgr_brokenGraph('e03.Calles');
	select pgr_brokenGraph('id', 'e03.calles');
	select pgr_brokenGraph('e03.calles', 'id', 'sourc', 'target');
	select pgr_brokenGraph('e03.calles', 'gido', 'source', 'target', 'subgraph', 'id<');
	select pgr_brokenGraph('e03.calles', 'gid', 'salida', 'target', 'subgraph', 'id<10');
	select pgr_brokenGraph('e03.calles', 'gid', 'salida', 'destino', 'subgraph', 'id<10 AND id>100');

	-- When using the named notation
	select pgr_brokenGraph('e03.calles', target:='destino', subgraph:='subgraph', id:='gido');
	select pgr_brokenGraph('e03.calles', target:='destino', subgraph:='subgraph', id:='gido', source:='salido');
	select pgr_brokenGraph(rows_where:='gido<100', id:='gido', source:='salida', 'e03.calles', target:='destino', subgraph:='subgraph');

-- The following should return "rows_where condition generated 0 rows"

	select pgr_brokenGraph('ways', 'id', 'source', 'target', 'subgraph', 'id<10 AND id>100');
	select pgr_brokenGraph('e03.calles', id:='gido', rows_where:='gido<100 AND gido>200', source:='salida', target:='destino', subgraph:='subgraph');
