select * from pgr_astar('
    SELECT unnest(array[1,2]) as id,
    unnest(array[10,10]) as source,
    unnest(array[20,20]) as target,
    unnest(array[0,1])::float8 as x1,
    unnest(array[0,1])::float8 as x2,
    unnest(array[0,1])::float8 as y1,
    unnest(array[0,1])::float8 as y2,
    unnest(array[2,1])::float8 as cost
    '::text, 10, 20, false, false);
