CREATE OR REPLACE FUNCTION pgr_p_astar(
		sql text,
		source_vid integer,
		target_vid integer)
	RETURNS integer
	AS '/vagrant/src/partition/test/demo/p_astar', 'p_astar_shortest_path'
	LANGUAGE 'c' IMMUTABLE STRICT;


