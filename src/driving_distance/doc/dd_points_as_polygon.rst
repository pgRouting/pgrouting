.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_points_as_polygon:

pgr_pointsAsPolygon - Polygon around set of points
===============================================================================

.. index:: 
    single: pgr_pointsAsPolygon(text)
    module: driving_distance

Name
-------------------------------------------------------------------------------

``pgr_pointsAsPolygon`` — Draws an alpha shape around given set of points.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`pgr_geomResult <type_geom_result>` (x, y) rows.

.. code-block:: sql

    pgr_geomResult[] pgr_pointsAsPolygon(text sql);


Description
-------------------------------------------------------------------------------

:sql: ``text`` a SQL query, which should return a set of rows with the following columns:

    .. code-block:: sql

        SELECT id, x, y FROM vertex_result;


    :id: ``int4`` vertex ID
    :x: ``int4`` vertex x value
    :y: ``int4`` vertex y value


Returns set of :ref:`type_geom_result`:

:seq:   "0" (not used)
:id1:   "0" (not used)
:id2:   "0" (not used)
:geom:  polygon geometry


.. rubric:: History

* Renamed in version 2.0.0


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_driving_distance`
* :ref:`type_geom_result`
