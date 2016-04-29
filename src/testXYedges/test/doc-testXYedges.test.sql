SELECT * FROM pgr_testXYedges(
    'SELECT id, source, target, cost, reverse_cost, x1, x2, y1, y2 FROM edge_table'
);
