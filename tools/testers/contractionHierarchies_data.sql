------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              SAMPLE DATA FOR CONTRACTION HIERARCHIES
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/



-- Sample graph #2
DROP TABLE IF EXISTS edges_2;
DROP TABLE IF EXISTS vertices_2;

create table vertices_2 (
    id integer primary key,
    x double precision,
    y double precision,
    geom Geometry(Point)
);

create table edges_2 (
    id bigserial primary key,
    source bigint,
    target bigint,
    cost float,
    reverse_cost float,
    geom geometry(Linestring)
);


/*---------------------------------------------------------------
 1_______11_____9_____7
 | \      \    / \   /|
 |  \      \  /   \ / |
 |   3______10_____8  |
 |  /  \   / |      \ |
 | /    \ /  |       \|
 2/______4___5________6
1-11 = cost 3 ;
11-9 = cost 6 ;
1-3 = cost 5 ;
1-2 = cost 3 ;
2-4 = cost 5 ;
2-3 = cost 3 ;
3-4 = cost 2 ;
4-5 = cost 7 ;
5-6 = cost 6 ;
3-10 = cost 2 ;
10-4 = cost 4 ;
10-5 = cost 3 ;
10-11 = cost 3 ;
10-9 = cost 4 ;
10-8 = cost 2 ;
8-6 = cost 2 ;
8-7 = cost 3 ;
7-6 = cost 4 ;
9-8 = cost 3 ;
9-7 = cost 5 ;
1(0,2)
2(0,0)
3(0.5,1)
4(1,0)
5(1.5,0)
6(3,0)
7(3,1)
8(2.5,1)
9(2,2)
10(1.5,1)
11(1,2)
*/

insert into vertices_2 (id, x, y, geom)
values
    ( 1, 0, 2, st_point(0,2) ),
    ( 2, 0, 0, st_point(0,0) ),
    ( 3, 0.5, 1, st_point(0.5, 1) ),
    ( 4, 1, 0, st_point(1, 0) ),
    ( 5, 1.5, 0, st_point(1.5, 0) ),
    ( 6, 3, 0, st_point(3, 0) ),
    ( 7, 3, 1, st_point(3, 1) ),
    ( 8, 2.5, 1, st_point(2.5, 1) ),
    ( 9, 2, 2, st_point(2, 2) ),
    ( 10, 1.5, 1, st_point(1.5, 1) ),
    ( 11, 1, 2, st_point(1, 2) );

insert into edges_2 (source, target, cost, reverse_cost, geom) 
values
    ( 1, 11, 3, 3, ST_MakeLine(ST_Point(0,2), ST_Point(1,2)) ),
    ( 11, 9, 6, 6, ST_MakeLine(ST_Point(1,2), ST_Point(2,2)) ),
    ( 1, 3, 5, 5, ST_MakeLine(ST_Point(0,2), ST_Point(0.5,1)) ),
    ( 1, 2, 3, 3, ST_MakeLine(ST_Point(0,2), ST_Point(0,0)) ), --1-2
    ( 2, 4, 5, 5, ST_MakeLine(ST_Point(0,0), ST_Point(1,0)) ), --2-4
    ( 2, 3, 3, 3, ST_MakeLine(ST_Point(0,0), ST_Point(0.5,1)) ), --2-3
    ( 3, 4, 2, 2, ST_MakeLine(ST_Point(0.5,1), ST_Point(1,0)) ), --3-4
    ( 4, 5, 7, 7, ST_MakeLine(ST_Point(1,0), ST_Point(1.5,0)) ), --4-5
    ( 5, 6, 6, 6, ST_MakeLine(ST_Point(1.5,0), ST_Point(3,0)) ), --5-6
    ( 3, 10, 2, 2, ST_MakeLine(ST_Point(0.5,1), ST_Point(1.5,1)) ), --3-10
    ( 10, 4, 4, 4, ST_MakeLine(ST_Point(1.5,1), ST_Point(1,0)) ), --10-4
    ( 10, 5, 3, 3, ST_MakeLine(ST_Point(1.5,1), ST_Point(1.5,0)) ), --10-5
    ( 10, 11, 3, 3, ST_MakeLine(ST_Point(1.5,1), ST_Point(1,2)) ), --10-11
    ( 10, 9, 4, 4, ST_MakeLine(ST_Point(1.5,1), ST_Point(2,2)) ), --10-9
    ( 10, 8, 2, 2, ST_MakeLine(ST_Point(1.5,1), ST_Point(2.5,1)) ), --10-8
    ( 8, 6, 2, 2, ST_MakeLine(ST_Point(2.5,1), ST_Point(3,0)) ), --8-6
    ( 8, 7, 3, 3, ST_MakeLine(ST_Point(2.5,1), ST_Point(3,1)) ), --8-7
    ( 7, 6, 4, 4, ST_MakeLine(ST_Point(3,1), ST_Point(3,0)) ), --7-6
    ( 9, 7, 5, 5, ST_MakeLine(ST_Point(2,2), ST_Point(3,1)) ), --9-7
    ( 9, 8, 3, 3, ST_MakeLine(ST_Point(2,2), ST_Point(2.5,1)) ); --9-8
