..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Spanning Tree Category ; Kruskal Family
   single: Kruskal Family

|

Kruskal - Family of functions
===============================================================================

.. official-start

* :doc:`pgr_kruskal`
* :doc:`pgr_kruskalBFS`
* :doc:`pgr_kruskalDD`
* :doc:`pgr_kruskalDFS`

.. official-end

.. toctree::
    :hidden:

    pgr_kruskal
    pgr_kruskalBFS
    pgr_kruskalDD
    pgr_kruskalDFS

Description
-------------------------------------------------------------------------------

Kruskal's algorithm is a greedy minimum spanning tree algorithm that in each
cycle finds and adds the edge of the least possible weight that connects any
two trees in the forest.

**The main Characteristics are:**

.. kruskal-description-start

.. include:: spanningTree-category.rst
   :start-after: spanntree_traits_start
   :end-before: spanntree_traits_end

- The total weight of all the edges in the tree or forest is minimized.
- Kruskal's running time: :math:`O(E * log E)`

.. kruskal-description-end

Inner Queries
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

See Also
-------------------------------------------------------------------------------

* :doc:`spanningTree-category`
* `Boost: Kruskal's algorithm <https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html>`__
* `Boost: Prim's algorithm <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Kruskal's algorithm <https://en.wikipedia.org/wiki/Kruskal's_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
