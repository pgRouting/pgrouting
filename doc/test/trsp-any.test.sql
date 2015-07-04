------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_pgr_trsp
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------


SELECT seq, id1 AS node, id2 AS edge, cost
        FROM pgr_trsp(
                'SELECT id, source, target, cost FROM edge_table',
                7, 12, false, false
        );

CREATE TABLE restrictions (
    rid serial,
    to_cost double precision,
    to_edge integer,
    FROM_edge integer,
    via text
);

INSERT INTO restrictions VALUES (1,100,7,4,null);
INSERT INTO restrictions VALUES (2,4,8,3,5);
INSERT INTO restrictions VALUES (3,100,9,16,null);

   SELECT seq, id1 AS node, id2 AS edge, cost
           FROM pgr_trsp(
                   'SELECT id, source, target, cost FROM edge_table',
                   7, 12, false, false,
                   'SELECT to_cost, to_edge AS target_id,
              FROM_edge || coalesce('','' || via, '''') AS via_path
          FROM restrictions'
           );
