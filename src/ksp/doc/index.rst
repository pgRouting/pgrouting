.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _ksp:

pgr_ksp - K-Shortest Path
===============================================================================

.. index:: 
  single: pgr_ksp(text,integer,integer,integer,boolean)
  module: ksp

Name
-------------------------------------------------------------------------------

``pgr_ksp`` — Returns the "K" shortest paths.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that make up a path.

.. code-block:: sql

  pgr_costResult[] pgr_ksp(sql text, source integer, target integer, paths integer, has_rcost boolean);


Description
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

  .. code-block:: sql

    SELECT id, source, target, cost, [,reverse_cost] FROM edge_table


  :id: ``int4`` identifier of the edge
  :source: ``int4`` identifier of the source vertex
  :target: ``int4`` identifier of the target vertex
  :cost: ``float8`` value, of the edge traversal cost. A negative cost will prevent the edge from being inserted in the graph.
  :reverse_cost: (optional) the cost for the reverse traversal of the edge. This is only used when ``has_rcost`` the parameter is ``true`` (see the above remark about negative costs).

:source: ``int4`` id of the start point
:target: ``int4`` id of the end point
:paths: ``int4`` number of alternative routes
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   edge ID
:id2:   route ID
:cost:  cost to traverse ``id1``

KSP code base taken from http://code.google.com/p/k-shortest-paths/source.


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Without ``reverse_cost``

.. code-block:: sql

  SELECT seq, id1 AS node, id2 AS edge, cost 
    FROM pgr_ksp(
      'SELECT id, source, target, cost FROM edge_table',
      7, 12, 2, false
    );


* With ``reverse_cost``

.. code-block:: sql

  SELECT seq, id1 AS node, id2 AS edge, cost 
    FROM pgr_ksp(
      'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      7, 12, 2, true
    );

The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`

