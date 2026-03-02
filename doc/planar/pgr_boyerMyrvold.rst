:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2020-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0

.. index::
   single: Planar Family ; pgr_boyerMyrvold
   single: boyerMyrvold

|

``pgr_boyerMyrvold`` - Experimental
===============================================================================

``pgr_boyerMyrvold`` — Returns the planar embedding of a graph, if the graph is
planar.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

* Version 4.1.0

  * New experimental function.


Description
-------------------------------------------------------------------------------

A planar embedding of a graph is a mapping of the graph onto a plane such that
no two edges cross. Given a graph, the Boyer-Myrvold planarity testing algorithm
can produce a planar embedding if the graph is planar, or identify a Kuratowski
subgraph (a subdivision of :math:`K_5` or :math:`K_{3,3}`) if it is not.

The main characteristics are:

* This implementation uses the Boyer-Myrvold Planarity Testing.
* Returns the edges of the planar embedding when the graph is planar.
* If the graph is not planar, it returns an empty set.
* Applicable only for **undirected** graphs.
* The algorithm does not consider traversal costs in the calculations.
* Running time: :math:`O(|V|)`

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_boyerMyrvold(`Edges SQL`)

   | RETURNS SETOF |result-edge|

:Example: Get the planar embedding of the graph

.. literalinclude:: boyerMyrvold.queries
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

Returns set of ``(seq, source, target, cost)``

============== =========== ==========================================
Column         Type        Description
============== =========== ==========================================
``seq``        ``BIGINT``  Sequential value starting from :math:`1`.
``source``     ``BIGINT``  Identifier of the source vertex.
``target``     ``BIGINT``  Identifier of the target vertex.
``cost``       ``FLOAT``   Cost of the edge.
============== =========== ==========================================

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Making a non planar graph
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Adding extra edges to create a :math:`K_5` subgraph.

.. literalinclude:: boyerMyrvold.queries
   :start-after: -- q2
   :end-before: -- q3

The graph is no longer planar, so the result is empty:

.. literalinclude:: boyerMyrvold.queries
   :start-after: -- q3
   :end-before: -- q4

Using with pgr_connectedComponents
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: boyerMyrvold.queries
   :start-after: -- q4
   :end-before: -- q5

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_isPlanar`
* :doc:`sampledata`
* `Boost: Boyer Myrvold
  <https://www.boost.org/libs/graph/doc/boyer_myrvold.html>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
