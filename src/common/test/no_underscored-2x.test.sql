
set client_min_messages to notice;
select 1, sname = 'public' and tname = 'edges2' from pgr_getTableName('EDGes2');
select 2, sname = 'public' and tname is null from pgr_getTableName('EDes2');

select 3,  pgr_getColumnName('EDGes2', 'SOuRce') = 'source';
select 4,  pgr_getColumnName('EDes2', 'SOuRce') is null;
select 5,  pgr_getColumnName('EDes2', 'SOuce') is null;
select 6,  pgr_getColumnName('EDes2', 'SOuce') is null;

select 7, pgr_isColumnInTable('EDGes2', 'SOuRce') = true;
select 8, pgr_isColumnInTable('EDes2', 'SOuRce');
select 9, pgr_isColumnInTable('EDes2', 'SOuce');
select 10, pgr_isColumnInTable('EDes2', 'SOuce');

select 11, pgr_isColumnIndexed('EDGes2', 'eid') = true;
select 12, pgr_isColumnIndexed('EDGes2', 'X1');

select 13, pgr_versionless('2.1.0foobar23', '2.1') = true;
select 14, pgr_versionless('2.1.0foobar23', '2.1-rc1') = true;
select 15, pgr_versionless('2.1.0foobar23', '2.1-beta') = true;

select 16, pgr_quote_ident('idname.text') = 'idname.text';

select 17, pgr_startPoint(the_geom) = '010100000000000000000000400000000000000000' from edges2 where eid = 1;
select 18, pgr_endPoint(the_geom) = '01010000000000000000000040000000000000F03F'  from edges2 where eid = 1;


