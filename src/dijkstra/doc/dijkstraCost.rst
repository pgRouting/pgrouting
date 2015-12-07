..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstraCost:

pgr_dijkstraCost - Cost of Shortest Path Dijkstra
===============================================================================



Synopsis
-------------------------------------------------------------------------------

``pgr_dijkstraCost``

Using Dijkstra algorithm implemented by Boost.Graph, and extract only the
aggregate cost of the shortest path(s) found, for the combination of pairs of vertices given.

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside

The ``pgr_dijkstraCost`` algorithm, is a good choice to calculate the sum of the costs
of the shortest path for a subset of pairs of nodes of the graph.
We make use of the Boost's implementation of dijkstra which runs in
:math:`O(V \log V + E)` time.

Characteristics:
----------------

The main Characteristics are:
  - It does not return a path.
  - Returns the sum of the costs of the shortest path for pair combination of nodes in the graph.
  - Process is done only on edges with positive costs.
  - When the starting vertex and ending vertex are the same, there is no path, but the aggregate cost
    is `0`, that value is returned.

    - `(v, v, 0)`

  - When there is no path then the aggregate cost is infinity, and the combination is not returned.

    - We return only the non infinity values in form of a set of `(start_vid, end_vid, agg_cost)`.

  - Let be the case the values returned are stored in a table, so the unique index would be the pair:
    `(start_vid, end_vid)`.

  - For undirected graphs, the results are symetric.

    - The  `agg_cost` of `(u, v)` is the same as for `(v, u)`.

  - Runing time: :math:`O(| start\_vids | * (V \log V + E))`

Signature Summary
-----------------

.. code-block:: none

     pgr_dijkstraCost(edges_sql, start_vid, end_vid);
     pgr_dijkstraCost(edges_sql, start_vid, end_vid, directed);
     pgr_dijkstraCost(edges_sql, start_vids, end_vid, directed);
     pgr_dijkstraCost(edges_sql, start_vid, end_vids, directed);
     pgr_dijkstraCost(edges_sql, start_vids, end_vids, directed);

	 RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET



Signatures
===============================================================================

.. index::
	single: dijkstraCost(edges_sql, start_vid, end_vid)

Minimal signature
-----------------

The minimal signature is for a **directed** graph from one ``start_vid`` to one ``end_vid``:

.. code-block:: none

     pgr_dijkstraCost(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid)
	 RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET


.. rubric:: Example

.. literalinclude:: ../test/doc-dijkstraCost.doc.result
   :start-after: --q1
   :end-before: --q2



.. index::
	single: dijkstraCost(edges_sql, start_vid, end_vid, directed)

pgr_dijkstraCost One to One
--------------------------------


This signature performs a Dijkstra from one ``start_vid`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

.. code-block:: sql

      pgr_dijkstraCost(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid,
			 BOOLEAN directed:=true);
	 RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET

.. rubric:: Example

.. literalinclude:: ../test/doc-dijkstraCost.doc.result
   :start-after: --q2
   :end-before: --q3



.. index::
	single: dijkstraCost(edges_sql, start_vids, end_vid, directed)

pgr_dijkstraCost Many to One
--------------------------------

.. code-block:: none

      pgr_dijkstraCost(TEXT edges_sql, array[ANY_INTEGER] start_vids, BIGINT end_vid,
			 BOOLEAN directed:=true);
	 RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET

This signature performs a Dijkstra from each ``start_vid`` in  ``start_vids`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


.. rubric:: Example

.. literalinclude:: ../test/doc-dijkstraCost.doc.result
   :start-after: --q3
   :end-before: --q4




.. index::
	single: dijkstraCost(edges_sql, start_vid, end_vids, directed)

pgr_dijkstraCost One to Many
--------------------------------

.. code-block:: none

       pgr_dijkstraCost(TEXT edges_sql, BIGINT start_vid, array[ANY_INTEGER] end_vids,
			 BOOLEAN directed:=true);
	 RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET

This signature performs a Dijkstra from one ``start_vid`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


.. rubric:: Example

.. literalinclude:: ../test/doc-dijkstraCost.doc.result
   :start-after: --q4
   :end-before: --q5





.. index::
	single: dijkstraCost(edges_sql, start_vids, end_vids, directed)

.. rubric:: pgr_dijkstraCost Many to Many

.. code-block:: none

       pgr_dijkstraCost(TEXT edges_sql, array[ANY_INTEGER] start_vids, array[ANY_INTEGER] end_vids,
			 BOOLEAN directed:=true);
	 RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET

This signature performs a Dijkstra from each ``start_vid`` in  ``start_vids`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Example
-------

.. literalinclude:: ../test/doc-dijkstraCost.doc.result
   :start-after: --q5
   :end-before: --q6



Description of the Signatures
=============================

Description of the edge's SQL query
-----------------------------------

:edges_sql: is a ``TEXT`` that containes an SQL query, which should return a set of rows with the following columns:

================  ===================   =================================================
Column            Type                      Description
================  ===================   =================================================
**id**            ``ANY-INTEGER``       Identifier of the edge.
**source**        ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``     Weight of the edge `(source, target)`, if negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``     (optional) Weight of the edge `(target, source)`, if negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   =================================================


Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, real, float


Description of the parameters of the signatures
-------------------------------------------------------------------------------

================  ====================== =================================================
Column            Type                   Description
================  ====================== =================================================
**sql**           ``TEXT``               SQL query as decribed above.
**start_vid**     ``BIGINT``             Identifier of the starting vertex of the path.
**end_vid**       ``BIGINT``             Identifier of the ending vertex of the path.
**start_vids**    ``array[ANY-INTEGER]`` Array of identifiers of starting vertices.
**end_vids**      ``array[ANY-INTEGER]`` Array of identifiers of ending vertices.
**directed**      ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
================  ====================== =================================================


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(start_vid, end_vid, agg_cost)``

============= ============= =================================================
Column        Type          Description
============= ============= =================================================
**start_vid** ``BIGINT``    Identifier of the starting vertex.
**end_vid**   ``BIGINT``    Identifier of the ending vertex.
**agg_cost**  ``FLOAT``     Aggregate cost of the shortest path from ``start_vid`` to ``end_vid``.
============= ============= =================================================


Examples
========

The examples of this section are based on the :ref:`sampledata` network.

.. literalinclude:: ../test/doc-dijkstraCost.doc.result
   :start-after: --q6
   :end-before: --q7




The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0
* Added functionality in version 2.1.0


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

