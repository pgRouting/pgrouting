..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_bdDijkstra:

pgr_bdDijkstra
===============================================================================

``pgr_bdDijkstra`` — Returns the shortest path(s) using Bidirectional Dijkstra algorithm.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc

   Boost Graph Inside

.. rubric:: Availability:

* pgr_bdDijkstra(one to one) 2.0.0, Signature changed 2.4.0

Signature Summary
-----------------

.. code-block:: none

    pgr_dijkstra(edges_sql, start_vid,  end_vid)
    pgr_bdDijkstra(edges_sql, start_vid, end_vid, directed)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET



Signatures
-------------------------------------------------------------------------------

.. index::
    single: bdDijkstra(Minimal Use)

Minimal signature
.......................................

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vid, end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

The minimal signature is for a **directed** graph from one ``start_vid`` to one ``end_vid``:

:Example:

.. literalinclude:: doc-pgr_bdDijkstra.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: bdDijkstra(One to One)

pgr_bdDijkstra One to One
.......................................

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vid, end_vid, directed)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from one ``start_vid`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

:Example:

.. literalinclude:: doc-pgr_bdDijkstra.queries
   :start-after: -- q2
   :end-before: -- q3



Description of the Signatures
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. include:: pgr_dijkstra.rst
    :start-after: pgr_dijkstra_parameters_start
    :end-before: pgr_dijkstra_parameters_end

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end




See Also
-------------------------------------------------------------------------------

* The queries use the :ref:`sampledata` network.
* http://www.cs.princeton.edu/courses/archive/spr06/cos423/Handouts/EPP%20shortest%20path%20algorithms.pdf
* https://en.wikipedia.org/wiki/Bidirectional_search

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

