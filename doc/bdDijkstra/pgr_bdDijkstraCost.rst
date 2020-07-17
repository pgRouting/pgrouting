..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bdDijkstraCost
===============================================================================

``pgr_bdDijkstraCost`` — Returns the shortest path(s)'s cost using Bidirectional Dijkstra algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability:

* Version 3.0.0

  * **Official** function

* Version 2.5.0

  * New **proposed** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_bdDijkstraCost.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_bdDijkstraCost.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_bdDijkstra.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_bdDijkstra.html>`__


Description
-------------------------------------------------------------------------------

**The main characteristics are:**

.. include:: bdDijkstra-family.rst
   :start-after: description start
   :end-before: description end

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_bdDijkstraCost(edges_sql, from_vid,  to_vid  [, directed])
    pgr_bdDijkstraCost(edges_sql, from_vid,  to_vids [, directed])
    pgr_bdDijkstraCost(edges_sql, from_vids, to_vid  [, directed])
    pgr_bdDijkstraCost(edges_sql, from_vids, to_vids [, directed])

    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

.. rubric:: Using default

.. code-block:: none

    pgr_bdDijkstraCost(edges_sql, from_vid,  to_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on a **directed** graph

.. literalinclude:: doc-pgr_bdDijkstraCost.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: bdDijkstraCost(One to One)

One to One
...............................................................................

.. code-block:: none

    pgr_bdDijkstraCost(edges_sql, from_vid,  to_vid  [, directed])
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on an **undirected** graph

.. literalinclude:: doc-pgr_bdDijkstraCost.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: bdDijkstraCost(One to Many)

One to Many
...............................................................................

.. code-block:: none

    pgr_bdDijkstraCost(edges_sql, from_vid,  to_vids [, directed])
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 11\}` on a **directed** graph

.. literalinclude:: doc-pgr_bdDijkstraCost.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: bdDijkstraCost(Many to One)

Many to One
...............................................................................

.. code-block:: none

    pgr_bdDijkstraCost(edges_sql, from_vids, to_vids [, directed])
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 7\}` to vertex :math:`3` on a **directed** graph

.. literalinclude:: doc-pgr_bdDijkstraCost.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: bdDijkstraCost(Many to Many)

Many to Many
...............................................................................

.. code-block:: none

    pgr_bdDijkstraCost(edges_sql, start_vids, end_vids [, directed])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 7\}` to vertices :math:`\{3, 11\}` on a **directed** graph

.. literalinclude:: doc-pgr_bdDijkstraCost.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. include:: pgr_bdDijkstra.rst
    :start-after: bdDijkstra_parameters_start
    :end-before: bdDijkstra_parameters_end

Inner query
-------------------------------------------------------------------------------

Edges query
...............................................................................

.. include::  pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include::  pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.
* :doc:`pgr_bdDijkstra`
* https://www.cs.princeton.edu/courses/archive/spr06/cos423/Handouts/EPP%20shortest%20path%20algorithms.pdf
* https://en.wikipedia.org/wiki/Bidirectional_search

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

