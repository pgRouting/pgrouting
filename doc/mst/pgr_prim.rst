..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_prim - Experimental
===============================================================================

``pgr_prim`` — Returns the minimum spanning tree of graph using Prim algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Experimental:

  * v3.0.0

Description
-------------------------------------------------------------------------------

This algorithm find the minimum spanning forest in a possibly disconnected
graph.

**The main characteristics are:**


- It's implementation is only on **undirected graph**.
- Process is done only on edges with positive costs.
- Span start from chosen root_vertex resulting subgraph.

  - When root vertex is not given then result is minimun spanning tree of disconnected graph.

- Values are returned when there is a minimum spanning tree.

  - When there is no edge in graph then EMPTY SET is return.

- The minimum spanning tree is same for any root vertex.
- Running time: :math:`O(E*log V)`

.. Note:: From boost Graph:
   "The algorithm as implemented in Boost.Graph does not produce correct results on graphs with parallel edges."



Signatures
-------------------------------------------------------------------------------

.. index::
    single: prim - Experimental

.. code-block:: none

    pgr_prim(edges_sql)

    RETURNS SET OF (seq, edge, cost)
    OR EMPTY SET


:Example: Minimum Spanning Forest

.. literalinclude:: doc-pgr_prim.queries
   :start-after: -- q1
   :end-before: -- q2


.. parameters, inner query & result columns

.. include:: pgr_kruskal.rst
   :start-after: mst_information_start
   :end-before: mst_information_end


See Also
-------------------------------------------------------------------------------

* `Boost: Prim's Algorithm  <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Prim's Algorithm <https://en.wikipedia.org/wiki/Prim's_algorithm>`__
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

