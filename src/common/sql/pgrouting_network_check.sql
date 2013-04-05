/*
	Alternative to pgrouting_analytics.sql

	Example:
	SELECT * FROM data.PGR_analyze_graph('SELECT source, target, geom_way 
		AS geom FROM data.dhaka_2po_4pgr', 0.000001) WHERE checkit;
 */
CREATE OR REPLACE FUNCTION data.PGR_analyze_graph(sql text, tolerance float)
  RETURNS TABLE(
	vertex bigint, 
	source integer,
	target integer,
	checkit boolean,
	geom geometry
) AS
$BODY$
DECLARE 
	rec	record;
	pnt	record;
	seg	record;
BEGIN
	-- Create temporary vertex table
	CREATE TEMPORARY TABLE vertices_temp (
		vertex bigint  PRIMARY KEY,
		source integer DEFAULT 0,
		target integer DEFAULT 0,
		checkit boolean DEFAULT false,
		geom geometry
	) ON COMMIT DROP;

	-- Count occurance of vertex as source/target
	RAISE NOTICE 'Count occurance of vertex as source/target';
	FOR rec IN EXECUTE sql
	LOOP
		-- Source
		BEGIN
			EXECUTE 'INSERT INTO vertices_temp (vertex,geom,source) VALUES ($1,$2,1)' 
				USING rec.source, ST_Startpoint(rec.geom);
				-- This assumes that source equals start point of geometry
		EXCEPTION WHEN unique_violation THEN
			EXECUTE 'UPDATE vertices_temp SET source = source + 1 WHERE vertex = $1' 
				USING rec.source;
		END;

		-- Target
		BEGIN
			EXECUTE 'INSERT INTO vertices_temp (vertex,geom,target) VALUES ($1,$2,1)' 
				USING rec.target, ST_Endpoint(rec.geom);
				-- This assumes that target equals end point of geometry
		EXCEPTION WHEN unique_violation THEN
			EXECUTE 'UPDATE vertices_temp SET target = target + 1 WHERE vertex = $1' 
				USING rec.target;
		END;
	END LOOP;

	-- Analyze graph for gaps and zlev errors
	RAISE NOTICE 'Analyze graph for gaps and zlev errors';
	FOR pnt IN EXECUTE 'SELECT * FROM vertices_temp 
				WHERE (source + target) = 1 ORDER BY vertex'
	LOOP
		FOR seg IN EXECUTE 'SELECT * FROM (' || sql || ') a WHERE ST_DWithin(a.geom,$1,$2)'
				USING pnt.geom, tolerance
		LOOP
			IF pnt.vertex NOT IN (seg.source, seg.target) THEN
				EXECUTE 'UPDATE vertices_temp SET checkit = TRUE WHERE vertex = $1'
					USING pnt.vertex;
			END IF;
		END LOOP;
	END LOOP;

	RETURN QUERY SELECT * FROM vertices_temp;
EXCEPTION
	-- Not sure this is a good idea or not. It prevents to show "ugly" console output 
	WHEN others THEN
	RAISE EXCEPTION '%', SQLERRM;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT
  COST 100;