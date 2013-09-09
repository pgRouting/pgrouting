.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_points_as_polygon:

pgr_pointsAsPolygon
===============================================================================

.. index:: 
    single: pgr_pointsAsPolygon(text)
    module: driving_distance

Name
-------------------------------------------------------------------------------

``pgr_pointsAsPolygon`` — Draws an alpha shape around given set of points.


Synopsis
-------------------------------------------------------------------------------

Returns the alpha shape as polygon geometry.

.. code-block:: sql

    geometry pgr_pointsAsPolygon(text sql);


Description
-------------------------------------------------------------------------------

:sql: ``text`` a SQL query, which should return a set of rows with the following columns:

    .. code-block:: sql

        SELECT id, x, y FROM vertex_result;

    :id: ``int4`` identifier of the vertex
    :x: ``float8`` x-coordinate
    :y: ``float8`` y-coordinate


Returns a polygon geometry.


.. rubric:: History

* Renamed in version 2.0.0


Examples
-------------------------------------------------------------------------------
In the following query there is not way to control which point in the polygon is the first in the list, so you may get similar but different results than the following which are also correct. Each of the pgr_pointsAsPolygon queries below is followed by one the compute the area of the polygon. This area should remain constant regardles of the order of the points making up the polygon.

.. code-block:: sql

    SELECT ST_AsText(pgr_pointsAsPolygon('SELECT id, x, y FROM vertex_table'));

                   st_astext                
    ----------------------------------------
     POLYGON((2 0,4 1,4 2,4 3,2 4,0 2,2 0))
    (1 row)

    SELECT round(ST_Area(pgr_pointsAsPolygon('SELECT id, x, y FROM vertex_table'))::numeric, 2) as st_area;

     st_area
    ---------
       10.00
    (1 row)

    
    SELECT ST_ASText(pgr_pointsASPolygon('SELECT id::integer, st_x(the_geom)::float as x, st_y(the_geom)::float as y  
                                          FROM edge_table_vertices_pgr'));  
                        st_astext                         
    ----------------------------------------------------------
     POLYGON((0.5 3.5,0 2,2 0,4 1,4 2,4 3,3.5 4,2 4,0.5 3.5))
    (1 row)

    SELECT round(ST_Area(pgr_pointsASPolygon('SELECT id::integer, st_x(the_geom)::float as x, st_y(the_geom)::float as y 
         FROM edge_table_vertices_pgr'))::numeric, 2) as st_area;

     st_area
    ---------
       11.75


The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_driving_distance` - Driving Distance
* :ref:`pgr_alphashape` - Alpha shape computation
