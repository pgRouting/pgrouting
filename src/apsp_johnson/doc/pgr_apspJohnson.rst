.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_apsp_johnson:

pgr_apspJohnson - Deprecated function
===============================================================================

.. warning:: This function is deprecated!!!

    * It has been replaced by a new functions, is no longer supported, and may be removed from future versions.
    * All code that uses this function should be converted to use its replacement: :ref:`pgr_johnson`. 


Name
-------------------------------------------------------------------------------

``pgr_apspJohnson`` - Returns all costs for each pair of nodes in the graph.


Synopsis
-------------------------------------------------------------------------------

Johnson's algorithm is a way to find the shortest paths between all pairs of vertices in a sparse, edge weighted, directed graph. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows for every pair of nodes in the graph.

.. code-block:: sql

    pgr_costResult[] pgr_apspJohnson(sql text);


Description
-------------------------------------------------------------------------------

:sql: a SQL query that should return the edges for the graph that will be analyzed:

    .. code-block:: sql

        SELECT source, target, cost FROM edge_table;

    :source: ``int4`` identifier of the source vertex for this edge
    :target: ``int4`` identifier of the target vertex for this edge
    :cost: ``float8`` a positive value for the cost to traverse this edge

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   source node ID
:id2:   target node ID
:cost:  cost to traverse from ``id1`` to ``id2``

.. rubric:: History

* Deprecated in version 2.2.0
* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. literalinclude:: doc-apspJohnson.queries
   :start-after: --q1
   :end-before: --q2

The query uses the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* :ref:`pgr_johnson`
* http://en.wikipedia.org/wiki/Johnson%27s_algorithm
