..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Ordering Family ; pgr_topologicalSort - Experimental
   single: topologicalSort - Experimental on v3.0

|

``pgr_topologicalSort`` - Experimental
===============================================================================

``pgr_topologicalSort`` — Linear ordering of the vertices for directed acyclic
graphs (DAG).

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Standardize output to |result_node_order|

.. rubric:: Version 3.0.0

* New experimental function.

Description
-------------------------------------------------------------------------------

The topological sort algorithm creates a linear ordering of the vertices such
that if edge :math:`(u,v)` appears in the graph, then :math:`v` comes before
:math:`u` in the ordering.

The main characteristics are:

* Process is valid for directed acyclic graphs only. otherwise it will throw
  warnings.

* For optimization purposes, if there are more than one answer, the function
   will return one of them.

* The returned values are ordered in topological order:

* Running time: :math:`O(V + E)`

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_topologicalSort(`Edges SQL`_)

   | Returns set of |result_node_order|
   | OR EMPTY SET

:Example: Topologically sorting the graph

.. literalinclude:: topologicalSort.queries
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

Returns set of |result_node_order|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INTEGER``
     - Sequential value starting from :math:`1`
   * - ``node``
     - ``BIGINT``
     - Linear topological ordering of the vertices

Additional examples
-------------------------------------------------------------------------------

:Example: Topologically sorting the one way segments

.. literalinclude:: topologicalSort.queries
   :start-after: -- q2
   :end-before: -- q3

:Example: Graph is not a DAG

.. literalinclude:: topologicalSort.queries
   :start-after: -- q3
   :end-before: -- q4

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: topological sort
  <https://www.boost.org/libs/graph/doc/topological_sort.html>`__
* https://en.wikipedia.org/wiki/Topological_sorting

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
