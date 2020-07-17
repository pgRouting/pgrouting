..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bdDijkstra
===============================================================================

``pgr_bdDijkstra`` — Returns the shortest path(s) using Bidirectional Dijkstra algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability:

* Version 3.0.0

  * **Official** function

* Version 2.5.0

  * New **Proposed** functions:

    * pgr_bdDijkstra(One to Many)
    * pgr_bdDijkstra(Many to One)
    * pgr_bdDijkstra(Many to Many)

* Version 2.4.0

  * Signature change on pgr_bdDijsktra(One to One)

    * Old signature no longer supported

* Version 2.0.0

  * **Official** pgr_bdDijkstra(One to One)

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_bdDijkstra.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_bdDijkstra.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_bdDijkstra.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_bdDijkstra.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_bdDijkstra.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/bd_dijkstra/doc/pgr_bdDijkstra.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/bd_dijkstra/doc/pgr_bdDijkstra.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/bd_dijkstra/doc/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/bd_dijkstra/doc/index.html>`__

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

    pgr_bdDijkstra(edges_sql, start_vid,  end_vid  [, directed])
    pgr_bdDijkstra(edges_sql, start_vid,  end_vids [, directed])
    pgr_bdDijkstra(edges_sql, start_vids, end_vid  [, directed])
    pgr_bdDijkstra(edges_sql, start_vids, end_vids [, directed])

    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
    OR EMPTY SET

.. rubric:: Using defaults

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vid, end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3`

.. literalinclude:: doc-pgr_bdDijkstra.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: bdDijkstra(One to One)

One to One
...............................................................................

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vid, end_vid [, directed])
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on an **undirected** graph

.. literalinclude:: doc-pgr_bdDijkstra.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: bdDijkstra(One to Many)

One to many
...............................................................................

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vid, end_vids [, directed])
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 11\}` on a **directed** graph

.. literalinclude:: doc-pgr_bdDijkstra.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: bdDijkstra(Many to One)

Many to One
...............................................................................

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vids, end_vid [, directed])
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 7\}` to vertex :math:`3` on a **directed** graph

.. literalinclude:: doc-pgr_bdDijkstra.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: bdDijkstra(Many to Many)

Many to Many
...............................................................................

.. code-block:: none

    pgr_bdDijkstra(edges_sql, start_vids, end_vids [, directed])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 7\}` to vertices :math:`\{3, 11\}` on a **directed** graph

.. literalinclude:: doc-pgr_bdDijkstra.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. bdDijkstra_parameters_start

====================== ================== ======== =================================================
Parameter              Type               Default     Description
====================== ================== ======== =================================================
**Edges SQL**          ``TEXT``                    `Edges query`_ as described below
**start_vid**          ``BIGINT``                  Identifier of the starting vertex of the path.
**start_vids**         ``ARRAY[BIGINT]``           Array of identifiers of starting vertices.
**end_vid**            ``BIGINT``                  Identifier of the ending vertex of the path.
**end_vids**           ``ARRAY[BIGINT]``           Array of identifiers of ending vertices.
**directed**           ``BOOLEAN``        ``true`` - When ``true`` Graph is considered `Directed`
                                                   - When ``false`` the graph is considered as `Undirected`.
====================== ================== ======== =================================================

.. bdDijkstra_parameters_end

Inner query
-------------------------------------------------------------------------------

Edges query
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end

See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.
* :doc:`bdDijkstra-family`
* https://www.cs.princeton.edu/courses/archive/spr06/cos423/Handouts/EPP%20shortest%20path%20algorithms.pdf
* https://en.wikipedia.org/wiki/Bidirectional_search

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

