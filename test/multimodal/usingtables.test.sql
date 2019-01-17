
create table layers_info(
  layer_name text,
  layer_from_query text,
  conn_policy integer,
  z integer
);
create table layer (
  id integer,
  geom geometry,
  z_start integer,
  z_end integer
);
