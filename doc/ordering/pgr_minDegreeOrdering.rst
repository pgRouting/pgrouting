..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Ordering Family ; pgr_minDegreeOrdering
   single: minDegreeOrdering - Experimental on v4.0

|

``pgr_minDegreeOrdering`` - Experimental
===============================================================================

``pgr_minDegreeOrdering`` — Returns the minimum degree ordering of an undirected graphs

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

* Version 4.0.0

  * New experimental function.


Description
-------------------------------------------------------------------------------

In numerical linear algebra, the MinDegree ordering is a heuristic used to
reduce the amount of fill-in during sparse matrix factorization, such as
Cholesky decomposition for symmetric matrices.

When eliminating variables in a sparse system, new nonzero entries—called
fill-ins—can appear in positions that were originally zero, increasing storage
and computation costs. The minimum degree approach attempts to reorder the
vertices (or equivalently, the rows and columns of the matrix) so that,
at each elimination step, the vertex with the smallest degree is removed first,
thereby reducing potential fill-in.

The main characteristics are:

- The implementation works on undirected graphs.
- The algorithm is a heuristic; finding the true optimal ordering is NP-complete.
- The time complexity is: :math:`O(|V| \log |V| + |E|)`
   
   - where :math:`|V|` is the number of vertices,
   - :math:`|E|` is the number of edges.

|Boost| Boost Graph Inside

Signatures
------------------------------------------------------------------------------

.. index::
    single: minDegreeOrdering - Experimental on v4.0

.. admonition:: \ \
   :class: signatures

   | pgr_minDegreeOrdering(`Edges SQL`_)

   | Returns set of |result_node_order|
   | OR EMPTY SET

:Example: Graph ordering of pgRouting :doc:`sampledata`

.. literalinclude:: minDegreeOrdering.queries
   :start-after: -- q1
   :end-before: -- q2

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

.. include:: pgr_cuthillMckeeOrdering.rst
   :start-after: node_ordering_start
   :end-before: node_ordering_end

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: King Ordering
  <https://www.boost.org/libs/graph/doc/minimum_degree_ordering.html>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`


