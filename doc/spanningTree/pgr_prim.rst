..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_prim
===============================================================================

``pgr_prim`` — Minimum spanning forest of graph using Prim algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_prim.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_prim.html>`__

Description
-------------------------------------------------------------------------------

This algorithm finds the minimum spanning forest in a possibly disconnected
graph using Prim's algorithm.

**The main characteristics are:**

.. include:: prim-family.rst
   :start-after: prim-description-start
   :end-before: prim-description-end

- EMPTY SET is returned when there are no edges in the graph.


Signatures
-------------------------------------------------------------------------------

.. index::
    single: prim

.. rubric:: Summary

.. code-block:: none

    pgr_prim(edges_sql)

    RETURNS SET OF (edge, cost)
    OR EMPTY SET


:Example: Minimum Spanning Forest of a subgraph

.. literalinclude:: doc-pgr_prim.queries
   :start-after: -- q1
   :end-before: -- q2


.. parameters, inner query & result columns
   are the same as kruskal's

.. include:: pgr_kruskal.rst
   :start-after: mst_information_start
   :end-before: mst_information_end


See Also
-------------------------------------------------------------------------------

* :doc:`spanningTree-family`
* :doc:`prim-family`
* The queries use the :doc:`sampledata` network.
* `Boost: Prim's algorithm documentation <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Prim's algorithm <https://en.wikipedia.org/wiki/Prim%27s_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

