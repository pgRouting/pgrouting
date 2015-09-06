set log_min_messages='NOTICE';  /** hack to force EDB to log so uses hack elog for test **/
SELECT seq, id1 AS node, id2 AS edge, cost FROM pgr_trsp('SELECT id, source, target, cost FROM edge_table_i244',7, 12, false, false);
