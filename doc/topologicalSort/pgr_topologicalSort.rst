..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_topologicalSort`` - Experimental
===============================================================================

``pgr_topologicalSort`` — Linear ordering of the vertices for directed acyclic
graphs (DAG).

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/topological_sort.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function

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


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. index::
   single: topologicalSort - Experimental on v3.0

.. admonition:: \ \
   :class: signatures

   | pgr_topologicalSort(`Edges SQL`_)

   | RETURNS SET OF |result-toposort|
   | OR EMPTY SET

:Example: Topologically sorting the graph

.. literalinclude:: doc-topologicalSort.queries
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

Result Columns
-------------------------------------------------------------------------------

Returns set of |result-toposort|

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
   * - ``sorted_v``
     - ``BIGINT``
     - Linear topological ordering of the vertices

Additional examples
-------------------------------------------------------------------------------

:Example: Topologically sorting the one way segments

.. literalinclude:: doc-topologicalSort.queries
   :start-after: -- q2
   :end-before: -- q3

:Example: Graph is not a DAG

.. literalinclude:: doc-topologicalSort.queries
   :start-after: -- q3
   :end-before: -- q4

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* https://en.wikipedia.org/wiki/Topological_sorting

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
