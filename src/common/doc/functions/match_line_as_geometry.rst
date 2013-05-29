.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_match_line_as_geometry:

pgr_matchLineAsGeometry - Match line with network and return geometry
===============================================================================

.. index:: 
  single: pgr_matchLineAsGeometry(varchar,geometry,double precision,double precision,boolean,boolean)
  module: matching

Name
-------------------------------------------------------------------------------

``pgr_matchLineAsGeometry`` — Matches a given line to an existing network and returns the path geometry.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`pgr_geomResult <type_geom_result>` (seq, id1, id2, cost) rows, that make up a path.

.. code-block:: sql

    pgr_geomResult[] pgr_matchLineAsGeometry(varchar geom_table, geometry line, double precision dist1, double precision dist2, boolean directed, boolean has_rcost);


Description
-------------------------------------------------------------------------------

:geom_table: ``varchar`` network table name (may contain the schema name as well)
:line: ``geometry`` representation of the line
:dist1: ``float8`` distance for nearest node search
:dist2: ``float8`` distance for shortest path search
:directed: ``true`` if the graph is directed
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

Returns set of :ref:`type_geom_result`:

:seq:   row sequence
:id1:   node ID
:id2:   edge ID (``-1`` for the last row)
:geom:  geometry attribute


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

  [TBD]


See Also
-------------------------------------------------------------------------------

* :ref:`type_geom_result`
