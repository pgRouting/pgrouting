..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Ordering Family ; pgr_cuthillMckeeOrdering - Experimental
   single: pgr_cuthillMckeeOrdering  - Experimental on v3.4

|

``pgr_cuthillMckeeOrdering`` - Experimental
===============================================================================

``pgr_cuthillMckeeOrdering`` — Returns the reverse Cuthill-Mckee ordering of an undirected
graphs

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

* Version 3.4.0

  * New experimental function.


Description
-------------------------------------------------------------------------------

In numerical linear algebra, the Cuthill-McKee algorithm (CM), named after
Elizabeth Cuthill and James McKee, is an algorithm to permute a sparse
matrix that has a symmetric sparsity pattern into a band matrix form with a
small bandwidth.

The vertices are basically assigned a breadth-first search order, except that at
each step, the adjacent vertices are placed in the queue in order of increasing degree.

**The main Characteristics are:**

- The implementation is for **undirected** graphs.
- The bandwidth minimization problems are considered NP-complete problems.
- The running time complexity is: :math:`O(m log(m)|V|)`

  - where :math:`|V|` is the number of vertices,
  - :math:`m` is the maximum degree of the vertices in the graph.

|Boost| Boost Graph Inside

Signatures
------------------------------------------------------------------------------

.. index::
    single: cuthillMckeeOrdering - Experimental on v3.4

.. admonition:: \ \
   :class: signatures

   | pgr_cuthillMckeeOrdering(`Edges SQL`_)

   | Returns set of |result_node_order|
   | OR EMPTY SET

:Example: Graph ordering of pgRouting :doc:`sampledata`

.. literalinclude:: cuthillMckeeOrdering.queries
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

.. node_ordering_start

Returns set of ``(seq, node)``

===============  =========== ======================================
Column           Type        Description
===============  =========== ======================================
``seq``           ``BIGINT``  Sequence of the order starting from 1.
``node``          ``BIGINT``  New ordering in reverse order.
===============  =========== ======================================

.. node_ordering_end

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: Cuthill-McKee Ordering
  <https://www.boost.org/libs/graph/doc/cuthill_mckee_ordering.html>`__
* `Wikipedia: Cuthill-McKee Ordering
  <https://en.wikipedia.org/wiki/Cuthill%E2%80%93McKee_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`


