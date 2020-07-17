..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_edgeDisjointPaths
==========================================

``pgr_edgeDisjointPaths`` — Calculates edge disjoint paths between two groups of vertices.


.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html

   Boost Graph Inside

.. Rubric:: Availability

* Version 3.0.0

  * **Official** function

* Version 2.5.0

  * **Proposed** function

* Version 2.3.0

  * New **Experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_edgeDisjointPaths.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_edgeDisjointPaths.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_edgeDisjointPaths.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_edgeDisjointPaths.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_edgeDisjointPaths.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/max_flow/doc/pgr_edgeDisjointPaths.html>`__

Description
-------------------------------------------------------------------------------

Calculates the edge disjoint paths between two groups of vertices.
Utilizes underlying maximum flow algorithms to calculate the paths.

The main characterics are:
  - Calculates the edge disjoint paths between any two groups of vertices.
  - Returns EMPTY SET when source and destination are the same, or cannot be reached.
  - The graph can be directed or undirected.
  - One to many, many to one, many to many versions are also supported.
  - Uses :doc:`pgr_boykovKolmogorov` to calculate the paths.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_edgeDisjointPaths(Edges SQL, start_vid, end_vid)
    pgr_edgeDisjointPaths(Edges SQL, start_vid, end_vid [, directed])
    pgr_edgeDisjointPaths(Edges SQL, start_vid, end_vids [, directed])
    pgr_edgeDisjointPaths(Edges SQL, start_vids, end_vid [, directed])
    pgr_edgeDisjointPaths(Edges SQL, start_vids, end_vids [, directed])

    RETURNS SET OF (seq, path_id, path_seq, [start_vid,] [end_vid,] node, edge, cost, agg_cost)
    OR EMPTY SET

.. rubric:: Using defaults

.. code-block:: none

    pgr_edgeDisjointPaths(Edges SQL, start_vid, end_vid)
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`3` to vertex :math:`5` on a **directed** graph

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: edgeDisjointPaths(One to One)

One to One
...............................................................................

.. code-block:: none

    pgr_edgeDisjointPaths(Edges SQL, start_vid, end_vid, directed)
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`3` to vertex :math:`5` on an **undirected** graph

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: edgeDisjointPaths(One to Many)

One to Many
...............................................................................

.. code-block:: none

    pgr_edgeDisjointPaths(Edges SQL, start_vid, end_vids, directed)
    RETURNS SET OF (seq, path_id, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`3` to vertices :math:`\{4, 5, 10\}` on a **directed** graph


.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: edgeDisjointPaths(Many to One)

Many to One
...............................................................................

.. code-block:: none

    pgr_edgeDisjointPaths(Edges SQL, start_vids, end_vid, directed)
    RETURNS SET OF (seq, path_id, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{3, 6\}` to vertex :math:`5` on a **directed** graph


.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: edgeDisjointPaths(Many to Many)

Many to Many
...............................................................................

.. code-block:: none

    pgr_edgeDisjointPaths(Edges SQL, start_vids, end_vids, directed)
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{3, 6\}` to vertices :math:`\{4, 5, 10\}` on a **directed** graph


.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
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

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end

See Also
-------------------------------------------------------------------------------

* :doc:`flow-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

