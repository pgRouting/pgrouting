.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_match_line_as_linestring:

pgr_matchLineAsLinestring - Match line with network and return linestring
===============================================================================

.. index:: 
  single: pgr_matchLineAsLinestring(varchar,geometry,double precision,double precision,boolean,boolean)
  module: matching

Name
-------------------------------------------------------------------------------

``pgr_matchLineAsLinestring`` — Matches a given line to an existing network and returns a single linestring geometry.


Synopsis
-------------------------------------------------------------------------------

Returns a single (multi)linestring (GEOMETRY).

.. code-block:: sql

    geometry pgr_matchLineAsLinestring(varchar geom_table, geometry line, double precision dist1, double precision dist2, boolean directed, boolean has_rcost);


Description
-------------------------------------------------------------------------------

:geom_table: ``varchar`` network table name (may contain the schema name as well)
:line: ``geometry`` representation of the line
:dist1: ``float8`` distance for nearest node search
:dist2: ``float8`` distance for shortest path search
:directed: ``true`` if the graph is directed
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

  [TBD]


See Also
-------------------------------------------------------------------------------

* [TBD]


