

    CREATE TABLE routing (gid serial, length integer, source integer, target integer, reverse_cost integer);
    INSERT INTO routing (length, source, target, reverse_cost) VALUES
     (10000, 1, 2, 10000),
     (5, 3, 1, 5),
     (10, 4, 3, 10),
     (10, 2, 4, 10);

    SELECT pgr_trsp(
          'SELECT gid as id, source::int4, target::int4, length::float8 as cost, length::float8 as reverse_cost FROM routing',
          1, 0.1, 1, 0.9, false, true
    );

