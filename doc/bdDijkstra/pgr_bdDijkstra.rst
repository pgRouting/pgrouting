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
* pgr_bdDijkstra(other signatures) 2.5.0

Signature Summary
-----------------

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vid,  end_vid)
    pgr_bdDijkstra(edges_sql, start_vid, end_vid, directed)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vid, end_vids, directed)
    pgr_bdDijkstra(edges_sql, start_vids, end_vid, directed)
    pgr_bdDijkstra(edges_sql, start_vids, end_vids, directed)

    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
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


.. index::
    single: bdDijkstra(One to Many) - Proposed

pgr_bdDijkstra One to many
.......................................

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vid, end_vids, directed)
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from one ``start_vid`` to each ``end_vid`` in ``end_vids``:

-  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
-  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform a one to one `pgr_dijkstra`
where the starting vertex is fixed, and stop when all ``end_vids`` are reached.

- The result is equivalent to the union of the results of the one to one `pgr_dijkstra`.
- The extra ``end_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_bdDijkstra.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: bdDijkstra(Many to One) - Proposed


pgr_bdDijkstra Many to One
.......................................

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vids, end_vid, directed)
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to one ``end_vid``:

-  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
-  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to one `pgr_dijkstra`
where the ending vertex is fixed.

- The result is the union of the results of the one to one `pgr_dijkstra`.
- The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_bdDijkstra.queries
   :start-after: -- q4
   :end-before: -- q5


.. index::
    single: bdDijkstra(Many to Many) - Proposed

pgr_bdDijkstra Many to Many
.......................................

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vids, end_vids, directed)
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to each ``end_vid`` in ``end_vids``:

-  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
-  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to Many `pgr_dijkstra`
for all ``start_vids``.

- The result is the union of the results of the one to one `pgr_dijkstra`.
- The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

The extra ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_bdDijkstra.queries
   :start-after: -- q5
   :end-before: -- q6



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

* The queries use the :doc:`sampledata` network.
* :ref:`bdDijkstra`
* http://www.cs.princeton.edu/courses/archive/spr06/cos423/Handouts/EPP%20shortest%20path%20algorithms.pdf
* https://en.wikipedia.org/wiki/Bidirectional_search

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

