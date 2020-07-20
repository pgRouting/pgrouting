..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_kruskal
===============================================================================

``pgr_kruskal`` — Returns the minimum spanning tree of graph using Kruskal algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_kruskal.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_kruskal.html>`__

Description
-------------------------------------------------------------------------------

This algorithm finds the minimum spanning forest in a possibly disconnected
graph using Kruskal's algorithm.

**The main Characteristics are:**

.. include:: kruskal-family.rst
   :start-after: kruskal-description-start
   :end-before: kruskal-description-end

- EMPTY SET is returned when there are no edges in the graph.

Signatures
-------------------------------------------------------------------------------

.. index::
    single: kruskal

.. rubric:: Summary

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

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------


Returns SET OF ``(edge, cost)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**edge**         ``BIGINT``  Identifier of the edge.
**cost**         ``FLOAT``   Cost to traverse the edge.
===============  =========== ====================================================

.. mst_information_end


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
