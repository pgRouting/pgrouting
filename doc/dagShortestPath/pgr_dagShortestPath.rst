..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_dagShortestPath - Experimental
===============================================================================

``pgr_dagShortestPath`` — Returns the shortest path(s) for weighted directed acyclic graphs(DAG).
In particular, the DAG shortest paths algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/dag_shortest_paths.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* **TBD**

Description
-------------------------------------------------------------------------------

Shortest Path for Directed Acyclic Graph(DAG) is a graph search algorithm that solves the shortest path problem for
weighted directed acyclic graph, producing a shortest path from
a starting vertex (``start_vid``) to an ending vertex (``end_vid``).

This implementation can only be used with a directed graph with no cycles i.e. directed acyclic graph. 

The algorithm relies on topological sorting the dag to impose a linear ordering on the vertices, and thus is more efficient for DAG's than either the Dijkstra or Bellman-Ford algorithm. 

The main Characteristics are:
  - Process is valid for weighted directed acyclic graphs only. otherwise it will throw warnings.
  - Values are returned when there is a path.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path:

      - The `agg_cost` the non included values `(u, v)` is :math:`\infty`

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` are ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time: :math:`O(| start\_vids | * (V + E))`


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_dagShortestPath(edges_sql, start_vid,  end_vid)
    pgr_dagShortestPath(edges_sql, start_vids,  end_vid)
    pgr_dagShortestPath(edges_sql, start_vid,  end_vids)
    pgr_dagShortestPath(edges_sql, start_vids,  end_vids)

    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
        OR EMPTY SET


.. index::
    single: dagShortestPath(One to One) - Experimental

One to One
...............................................................................

.. code-block:: none

    pgr_dagShortestPath(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from one ``start_vid`` to one ``end_vid``.

:Example:

.. literalinclude:: doc-pgr_dagShortestPath.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: dagShortestPath(One to Many) - Experimental

One to Many
...............................................................................

.. code-block:: none

    pgr_dagShortestPath(TEXT edges_sql, BIGINT start_vid, ARRAY[ANY_INTEGER] end_vids);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from one ``start_vid`` to each ``end_vid`` in ``end_vids``.

Using this signature, will load once the graph and perform a one to one `pgr_bellmanFord`
where the starting vertex is fixed, and stop when all ``end_vids`` are reached.

:Example:

.. literalinclude:: doc-pgr_dagShortestPath.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: dagShortestPath(Many to One) - Experimental

Many to One
...............................................................................

.. code-block:: none

    pgr_dagShortestPath(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, BIGINT end_vid);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to one ``end_vid``.

Using this signature, will load once the graph and perform several one to one `pgr_bellmanFord`
where the ending vertex is fixed.

:Example:

.. literalinclude:: doc-pgr_dagShortestPath.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: dagShortestPath(Many to Many) - Experimental

Many to Many
...............................................................................

.. code-block:: none

    pgr_dagShortestPath((TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, ARRAY[ANY_INTEGER] end_vids);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to each ``end_vid`` in ``end_vids``.

Using this signature, will load once the graph and perform several one to Many `pgr_bellmanFord`
for all ``start_vids``.

:Example:

.. literalinclude:: doc-pgr_dagShortestPath.queries
   :start-after: -- q4
   :end-before: -- q5

Parameters
-------------------------------------------------------------------------------

.. pgr_dagShortestPath_parameters_start

Description of the parameters of the signatures
...............................................................................

============== ================== ======== =================================================
Parameter      Type               Default     Description
============== ================== ======== =================================================
**edges_sql**        ``TEXT``                    SQL query as described above.
**start_vid**  ``BIGINT``                  Identifier of the starting vertex of the path.
**start_vids** ``ARRAY[BIGINT]``           Array of identifiers of starting vertices.
**end_vid**    ``BIGINT``                  Identifier of the ending vertex of the path.
**end_vids**   ``ARRAY[BIGINT]``           Array of identifiers of ending vertices.
============== ================== ======== =================================================

.. pgr_dagShortestPath_parameters_end

Inner Query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Results Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_short_start
    :end-before: return_path_short_end

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Topological_sorting
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

