..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bdDijkstra
===============================================================================

``pgr_bdDijkstra`` — Returns the shortest path(s) using Bidirectional Dijkstra algorithm.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc

   Boost Graph Inside

.. rubric:: Availability:

* Official on v3.0.0
* Proposed on v2.5.0:

  * pgr_bdDijkstra(One to Many)
  * pgr_bdDijkstra(Many to One)
  * pgr_bdDijkstra(Many to Many)

* Signature change on v2.4.0

  * pgr_bdDijkstra(One to One)

* Proposed on v2.5.0:

  * pgr_bdDijkstra(One to Many)
  * pgr_bdDijkstra(Many to One)
  * pgr_bdDijkstra(Many to Many)

* Signature change on v2.4.0:

  * pgr_bdDijkstra(One to One)

* New on v2.0.0:

  * pgr_bdDijkstra(One to One)

**Supported versions:**
current(`3.0 <http://docs.pgrouting.org/dev/en/pgr_bdDijkstra.html>`__)
`2.6 <http://docs.pgrouting.org/2.6/en/pgr_bdDijkstra.html>`__
`2.5 <http://docs.pgrouting.org/2.5/en/pgr_bdDijkstra.html>`__
`2.4 <http://docs.pgrouting.org/2.4/en/pgr_bdDijkstra.html>`__

**Unsupported versions:**
`2.3 <http://docs.pgrouting.org/2.3/en/src/bd_dijkstra/doc/pgr_bdDijkstra.html#bd-dijkstra>`__
`2.2 <http://docs.pgrouting.org/2.2/en/src/bd_dijkstra/doc/pgr_bdDijkstra.html#bd-dijkstra>`__
`2.1 <http://docs.pgrouting.org/2.1/en/src/bd_dijkstra/doc/index.html#bd-dijkstra>`__
`2.0 <http://docs.pgrouting.org/2.0/en/src/bd_dijkstra/doc/index.html#bd-dijkstra>`__

Description
-------------------------------------------------------------------------------
* **TBD**

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
    single: bdDijkstra(One to Many) - Proposed

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
    single: bdDijkstra(Many to One) - Proposed

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
    single: bdDijkstra(Many to Many) - Proposed

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

.. include:: pgr_dijkstra.rst
    :start-after: pgr_dijkstra_parameters_start
    :end-before: pgr_dijkstra_parameters_end

Inner query
-------------------------------------------------------------------------------

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
* http://www.cs.princeton.edu/courses/archive/spr06/cos423/Handouts/EPP%20shortest%20path%20algorithms.pdf
* https://en.wikipedia.org/wiki/Bidirectional_search

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

