.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_alphashape:

pgr_alphaShape
===============================================================================

.. index:: 
	single: pgr_alphashape(text,float8)

Name
-------------------------------------------------------------------------------

``pgr_alphaShape`` — Core function for alpha shape computation.


Synopsis
-------------------------------------------------------------------------------

Returns a table with (x, y) rows that describe the vertices of an alpha shape.

.. code-block:: sql

	table() pgr_alphaShape(text sql [, float8 alpha]);


Description
-------------------------------------------------------------------------------

:sql: ``text`` a SQL query, which should return a set of rows with the following columns:

    .. code-block:: sql

        SELECT id, x, y FROM vertex_table

    :id: ``int4`` identifier of the vertex
    :x: ``float8`` x-coordinate
    :y: ``float8`` y-coordinate

:alpha: (optional) ``float8`` alpha value. If specified alpha value equals 0 (default), then optimal alpha value is used.
    For more information, see `CGAL - 2D Alpha Shapes <http://doc.cgal.org/latest/Alpha_shapes_2/group__PkgAlphaShape2.html>`_.

Returns a vertex record for each row:

:x: x-coordinate
:y: y-coordinate

If a result includes multiple outer/inner rings, return those with separator row (x=NULL and y=NULL).

.. rubric:: History

* Renamed in version 2.0.0
* Added alpha argument with default 0 (use optimal value) in version 2.1.0
* Supported to return multiple outer/inner ring coordinates with separator row (x=NULL and y=NULL) in version 2.1.0

Examples
-------------------------------------------------------------------------------
In the alpha shape code we have no way to control the order of the points so the actual output you might get could be similar but different. The simple query is followed by a more complex one that constructs a polygon and computes the areas of it. This should be the same as the result on your system. We leave the details of the complex query to the reader as an exercise if they wish to decompose it into understandable pieces or to just copy and paste it into a SQL window to run.

.. code-block:: sql

    SELECT * FROM pgr_alphaShape('SELECT id, x, y FROM vertex_table');

     x | y 
    ---+---
     2 | 4
     0 | 2
     2 | 0
     4 | 1
     4 | 2
     4 | 3
    (6 rows)

    SELECT round(ST_Area(ST_MakePolygon(ST_AddPoint(foo.openline, ST_StartPoint(foo.openline))))::numeric, 2) AS st_area
    FROM (SELECT ST_MakeLine(points ORDER BY id) AS openline FROM
    (SELECT ST_MakePoint(x, y) AS points, row_number() over() AS id
    FROM pgr_alphaShape('SELECT id, x, y FROM vertex_table')
    ) AS a) AS foo;

     st_area
    ---------
       10.00
    (1 row)


    SELECT * FROM pgr_alphaShape('SELECT id::integer, ST_X(the_geom)::float AS x, ST_Y(the_geom)::float AS y FROM edge_table_vertices_pgr');
      x  |  y  
    -----+-----
       2 |   4
     0.5 | 3.5
       0 |   2
       2 |   0
       4 |   1
       4 |   2
       4 |   3
     3.5 |   4
    (8 rows)

    SELECT round(ST_Area(ST_MakePolygon(ST_AddPoint(foo.openline, ST_StartPoint(foo.openline))))::numeric, 2) AS st_area
    FROM (SELECT ST_MakeLine(points ORDER BY id) AS openline FROM
    (SELECT ST_MakePoint(x, y) AS points, row_number() over() AS id
    FROM pgr_alphaShape('SELECT id::integer, ST_X(the_geom)::float AS x, ST_Y(the_geom)::float AS y FROM edge_table_vertices_pgr')
    ) AS a) AS foo;

     st_area
    ---------
       11.75
    (1 row)

 
The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_driving_distance` - Driving Distance
* :ref:`pgr_points_as_polygon` - Polygon around set of points
