..
   ****************************************************************************
   pgRouting Manual
   Copyright(c) pgRouting Contributors

   This documentation is licensed under a Creative Commons Attribution-Share

   Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Ordering Family ; pgr_sloanOrdering - Experimental
   single: pgr_sloanOrdering - Experimental on v4.0

|

``pgr_sloanOrdering`` - Experimental
===============================================================================

``pgr_sloanOrdering`` — Returns the Sloan ordering of an undirected graph

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.0.0

* New experimental function.


Description
-------------------------------------------------------------------------------

The Sloan ordering algorithm reorders the vertices of a graph to reduce
bandwidth, profile, and wavefront properties, which is particularly useful for
sparse matrix computations and finite element analysis.

* Finds a pseudoperipheral vertex pair to determine good starting points
* Uses a priority-based algorithm that balances vertex degree and distance from the start vertex.
* Aims to minimize bandwidth (maximum difference between connected vertex indices.
* The implementation is for undirected graphs
* Typically produces better orderings than simple breadth-first approaches.
* Time complexity: :math:`O(V² + E)` where V is the number of vertices and E is
  the number of edges.

|Boost| Boost Graph Inside

Signatures
------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_sloanOrdering(`Edges SQL`_)

   | Returns set of |result_node_order|
   | OR EMPTY SET

:Example : Sloan ordering without specifying start vertex

.. literalinclude:: sloanOrdering.queries
   :start-after: -- q1
   :end-before: -- q2

.. Parameters, Inner Queries & result columns

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: only_edge_param_start
   :end-before: only_edge_param_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result columns
-------------------------------------------------------------------------------

Returns set of ``(seq, node)``

===============  =========== ======================================
Column           Type        Description
===============  =========== ======================================
``seq``           ``BIGINT``  Sequence of the order starting from 1.
``node``          ``BIGINT``  New sloan ordering order.
===============  =========== ======================================

Additional Examples

:Example: Sloan ordering of Original graph from Boost example (vertices 0-9).

.. graphviz::

   graph G{
   node[shape=circle, style=filled, fillcolor=lightblue, color=black, fontcolor=black, fontsize=12];
   edge[color=black, penwidth=1.5];

   0 -- 3;
   0 -- 5;
   1 -- 2;
   1 -- 4;
   1 -- 6;
   1 -- 9;
   2 -- 3;
   2 -- 4;
   3 -- 5;
   3 -- 8;
   4 -- 6;
   5 -- 6;
   5 -- 7;
   6 -- 7;

   {rank=same; 0; 8;}
   {rank=same; 3; 5; 7;}
   {rank=same; 2; 4; 6;}
   {rank=same; 1; 9;}

   }

.. literalinclude:: sloanOrdering.queries
  :start-after: -- q2
  :end-before: --q3

.. literalinclude:: sloanOrdering.queries
  :start-after: --q3
  :end-before: --q4


See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: Sloan Ordering
  <https://www.boost.org/doc/libs/latest/libs/graph/doc/sloan_ordering.htm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
