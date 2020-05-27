SET extra_float_digits=-3;

CREATE TABLE restrictions2 (
    rid integer NOT NULL,
    to_cost double precision,
    teid integer,
    feid integer,
    via text
);

COPY restrictions2 (rid, to_cost, teid, feid, via) FROM stdin WITH NULL '__NULL__' DELIMITER ',';
1,100,7,4,__NULL__
2,4,8,3,5
3,100,9,16,__NULL__
\.

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

select * from pgr_trsp(
    'select id::integer, source::integer, target::integer,cost, reverse_cost from edge_table ORDER BY ID',
    1,    -- edge_id for start
    0.5,  -- midpoint of edge
    6,    -- edge_id of route end
    0.5,  -- midpoint of edge
    true, -- directed graph?
    true, -- has_reverse_cost?
    -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions2');

