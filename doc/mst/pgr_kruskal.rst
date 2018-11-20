..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_kruskal - Experimental
===============================================================================

``pgr_kruskal`` — Returns the minimum spanning tree of graph using Kruskal algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Experimental

  * v3.0.0

Description
-------------------------------------------------------------------------------

Kruskal's algorithm is a greedy minimum spanning tree algorithm that in each
cycle finds and adds the edge
of the least possible weight that connects any two trees in the forest.
It is a greedy algorithm, as it finds a minimum spanning tree for a
connected weighted undirected graph by adding increasing cost arcs at each step.

**The main Characteristics are:**

.. include:: kruskal-family.rst
   :start-after: kruskal-description-start
   :end-before: kruskal-description-end

- EMPTY SET is returned when there are no edges in the graph.

Signatures
-------------------------------------------------------------------------------

.. index::
    single: kruskal - Experimental

.. code-block:: none

    pgr_kruskal(edges_sql)

    RETURNS SET OF (seq, edge, cost)
    OR EMPTY SET


:Example: Minimum Spanning Forest

.. literalinclude:: doc-pgr_kruskal.queries
   :start-after: -- q1
   :end-before: -- q2

.. mst_information_start

Parameters
-------------------------------------------------------------------------------

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**Edges SQL**       ``TEXT``               SQL query described in `Inner query`_.
=================== ====================== =================================================

Inner query
-------------------------------------------------------------------------------

.. rubric::edges_sql

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------


Returns set of ``(seq, edge, cost)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**seq**          ``BIGINT``  Sequential value starting from **1**.
**edge**         ``BIGINT``  Identifier of the edge.
**cost**         ``FLOAT``   Cost to traverse the edge.
===============  =========== ====================================================

.. mst_information_end


See Also
-------------------------------------------------------------------------------

* `Boost: Kruskal's Algorithm  <https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html>`__
* `Wikipedia: Kruskal's Algorithm <https://en.wikipedia.org/wiki/Kruskal's_algorithm>`__
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
