.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_ksp_v2:

pgr_ksp - Deprecated Signature
===============================================================================

.. warning:: This function signature is deprecated!!!

    * That means they have been replaced by a new signature, this signature is no longer supported, and may be removed from future versions.
    * All code that uses the function signature should be converted to use its replacement :ref:`pgr_ksp`.


Name
-------------------------------------------------------------------------------

``pgr_ksp`` — Returns the "K" shortest paths.


Synopsis
-------------------------------------------------------------------------------

The K shortest path routing algorithm based on Yen's algorithm. "K" is the number of shortest paths desired. Returns a set of :ref:`pgr_costResult3 <type_cost_result3>` (seq, id1, id2, id3, cost) rows, that make up a path.

.. code-block:: sql

  pgr_costResult3[] pgr_ksp(sql text, source integer, target integer,
                           paths integer, has_rcost boolean);


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

:seq:   sequence for ording the results
:id1:   route ID
:id2:   node ID
:id3:   edge ID (``0`` for the last row)
:cost:  cost to traverse from ``id2`` using ``id3``

KSP code base taken from http://code.google.com/p/k-shortest-paths/source.


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Without ``reverse_cost``

.. literalinclude:: doc-ksp-v2.queries
    :start-after: --q1
    :end-before: --q2


* With ``reverse_cost``

.. literalinclude:: doc-ksp-v2.queries
    :start-after: --q2
    :end-before: --q3


The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result3`
* http://en.wikipedia.org/wiki/K_shortest_path_routing
