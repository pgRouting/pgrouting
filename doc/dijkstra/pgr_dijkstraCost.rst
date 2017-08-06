..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstraCost:

pgr_dijkstraCost
===============================================================================


Synopsis
-------------------------------------------------------------------------------

``pgr_dijkstraCost``

Using Dijkstra algorithm implemented by Boost.Graph, and extract only the
aggregate cost of the shortest path(s) found, for the combination of vertices given.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside

.. rubric:: Availability

* pgr_dijkstraCost(all signatures) 2.2.0

The ``pgr_dijkstraCost`` algorithm, is a good choice to calculate the sum of the costs
of the shortest path for a subset of pairs of nodes of the graph.
We make use of the Boost's implementation of dijkstra which runs in
:math:`O(V \log V + E)` time.

Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:
  - It does not return a path.
  - Returns the sum of the costs of the shortest path for pair combination of nodes in the graph.
  - Process is done only on edges with positive costs.
  - Values are returned when there is a path.

    - The returned values are in the form of a set of `(start_vid, end_vid, agg_cost)`.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` int the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path.

      - The `agg_cost` in the non included values `(u, v)` is :math:`\infty`

  - Let be the case the values returned are stored in a table, so the unique index would be the pair:
    `(start_vid, end_vid)`.

  - For undirected graphs, the results are symmetric.

    - The  `agg_cost` of `(u, v)` is the same as for `(v, u)`.

  - Any duplicated value in the `start_vids` or `end_vids` is ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time: :math:`O(| start\_vids | * (V \log V + E))`

Signature Summary
-------------------------------------------------------------------------------

.. code-block:: none

     pgr_dijkstraCost(edges_sql, start_vid, end_vid);
     pgr_dijkstraCost(edges_sql, start_vid, end_vid, directed);
     pgr_dijkstraCost(edges_sql, start_vids, end_vid, directed);
     pgr_dijkstraCost(edges_sql, start_vid, end_vids, directed);
     pgr_dijkstraCost(edges_sql, start_vids, end_vids, directed);

	 RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET



Signatures
-------------------------------------------------------------------------------

.. index::
	single: dijkstraCost(Minimal Use)

Minimal signature
...............................................................................

The minimal signature is for a **directed** graph from one ``start_vid`` to one ``end_vid``:

.. code-block:: none

     pgr_dijkstraCost(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid)
	 RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET


.. rubric:: Example

.. literalinclude:: doc-pgr_dijkstraCost.queries
   :start-after: --q1
   :end-before: --q2



.. index::
	single: dijkstraCost(One to One)

pgr_dijkstraCost One to One
...............................................................................


This signature performs a Dijkstra from one ``start_vid`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

.. code-block:: none

    pgr_dijkstraCost(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid,
			 BOOLEAN directed:=true);
	RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET

:Example:

.. literalinclude:: doc-pgr_dijkstraCost.queries
    :start-after: --q2
    :end-before: --q3


.. index::
    single: dijkstraCost(One to Many)

pgr_dijkstraCost One to Many
...............................................................................

.. code-block:: none

    pgr_dijkstraCost(TEXT edges_sql, BIGINT start_vid, array[ANY_INTEGER] end_vids,
	    BOOLEAN directed:=true);
	RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET

This signature performs a Dijkstra from one ``start_vid`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


:Example:

.. literalinclude:: doc-pgr_dijkstraCost.queries
   :start-after: --q4
   :end-before: --q5




.. index::
	single: dijkstraCost(Many to One)

pgr_dijkstraCost Many to One
...............................................................................

.. code-block:: none

    pgr_dijkstraCost(TEXT edges_sql, array[ANY_INTEGER] start_vids, BIGINT end_vid,
			 BOOLEAN directed:=true);
	RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET

This signature performs a Dijkstra from each ``start_vid`` in  ``start_vids`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


:Example:

.. literalinclude:: doc-pgr_dijkstraCost.queries
    :start-after: --q3
    :end-before: --q4



.. index::
	single: dijkstraCost(Many to Many)

pgr_dijkstraCost Many to Many
...............................................................................

.. code-block:: none

    pgr_dijkstraCost(TEXT edges_sql, array[ANY_INTEGER] start_vids, array[ANY_INTEGER] end_vids,
	    BOOLEAN directed:=true);
	RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET

This signature performs a Dijkstra from each ``start_vid`` in  ``start_vids`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

:Example:

.. literalinclude:: doc-pgr_dijkstraCost.queries
   :start-after: --q5
   :end-before: --q6



Description of the Signatures
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. include:: pgr_dijkstra.rst
    :start-after: pgr_dijkstra_parameters_start
    :end-before: pgr_dijkstra_parameters_end

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end


Additional Examples
-------------------------------------------------------------------------------

:Example 1: Demonstration of repeated values are ignored, and result is sorted.

.. literalinclude:: doc-pgr_dijkstraCost.queries
    :start-after: --q6
    :end-before: --q7

:Example 2: Making `start_vids` the same as `end_vids`

.. literalinclude:: doc-pgr_dijkstraCost.queries
    :start-after: --q7
    :end-before: --q8



See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

