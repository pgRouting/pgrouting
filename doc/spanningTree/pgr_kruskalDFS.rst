..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_kruskalDFS
===============================================================================

``pgr_kruskalDFS`` — Kruskal algorithm for Minimum Spanning Tree with Depth First
Search ordering.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_kruskalDFS.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_kruskalDFS.html>`__

Description
-------------------------------------------------------------------------------

Visits and extracts the nodes information in Depth First Search ordering
of the Minimum Spanning Tree created using Kruskal's algorithm.

**The main Characteristics are:**

.. include:: kruskal-family.rst
   :start-after: kruskal-description-start
   :end-before: kruskal-description-end

- Returned tree nodes from a root vertex are on Depth First Search order
- Depth First Search Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_kruskalDFS(Edges SQL, Root vid [, max_depth])
    pgr_kruskalDFS(Edges SQL, Root vids [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: kruskalDFS(Single vertex)

Single vertex
...............................................................................

.. code-block:: none

    pgr_kruskalDFS(Edges SQL, Root vid [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertex :math:`2`

.. literalinclude:: doc-pgr_kruskalDFS.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: kruskalDFS(Multiple vertices)

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_kruskalDFS(Edges SQL, Root vids [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertices :math:`\{13, 2\}` with :math:`depth <= 3`

.. literalinclude:: doc-pgr_kruskalDFS.queries
   :start-after: --q2
   :end-before: --q3


.. mstfs-information-start

Parameters
-------------------------------------------------------------------------------

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**Edges SQL**       ``TEXT``               SQL query described in `Inner query`_.
**Root vid**        ``BIGINT``             Identifier of the root vertex of the tree.

                                           - Used on `Single vertex`_
                                           - When value is :math:`0` then gets the spanning forest
                                             starting in aleatory nodes for each tree in the forest.

**Root vids**       ``ARRAY[ANY-INTEGER]`` Array of identifiers of the root vertices.

                                           - Used on `Multiple vertices`_
                                           - :math:`0` values are ignored
                                           - For optimization purposes, any duplicated value is ignored.
=================== ====================== =================================================

Optional Parameters
...............................................................................


=================== =========== =========================== =================================================
Parameter           Type        Default                     Description
=================== =========== =========================== =================================================
**max_depth**       ``BIGINT``  :math:`9223372036854775807` Upper limit for depth of node in the tree

                                                            - When value is ``Negative`` then **throws error**
=================== =========== =========================== =================================================

Inner query
-------------------------------------------------------------------------------

.. rubric::Edges SQL

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgr_kruskalDD.rst
   :start-after: result columns start
   :end-before: result columns end

.. mstfs-information-end

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
