.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_match_line:

pgr_matchLine - Match line with network
===============================================================================

.. index:: 
	single: pgr_matchLine(varchar,geometry,double precision,double precision,boolean,boolean)
	module: matching

Name
-------------------------------------------------------------------------------

``pgr_matchLine`` — Matches a given line to an existing network.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that make up a path.

.. code-block:: sql

    pgr_costResult[] pgr_matchLine(varchar geom_table, geometry line, double precision dist1, double precision dist2, boolean directed, boolean has_rcost);


Description
-------------------------------------------------------------------------------

:geom_table: ``varchar`` network table name (may contain the schema name as well)
:line: ``geometry`` representation of the line
:dist1: ``float8`` distance for nearest node search
:dist2: ``float8`` distance for shortest path search
:directed: ``true`` if the graph is directed
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   node ID
:id2:   edge ID (``-1`` for the last row)
:cost:  cost to traverse from ``id1`` using ``id2``


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

  [TBD]


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
