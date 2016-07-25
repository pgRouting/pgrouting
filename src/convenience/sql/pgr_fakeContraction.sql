create type contraction_vertex as (seq int, id int, type char, contracted text);
create or replace function pgr_fakeContraction(edges text, ch_order integer[])
    returns setof contraction_vertex  as
$body$
declare
	cv contraction_vertex%ROWTYPE;
	row record;
begin
	if ch_order = ARRAY[1] then
		-- dealing with dead end contraction
		for row in SELECT * FROM (
			VALUES
			(1, 2, 'v', '{1}'),
			(2, 3, 'v', '{}'),
			(3, 4, 'v', '{}'),
			(4, 5, 'v', '{7, 8}'),
			(5, 6, 'v', '{}'),
			(6, 9, 'v', '{}'),
			(7, 10, 'v', '{13}'),
			(8, 11, 'v', '{}'),
			(9, 13, 'v', '{}'),
			(10, 15, 'v', '{14}'),
			(11, 17, 'v', '{16}'))  
		AS t (seq, id, type, contracted) loop
		cv.seq := row.seq;
		cv.id := row.id;
		cv.type := row.type;
		cv.contracted := row.contracted;
		return next cv;
		end loop;
	end if;
	if ch_order = ARRAY[1, 2] then
		-- dealing with dead end and linear contraction
		for row in SELECT * FROM (
			VALUES
			(1, -1, 'e', '{1, 2}'),
			(2, -2, 'e', '{4}'),
			(3, -3, 'e', '{10, 13}'),
			(4, -4, 'e', '{12}'),
			(5, 5, 'v', '{7, 8}'),
			(6, 6, 'v', '{}'),
			(7, 9, 'v', '{}'),
			(8, 11, 'v', '{}'),
			(9, 15, 'v', '{14}'),
			(10, 17, 'v', '{16}'))  
		AS t (seq, id, type, contracted) loop
		cv.seq := row.seq;
		cv.id := row.id;
		cv.type := row.type;
		cv.contracted := row.contracted;
		return next cv;
		end loop;
	end if;
end;
$body$
language 'plpgsql' ;
