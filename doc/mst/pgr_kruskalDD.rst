..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_kruskalDD - Experimental
===============================================================================

``pgr_kruskalDD`` — Returns the catchament nodes using Kruskal algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Experimental on v3.0.0

Description
-------------------------------------------------------------------------------

Using Kruskal algorithm, extracts the nodes that have aggregate costs less than
or equal to the value distance.

**The main Characteristics are:**

- It's implementation is only on **undirected** graph.
- Process is done only on edges with positive costs.
- Returned tree nodes from a root vertex are on Depth First Search order.
- Running time: :math:`O(E * log E)`

  - Kruskal Running time: :math:`O(E * log E)`
  - Depth First Search Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_kruskal(edges_sql, root_vid, distance)
    pgr_kruskal(edges_sql, root_vids, distance)

    RETURNS SET OF (seq, from_v, depth, node, edge, cost, agg_cost)
    OR EMPTY SET

.. index::
    single: kruskal(Single vertex) - Experimental

Single vertex
...............................................................................

.. code-block:: none

    pgr_kruskal(edges_sql, root_vid, distance)

    RETURNS SET OF (seq, from_v, depth, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: The Minimum Spanning Tree starting on vertex :math:`2` with aggregate cost < math:`3.5`

.. literalinclude:: doc-pgr_kruskalDD.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: kruskal(Multiple vertices) - Experimental

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_kruskal(edges_sql, root_vids, distance)

    RETURNS SET OF (seq, from_v, depth, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: The Minimum Spanning Tree starting on vertices :math:`\{13, 2\}` with aggregate cost < :math:`3.5`

.. literalinclude:: doc-pgr_kruskalDD.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**edges_sql**       ``TEXT``               SQL query described in `Inner query`_.
**root_vid**        ``BIGINT``             Identifier of the root vertex of the tree.

                                           - When :math:`0` gets the spanning forest
                                             starting in aleatory nodes for each tree.

**root_vids**       ``ARRAY[ANY-INTEGER]`` Array of identifiers of the root vertices.

                                           - :math:`0` values are ignored
                                           - For optimization purposes, any duplicated value is ignored.

**distance**        ``FLOAT``              Upper limit for the inclusion of the node in the result.
=================== ====================== =================================================

Inner query
-------------------------------------------------------------------------------

.. rubric::edges_sql

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq, component, edge, cost, tree_cost)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**seq**          ``BIGINT``  Sequential value starting from :math:`1`.
**start_vid**    ``BIGINT``  Identifier of the starting vertex.

                             - In `Multiple Vertices`_ results are in ascending order.

**node**         ``BIGINT``  Identifier of ``node`` reached using ``edge``
**edge**         ``BIGINT``  Identifier of the ``edge`` used to arrive to node.

                             - :math:`-1`  when ``node`` = ``from_v``.

**cost**         ``FLOAT``   Cost to traverse ``edge``.
**agg_cost**     ``FLOAT``   Aggregate cost from ``from_v`` to ``node``.
===============  =========== ====================================================


See Also
-------------------------------------------------------------------------------

- `Boost Kruskal documentation <https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/kruskal_min_spanning_tree.html>`__
- Kruskal on `wikipedia <https://en.wikipedia.org/wiki/Kruskal%27s_algorithm>`__
- The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

