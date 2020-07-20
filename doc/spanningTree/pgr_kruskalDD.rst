..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_kruskalDD
===============================================================================

``pgr_kruskalDD`` — Catchament nodes using Kruskal's algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_kruskalDD.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_kruskalDD.html>`__

Description
-------------------------------------------------------------------------------

Using Kruskal's algorithm, extracts the nodes that have aggregate costs less than
or equal to the value ``Distance`` from a **root** vertex (or vertices) within
the calculated minimum spanning tree.


**The main Characteristics are:**

.. include:: kruskal-family.rst
   :start-after: kruskal-description-start
   :end-before: kruskal-description-end

- Returned tree nodes from a root vertex are on Depth First Search order.
- Depth First Search running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_kruskalDD(edges_sql, root_vid, distance)
    pgr_kruskalDD(edges_sql, root_vids, distance)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: kruskalDD(Single vertex)

Single vertex
...............................................................................

.. code-block:: none

    pgr_kruskalDD(edges_sql, root_vid, distance)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertex :math:`2` with :math:`agg\_cost <= 3.5`

.. literalinclude:: doc-pgr_kruskalDD.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: kruskalDD(Multiple vertices)

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_kruskalDD(edges_sql, root_vids, distance)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertices :math:`\{13, 2\}` with :math:`agg\_cost <= 3.5`;

.. literalinclude:: doc-pgr_kruskalDD.queries
   :start-after: -- q2
   :end-before: -- q3

.. mstDD-information-start

Parameters
-------------------------------------------------------------------------------

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**Edges SQL**       ``TEXT``               SQL query described in `Inner query`_.
**Root vid**        ``BIGINT``             Identifier of the root vertex of the tree.

                                           - Used on `Single vertex`_
                                           - When :math:`0` gets the spanning forest
                                             starting in aleatory nodes for each tree.

**Root vids**       ``ARRAY[ANY-INTEGER]`` Array of identifiers of the root vertices.

                                           - Used on `Multiple vertices`_
                                           - :math:`0` values are ignored
                                           - For optimization purposes, any duplicated value is ignored.

**Distance**        ``ANY-NUMERIC``        Upper limit for the inclusion of the node in the result.

                                           - When the value is Negative **throws error**
=================== ====================== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERIC: SMALLINT, INTEGER, BIGINT, REAL, FLOAT, NUMERIC

Inner query
-------------------------------------------------------------------------------

.. rubric::edges_sql

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. result columns start

Returns SET OF ``(seq, depth, start_vid, node, edge, cost, agg_cost)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**seq**          ``BIGINT``  Sequential value starting from :math:`1`.
**depth**        ``BIGINT``  Depth of the ``node``.

                             - :math:`0`  when ``node`` = ``start_vid``.

**start_vid**    ``BIGINT``  Identifier of the root vertex.

                             - In `Multiple Vertices`_ results are in ascending order.

**node**         ``BIGINT``  Identifier of ``node`` reached using ``edge``.
**edge**         ``BIGINT``  Identifier of the ``edge`` used to arrive to ``node``.

                             - :math:`-1`  when ``node`` = ``start_vid``.

**cost**         ``FLOAT``   Cost to traverse ``edge``.
**agg_cost**     ``FLOAT``   Aggregate cost from ``start_vid`` to ``node``.
===============  =========== ====================================================

.. result columns end

.. mstDD-information-end

See Also
-------------------------------------------------------------------------------

* :doc:`spanningTree-family`
* :doc:`kruskal-family`
* The queries use the :doc:`sampledata` network.
* `Boost: Kruskal's algorithm documentation <https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html>`__
* `Wikipedia: Kruskal's algorithm <https://en.wikipedia.org/wiki/Kruskal's_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
