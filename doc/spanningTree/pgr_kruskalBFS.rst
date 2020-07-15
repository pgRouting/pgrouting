..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_kruskalBFS
===============================================================================

``pgr_kruskalBFS`` — Prim algorithm for Minimum Spanning Tree with Depth First
Search ordering.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_kruskalBFS.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_kruskalBFS.html>`__

Description
-------------------------------------------------------------------------------

Visits and extracts the nodes information in Breath First Search ordering
of the Minimum Spanning Tree created using Prims's algorithm.

**The main Characteristics are:**

.. include:: kruskal-family.rst
   :start-after: kruskal-description-start
   :end-before: kruskal-description-end

- Returned tree nodes from a root vertex are on Breath First Search order
- Breath First Search Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_kruskalBFS(Edges SQL, Root vid [, max_depth])
    pgr_kruskalBFS(Edges SQL, Root vids [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: kruskalBFS(Single vertex)

Single vertex
...............................................................................

.. code-block:: none

    pgr_kruskalBFS(Edges SQL, Root vid [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree having as root vertex :math:`2`

.. literalinclude:: doc-pgr_kruskalBFS.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: kruskalBFS(Multiple vertices)

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_kruskalBFS(Edges SQL, Root vids [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertices :math:`\{13, 2\}` with :math:`depth <= 3`

.. literalinclude:: doc-pgr_kruskalBFS.queries
   :start-after: --q2
   :end-before: --q3

.. Parameters, Inner query & result columns

.. include:: pgr_kruskalDFS.rst
   :start-after: mstfs-information-start
   :end-before: mstfs-information-end


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
