DROP TABLE IF EXISTS category;
CREATE TABLE category (
	category text PRIMARY KEY,
	capacity BIGINT
);

INSERT INTO category VALUES ('Motorway', 130), ('Primary', 100), ('Regional', 80), ('Local', 50);

ALTER TABLE edge_table ADD COLUMN category TEXT;
ALTER TABLE edge_table ADD COLUMN reverse_category TEXT;

UPDATE edge_table SET category = 'Regional', reverse_category = 'Motorway' WHERE id = 1;
UPDATE edge_table SET category = 'Regional', reverse_category = 'Primary' WHERE id = 2;
UPDATE edge_table SET category = 'Primary', reverse_category = 'Motorway' WHERE id = 3;
UPDATE edge_table SET category = 'Primary', reverse_category = 'Local' WHERE id = 4;
UPDATE edge_table SET category = 'Motorway', reverse_category = 'Local' WHERE id = 5;
UPDATE edge_table SET category = 'Local', reverse_category = 'Primary' WHERE id = 6;
UPDATE edge_table SET category = 'Local', reverse_category = 'Motorway' WHERE id = 7;
UPDATE edge_table SET category = 'Primary', reverse_category = 'Motorway' WHERE id = 8;
UPDATE edge_table SET category = 'Motorway', reverse_category = 'Regional' WHERE id = 9;
UPDATE edge_table SET category = 'Motorway', reverse_category = 'Local' WHERE id = 10;
UPDATE edge_table SET category = 'Motorway', reverse_category = 'Primary' WHERE id = 11;
UPDATE edge_table SET category = 'Primary', reverse_category = 'Regional' WHERE id = 12;
UPDATE edge_table SET category = 'Primary', reverse_category = 'Local' WHERE id = 13;
UPDATE edge_table SET category = 'Regional', reverse_category = 'Motorway' WHERE id = 14;
UPDATE edge_table SET category = 'Regional', reverse_category = 'Local' WHERE id = 15;
UPDATE edge_table SET category = 'Regional', reverse_category = 'Regional' WHERE id = 16;
UPDATE edge_table SET category = 'Motorway', reverse_category = 'Primary' WHERE id = 17;
UPDATE edge_table SET category = 'Local', reverse_category = 'Motorway' WHERE id = 18;


SELECT * FROM pgr_maxflowpushrelabel('SELECT id, source, target, c1.capacity as cost, c2.capacity as reverse_cost
FROM edge_table AS edges, category as c1, category as c2
WHERe edges.category = c1.category AND edges.reverse_category = c2.category', 3, 5);


