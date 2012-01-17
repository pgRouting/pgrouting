CREATE TYPE link_point AS (id integer, name varchar);

-------------------------------------------------------------------
-- This function finds nearest link to a given node
-- point - text representation of point
-- distance - function will search for a link within this distance
-- tbl - table name
-------------------------------------------------------------------
CREATE OR REPLACE FUNCTION find_nearest_link_within_distance(point varchar, 
	distance double precision, tbl varchar)
	RETURNS INT AS
$$
DECLARE
    row record;
    x float8;
    y float8;
    
    srid integer;
    
BEGIN

    FOR row IN EXECUTE 'select getsrid(the_geom) as srid from '||tbl||' where gid = (select min(gid) from '||tbl||')' LOOP
    END LOOP;
	srid:= row.srid;
    
    -- Getting x and y of the point
    
    FOR row in EXECUTE 'select ST_X(ST_GeometryFromText('''||point||''', '||srid||')) as x' LOOP
    END LOOP;
	x:=row.x;

    FOR row in EXECUTE 'select ST_Y(ST_GeometryFromText('''||point||''', '||srid||')) as y' LOOP
    END LOOP;
	y:=row.y;

    -- Searching for a link within the distance

    FOR row in EXECUTE 'select gid, ST_Distance(the_geom, ST_GeometryFromText('''||point||''', '||srid||')) as dist from '||tbl||
			    ' where ST_SetSRID(''BOX3D('||x-distance||' '||y-distance||', '||x+distance||' '||y+distance||')''::BOX3D, '||srid||')&&the_geom order by dist asc limit 1'
    LOOP
    END LOOP;

    IF row.gid IS NULL THEN
	    --RAISE EXCEPTION 'Data cannot be matched';
	    RETURN NULL;
    END IF;

    RETURN row.gid;

END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT;

-------------------------------------------------------------------
-- This function finds nearest node to a given node
-- point - text representation of point
-- distance - function will search for a link within this distance
-- tbl - table name
-------------------------------------------------------------------

CREATE OR REPLACE FUNCTION find_nearest_node_within_distance(point varchar, 
	distance double precision, tbl varchar)
	RETURNS INT AS
$$
DECLARE
    row record;
    x float8;
    y float8;
    d1 double precision;
    d2 double precision;
    d  double precision;
    field varchar;

    node integer;
    source integer;
    target integer;
    
    srid integer;
    
BEGIN

    FOR row IN EXECUTE 'select getsrid(the_geom) as srid from '||tbl||' where gid = (select min(gid) from '||tbl||')' LOOP
    END LOOP;
	srid:= row.srid;

    -- Getting x and y of the point

    FOR row in EXECUTE 'select ST_X(ST_GeometryFromText('''||point||''', '||srid||')) as x' LOOP
    END LOOP;
	x:=row.x;

    FOR row in EXECUTE 'select ST_Y(ST_GeometryFromText('''||point||''', '||srid||')) as y' LOOP
    END LOOP;
	y:=row.y;

    -- Getting nearest source

    FOR row in EXECUTE 'select source, ST_Distance(ST_StartPoint(the_geom), ST_GeometryFromText('''||point||''', '||srid||')) as dist from '||tbl||
			    ' where ST_SetSRID(''BOX3D('||x-distance||' '||y-distance||', '||x+distance||' '||y+distance||')''::BOX3D, '||srid||')&&the_geom order by dist asc limit 1'
    LOOP
    END LOOP;
    
    d1:=row.dist;
    source:=row.source;

    -- Getting nearest target

    FOR row in EXECUTE 'select target, ST_Distance(ST_EndPoint(the_geom), ST_GeometryFromText('''||point||''', '||srid||')) as dist from '||tbl||
			    ' where ST_SetSRID(''BOX3D('||x-distance||' '||y-distance||', '||x+distance||' '||y+distance||')''::BOX3D, '||srid||')&&the_geom order by dist asc limit 1'
    LOOP
    END LOOP;

    -- Checking what is nearer - source or target
    
    d2:=row.dist;
    target:=row.target;
    IF d1<d2 THEN
	node:=source;
        d:=d1;
    ELSE
	node:=target;
        d:=d2;
    END IF;

    IF d=NULL OR d>distance THEN
        node:=NULL;
    END IF;

    RETURN node;

END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT;

-------------------------------------------------------------------
-- This function finds nearest node as a source or target of the
-- nearest link
-- point - text representation of point
-- distance - function will search for a link within this distance
-- tbl - table name
-------------------------------------------------------------------

CREATE OR REPLACE FUNCTION find_node_by_nearest_link_within_distance(point varchar, 
	distance double precision, tbl varchar)
	RETURNS link_point AS
$$
DECLARE
    row record;
    link integer;
    d1 double precision;
    d2 double precision;
    field varchar;
    res link_point;
    
    srid integer;
BEGIN

    FOR row IN EXECUTE 'select getsrid(the_geom) as srid from '||tbl||' where gid = (select min(gid) from '||tbl||')' LOOP
    END LOOP;
	srid:= row.srid;


    -- Searching for a nearest link
    
    FOR row in EXECUTE 'select id from find_nearest_link_within_distance('''||point||''', '||distance||', '''||tbl||''') as id'
    LOOP
    END LOOP;
    IF row.id is null THEN
        res.id = -1;
        RETURN res;
    END IF;
    link:=row.id;

    -- Check what is nearer - source or target
    
    FOR row in EXECUTE 'select ST_Distance((select ST_StartPoint(the_geom) from '||tbl||' where gid='||link||'), ST_GeometryFromText('''||point||''', '||srid||')) as dist'
    LOOP
    END LOOP;
    d1:=row.dist;

    FOR row in EXECUTE 'select ST_Distance((select ST_EndPoint(the_geom) from '||tbl||' where gid='||link||'), ST_GeometryFromText('''||point||''', '||srid||')) as dist'
    LOOP
    END LOOP;
    d2:=row.dist;

    IF d1<d2 THEN
	field:='source';
    ELSE
	field:='target';
    END IF;
    
    FOR row in EXECUTE 'select '||field||' as id, '''||field||''' as f from '||tbl||' where gid='||link
    LOOP
    END LOOP;
        
    res.id:=row.id;
    res.name:=row.f;
    
    RETURN res;


END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT;

-------------------------------------------------------------------
-- This function matches given line to the existing network.
-- Returns set of edges as geometry.
-- tbl - table name
-- line - line to match
-- distance - distance for nearest node search
-- distance2 - distance for shortest path search
-- dir - true if your network graph is directed
-- rc - true if you have a reverse_cost column
-------------------------------------------------------------------

CREATE OR REPLACE FUNCTION match_line_as_geometry(tbl varchar, line geometry, distance double precision, 
						distance2 double precision, dir boolean, rc boolean)
	RETURNS SETOF GEOMS AS
$$
DECLARE
    row record;
    num integer;
    i integer;
    geom geoms;
    points integer[];
    
    srid integer;
    
    query text;
    
BEGIN

    FOR row IN EXECUTE 'select getsrid(the_geom) as srid from '||tbl||' where gid = (select min(gid) from '||tbl||')' LOOP
    END LOOP;
	srid:= row.srid;


    FOR row IN EXECUTE 'select geometryType(ST_GeometryFromText('''||astext(line)||''', '||srid||')) as type' LOOP
    END LOOP;
    
    IF row.type <> 'LINESTRING' THEN
	RAISE EXCEPTION 'Geometry should be a linestring.';
    END IF;
    
    -- Searching through all points in given line
    
    num:=NumPoints(line);
    i:= 0;
    
    LOOP
	i:=i+1;

        -- Getting nearest node to the current point
	
	FOR row in EXECUTE 'select * from find_nearest_node_within_distance(''POINT('
			    ||ST_X(ST_PointN(line, i))||' '||ST_Y(PointN(line, i))||')'','||distance||', '''||tbl||''') as id'
	LOOP
	END LOOP;
	
	IF row.id IS NOT NULL THEN
	    points[i-1]:=row.id;

        ELSE 
	
	    -- If there is no nearest node within given distance, let's try another algorithm
	
            FOR row in EXECUTE 'select * from find_node_by_nearest_link_within_distance(''POINT('
	    		        ||ST_X(ST_PointN(line, i))||' '||ST_Y(PointN(line, i))||')'','||distance2||', '''||tbl||''') as id'
	    LOOP
	    END LOOP;

	    points[i-1]:=row.id;

        END IF;

	IF i>1 AND points[i-2] <> points[i-1] THEN
	
	    -- We could find existing edge, so let's construct the main query now
	
	    query := 'select gid, the_geom FROM shortest_path( ''select gid as id, source::integer,'||
				' target::integer, length::double precision as cost,x1,x2,y1,y2';
				
	    IF rc THEN query := query || ', reverse_cost'; 
	    END IF;				
				
	    query := query || ' from '||quote_ident(tbl)||' where setsrid(''''BOX3D('||ST_X(ST_PointN(line, i-1))-distance2*2||' '
				||ST_Y(ST_PointN(line, i-1))-distance2*2||', '||ST_X(ST_PointN(line, i))+distance2*2||' '
				||ST_Y(ST_PointN(line, i))+distance2*2||')''''::BOX3D, '||srid||')&&the_geom'', '
				|| points[i-1] ||', '||	points[i-2] ||', '''||dir||''', '''||rc||'''), '
				||quote_ident(tbl)||' where edge_id=gid';
	    FOR row IN EXECUTE query
	    LOOP

		geom.gid := row.gid;
		geom.the_geom := row.the_geom;
		
		RETURN NEXT geom;
		
	    END LOOP;

	END IF;															


	EXIT WHEN i=num;
	
	
    END LOOP;    
    
    RETURN;

END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;

-------------------------------------------------------------------
-- This function matches given line to the existing network.
-- Returns set of edges.
-- tbl - table name
-- line - line to match
-- distance - distance for nearest node search
-- distance2 - distance for shortest path search
-- dir - true if your network graph is directed
-- rc - true if you have a reverse_cost column
-------------------------------------------------------------------

CREATE OR REPLACE FUNCTION match_line(tbl varchar, line geometry, distance double precision, 
						distance2 double precision, dir boolean, rc boolean)
	RETURNS SETOF PATH_RESULT AS
$$
DECLARE
    row record;
    num integer;
    
    i integer;
    z integer;
    t integer;
    
    prev integer;
    
    query text;
    
    path path_result;
    
    edges integer[];
    vertices integer[];
    costs double precision[];
    
    srid integer;
    
    points integer[];

BEGIN

    FOR row IN EXECUTE 'select getsrid(the_geom) as srid from '||tbl||' where gid = (select min(gid) from '||tbl||')' LOOP
    END LOOP;
	srid:= row.srid;

    FOR row IN EXECUTE 'select geometryType(ST_GeometryFromText('''||astext(line)||''', '||srid||')) as type' LOOP
    END LOOP;
    
    IF row.type <> 'LINESTRING' THEN
	RAISE EXCEPTION 'Geometry should be a linestring.';
    END IF;

    num:=ST_NumPoints(line);
    i:= 0;
    z:= 0;
    prev := -1;
        
    -- Searching through all points in given line

    LOOP
	i:=i+1;

        -- Getting nearest node to the current point

        FOR row in EXECUTE 'select * from find_nearest_node_within_distance(''POINT('
			    ||ST_X(ST_PointN(line, i))||' '||ST_Y(ST_PointN(line, i))||')'','||distance||', '''||tbl||''') as id'
	LOOP
	END LOOP;
	

	IF row.id IS NOT NULL THEN
	    points[i-1]:=row.id;

        ELSE 

	    -- If there is no nearest node within given distance, let's try another algorithm

            FOR row in EXECUTE 'select * from find_node_by_nearest_link_within_distance(''POINT('
	    		        ||ST_X(ST_PointN(line, i))||' '||ST_Y(ST_PointN(line, i))||')'','||distance2||', '''||tbl||''') as id'
	    LOOP
	    END LOOP;

	    points[i-1]:=row.id;
            IF row.id = -1 THEN
                return;
            END IF;

        END IF;

	IF i>1 AND points[i-2] <> points[i-1] THEN
	
	    -- We could find existing edge, so let's construct the main query now

	    query := 'select edge_id, vertex_id, cost FROM shortest_path( ''select gid as id, source::integer,'||
				' target::integer, length::double precision as cost,x1,x2,y1,y2 ';
				
	    IF rc THEN query := query || ', reverse_cost'; 
	    END IF;
	    
	    query := query || ' from '||quote_ident(tbl)||' where setsrid(''''BOX3D('||ST_X(ST_PointN(line, i-1))-distance2*2||' '
				||ST_Y(ST_PointN(line, i-1))-distance2*2||', '||ST_X(ST_PointN(line, i))+distance2*2||' '
				||ST_Y(ST_PointN(line, i))+distance2*2||')''''::BOX3D, '||srid||')&&the_geom'', '
				|| points[i-1] ||', '||	points[i-2] ||', '''||dir||''', '''||rc||''')';

	    
	    BEGIN
	    
	    FOR row IN EXECUTE query
	    LOOP
	    
	    
		IF row IS NULL THEN
		    RAISE NOTICE 'Cannot find a path between % and %', points[i-1], points[i-2];
	    	    RETURN;
		END IF;

		edges[z] := row.edge_id;
		vertices[z] := row.vertex_id;
		costs[z] := row.cost;
		
		IF edges[z] = -1 THEN
		
		    t := 0;
		    
		    -- Ordering edges
		    
		    FOR t IN (prev+1)..z-1 LOOP
		    
			path.edge_id := edges[t];
			path.vertex_id := vertices[t];
			path.cost = costs[t];
			
			edges[t] := edges[z-t+prev+1];
			vertices[t] := vertices[z-t+prev+1];
			costs[t] := costs[z-t+prev+1];

			edges[z-t+prev+1] := path.edge_id;
			vertices[z-t+prev+1] := path.vertex_id;
			costs[z-t+prev+1] := path.cost;
			
					    
		    END LOOP;
		    
		    prev := z;

		END IF;	
		
		z := z+1;
		
	    END LOOP;
	    
	    EXCEPTION
		WHEN containing_sql_not_permitted THEN RETURN;
	    
	    END;

	END IF;															

	EXIT WHEN i=num;
	
    END LOOP;    

    FOR t IN 0..array_upper(edges, 1) LOOP
    
	IF edges[array_upper(edges, 1)-t] > 0 OR (edges[array_upper(edges, 1)-t] < 0 AND t = array_upper(edges, 1)) THEN
	path.edge_id := edges[array_upper(edges, 1)-t];
	path.vertex_id := vertices[array_upper(edges, 1)-t];
	path.cost = costs[array_upper(edges, 1)-t];
	RETURN NEXT path;	
	END IF;
    END LOOP;
    
    RETURN;

END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;

-------------------------------------------------------------------
-- This function matches given line to the existing network.
-- Returns single (multi)linestring.
-- tbl - table name
-- line - line to match
-- distance - distance for nearest node search
-- distance2 - distance for shortest path search
-- dir - true if your network graph is directed
-- rc - true if you have a reverse_cost column
-------------------------------------------------------------------

CREATE OR REPLACE FUNCTION match_line_as_linestring(tbl varchar, line geometry, distance double precision, 
						distance2 double precision, dir boolean, rc boolean)
	RETURNS GEOMETRY AS
$$
DECLARE
    row record;
    
    i integer;
    
    edges integer[];
    
    srid integer;
    
BEGIN

    FOR row IN EXECUTE 'select getsrid(the_geom) as srid from '||tbl||' where gid = (select min(gid) from '||tbl||')' LOOP
    END LOOP;
	srid:= row.srid;

    FOR row IN EXECUTE 'select geometryType(ST_GeometryFromText('''||astext(line)||''', '||srid||')) as type' LOOP
    END LOOP;
    
    IF row.type <> 'LINESTRING' THEN
	RAISE EXCEPTION 'Geometry should be a linestring.';
    END IF;

    i := 0;
    
    FOR row IN EXECUTE 'select * from match_line('''||quote_ident(tbl)||''', ST_GeometryFromText('''||astext(line)||''', '||srid||'), '
			    ||distance||', '||distance2||', '''||dir||''', '''||rc||''')' LOOP
	edges[i] := row.edge_id;
	i := i + 1;
    END LOOP;
    IF i = 0 THEN
        return NULL;
    END IF;

    -- Attempt to create a single linestring. It may return multilinestring as well.

    FOR row IN EXECUTE 'select ST_LineMerge(ST_Union(ST_Multi(the_geom))) as the_geom from '||tbl||' where gid in ('||array_to_string(edges, ', ')||') and gid > 0' LOOP
    END LOOP;
    
    IF isvalid(row.the_geom) THEN
        RETURN row.the_geom;
    ELSE
	RAISE EXCEPTION 'The result is not a valid geometry.';
    END IF;

END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;