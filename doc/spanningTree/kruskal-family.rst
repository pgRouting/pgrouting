..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Kruskal - Family of functions
===============================================================================

.. index from here

* :doc:`pgr_kruskal`
* :doc:`pgr_kruskalBFS`
* :doc:`pgr_kruskalDD`
* :doc:`pgr_kruskalDFS`

.. index to here

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside


.. toctree::
    :hidden:

    pgr_kruskal
    pgr_kruskalBFS
    pgr_kruskalDD
    pgr_kruskalDFS

.. rubric:: Previous versions of this page

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/kruskal-family.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/kruskal-family.html>`__

Description
-------------------------------------------------------------------------------

Kruskal's algorithm is a greedy minimum spanning tree algorithm that in each
cycle finds and adds the edge of the least possible weight that connects any
two trees in the forest.

**The main Characteristics are:**

.. kruskal-description-start

- It's implementation is only on **undirected** graph.
- Process is done only on edges with positive costs.
- The total weight of all the edges in the tree or forest is minimized.
- When the graph is connected

  - The resulting edges make up a tree

- When the graph is not connected,

  - Finds a minimum spanning tree for each connected component.
  - The resulting edges make up a forest.

- Kruskal's running time: :math:`O(E * log E)`

.. kruskal-description-end

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end


See Also
-------------------------------------------------------------------------------

* :doc:`spanningTree-family`
* `Boost: Kruskal's algorithm documentation <https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html>`__
* `Wikipedia: Kruskal's algorithm <https://en.wikipedia.org/wiki/Kruskal's_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
