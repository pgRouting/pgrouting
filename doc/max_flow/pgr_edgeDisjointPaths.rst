..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


pgr_edgeDisjointPaths
==========================================

Synopsis
-------------------------------------------------------------------------------

``pgr_edgeDisjointPaths`` — Calculates edge disjoint paths between two groups of vertices.


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html

   Boost Graph Inside

.. Rubric:: Availability: 2.3.0


Calculates the edge disjoint paths between two groups of vertices.
Utilizes underlying maximum flow algorithms to calculate the paths.

Characteristics:
...............................................................................

The main characterics are:
  - Calculates the edge disjoint paths between any two groups of vertices.
  - Returns EMPTY SET when source and destination are the same, or cannot be reached.
  - The graph can be directed or undirected.
  - One to many, many to one, many to many versions are also supported.
  - Uses :ref:`pgr_boykovKolmogorov` to calculate the paths.

Signature Summary
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, start_vid, end_vid)
    pgr_edgeDisjointPaths(edges_sql, start_vid, end_vid [, directed])
    pgr_edgeDisjointPaths(edges_sql, start_vid, end_vids [, directed])
    pgr_edgeDisjointPaths(edges_sql, start_vids, end_vid [, directed])
    pgr_edgeDisjointPaths(edges_sql, start_vids, end_vids [, directed])

    RETURNS SET OF (seq, path_id, path_seq, [start_vid,] [end_vid,] node, edge, cost, agg_cost)
    OR EMPTY SET


.. index::
    single: edgeDisjointPaths(Minimal Use)

Minimal use
...............................................................................

.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, start_vid, end_vid)
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

The minimal use is for a **directed** graph from one ``start_vid`` to one ``end_vid``.

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: edgeDisjointPaths(One to One)

One to One
...............................................................................

This signature finds the set of dijoint paths from one ``start_vid`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, start_vid, end_vid, directed)
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: edgeDisjointPaths(One to Many)

One to Many
...............................................................................

This signature finds the sset of disjoint paths  from the ``start_vid`` to each one of the ``end_vid`` in ``end_vids``:
  - on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  - on an **undirected** graph when ``directed`` flag is set to ``false``.
  - The result is equivalent to the union of the results of the one to one `pgr_edgeDisjointPaths`.
  - The extra ``end_vid`` in the result is used to distinguish to which path it belongs.




.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, start_vid, end_vids, directed)
    RETURNS SET OF (seq, path_id, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q3
   :end-before: -- q4



.. index::
    single: edgeDisjointPaths(Many to One)

Many to One
...............................................................................

This signature finds the set of disjoint paths from each one of the ``start_vid`` in ``start_vids`` to the ``end_vid``:
  - on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  - on an **undirected** graph when ``directed`` flag is set to ``false``.
  - The result is equivalent to the union of the results of the one to one `pgr_edgeDisjointPaths`.
  - The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, start_vids, end_vid, directed)
    RETURNS SET OF (seq, path_id, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q4
   :end-before: -- q5



.. index::
    single: edgeDisjointPaths(Many to Many)

Many to Many
...............................................................................

This signature finds the set of disjoint paths from each one of the ``start_vid`` in ``start_vids`` to each one of the ``end_vid`` in ``end_vids``:
  - on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  - on an **undirected** graph when ``directed`` flag is set to ``false``.
  - The result is equivalent to the union of the results of the one to one `pgr_edgeDisjointPaths`.
  - The extra ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.

.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, start_vids, end_vids, directed)
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q5
   :end-before: -- q6



Parameters
-------------------------------------------------------------------------------

.. include:: pgr_dijkstra.rst
    :start-after: pgr_dijkstra_parameters_start
    :end-before: pgr_dijkstra_parameters_end

Inner query
-------------------------------------------------------------------------------

edges_sql
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end

See Also
--------

* :doc:`flow-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

