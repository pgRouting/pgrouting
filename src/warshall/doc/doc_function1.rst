.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************
   INSTRUCTIONS
   - if section consists of only one value then use this file as index.rst
   - change [...] (including the square braquets) to appropiate values
   - one file / function,  may signatures of the same function go in the same file

.. _pgr_[function1]:

pgr_[function1] - [very tiny description of function1]
===============================================================================

.. index:: 
	single: pgr_[function1](text,integer,integer,boolean,boolean)
	module: [section]

Name
-------------------------------------------------------------------------------

``pgr_[function1]`` — [medium description of
function1 it can span many
lines]

..
   keep if uses boost

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

[very long description of function
some hints
``inline code`` **bold**.
rubric is a kind of sub title
====  main heading
----  sub heading

For the signature sections choose from the examples
many lines]


Signatures
===============================================================================

.. rubric:: pgr_functon1 Minimal signature

[
 The minimal signature is for a **directed** graph from one ``start_v`` to one ``end_v``:
]

.. code-block:: sql

      pgr_[function1] ( [parameters] )
       	 RETURNS SET OF ( [output] ) or EMPTY SET


.. rubric:: pgr_[function1] complete signature

[
This signature performs a .....
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.
]

.. code-block:: sql

      pgr_dijkstra(  [parameters],
	                 boolean directed:=true);
       	 RETURNS SET OF ( [output] ) or EMPTY SET


The problem definition
======================

The graphs are defined as follows:

.. rubric:: Directed graph

The weighted directed graph, ``G_d(V,E)``, is definied by:

* the set of vertices 

  - ``V`` = ``source`` Union ``target`` Union ``{start_v}`` Union ``{end_v}``

* the set of edges

  - when ``reverse_cost`` column is *not* used: 

    - ``E`` = ``{ (source, target, cost) where cost >=0 }``

  - when ``reverse_cost`` column is used: 

    - ``E`` = ``{ (source, target, cost) where cost >=0 }``  union ``{ (target, source, reverse_cost) where reverse_cost >=0)}``

**This is done transparently using directed Boost.Graph**

.. rubric:: Undirected graph

The weighted undirected graph, ``G_u(V,E)``, is definied by:

* the set of vertices

  -  ``V`` = ``source`` Union ``target`` Union ``{start_v}`` Union ``{end_v}``

* the set of edges

  - when ``reverse_cost`` column is *not* used:

    - ``E`` = ``{ (source, target, cost) where cost >=0 }``  union ``{ (target, source, cost) where cost >=0)}``


  - when ``reverse_cost`` column is used:

    - ``E`` = ``{ (source, target, cost) where cost >=0 }``  union ``{ (target, source, cost) where cost >=0)}``  \
      union ``{ (target, source, reverse_cost) where cost >=0 }``  union ``{ (source, target,  reverse_cost) where reverse_cost >=0)}``

**This is done transparently using directed Boost.Graph**

.. rubric:: The problem

Given a graph:

  - ``G(V,E)``  where ``G(V,E) = G_d(V,E)`` or ``G(V,E) = G_u(V,E)``

and the .....

The algorithm returns a ......
set of ``(seq, node, edge, cost, agg_cost)``
which is the ....
where ``seq`` indicates the relative position in the path of the ``node`` or ``edge``.

  - When ``seq = 1`` then the row represents the begining of the path.
  - When ``edge = -1`` it represents the end of the path.
  - When ``node = end_v`` it represents the end of the path.


If there is no path, the resulting set is empty.

Aditional information like the cost (``cost``) of the edge to be used to go to the next node
and the aggregate cost (``agg_cost``) from the ``start_v`` up to the ``node`` is included.



Description of the Signatures
=============================

Description of the SQL query
-------------------------------------------------------------------------------

:sql: an SQL query, which should return a set of rows with the following columns:

	:id: ``ANY-INTEGER`` identifier of the edge.
	:source: ``ANY-INTEGER`` identifier of the source vertex of the edge.
	:target: ``ANY-INTEGER`` identifier of the target vertex of the edge.
	:cost: ``ANY-NUMERICAL`` value of the edge traversal cost. A negative cost will prevent the edge (``source``, ``target``) from being inserted in the graph.
	:reverse_cost: ``ANY-NUMERICAL`` (optional) the value for the reverse traversal of the edge. A negative cost will prevent the edge (``target``, ``source``) from being inserted in the graph.

Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float

For example:

.. code-block:: sql

    SELECT id, source, target, cost, reverse_cost from edge_table where geom && ST_Expand(ST_SetSRID(ST_Point(45, 34), 4326), 0.1)


Description of the parameters of the signatures
-------------------------------------------------------------------------------

:sql: SQL query as decribed above.
:start_v: ``BIGINT`` id of the starting vertex.
:start_v: ``array[ANY-INTEGER]`` array of id of starting vertices.
:end_v: ``BIGINT`` id of the ending vertex.
:end_v: ``array[ANY-INTEGER]`` array of id of ending vertices.
:directed: ``boolean`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq [, start_v] [, end_v] , node, edge, cost, agg_cost)``

:seq: ``INT``  relative position in the path. Has value **1** for the begining of the path.
:start_v: ``BIGINT`` id of the starting vertex. Used when multiple starting vetrices are in the query.
:end_v: ``BIGINT`` id of the ending vertex. Used when multiple ending vertices are in the query.
:node: ``BIGINT`` id of the node in the path from start_v to end_v.
:edge: ``BIGINT`` id of the edge used to go from ``node`` to the next node in the path sequence. ``-1`` for the last node of the path. 
:cost: ``FLOAT`` cost to traverse from ``node`` using ``edge`` to the next node in the path sequence.
:agg_cost:  ``FLOAT`` total cost from ``start_v`` to ``node``.


Examples
========

The examples of this section are based on the :ref:`sampledata` network.

The examples include combinations from starting vertices 2 and 11 to ending vertices 3 and 5 in a directed and
undirected graph with and with out reverse_cost.

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
--------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

[put as many examples as needed and use the documentation data for the examples]

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 3
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |    2 |    4 |    1 |        0
           2 |    5 |    8 |    1 |        1
           3 |    6 |    9 |    1 |        2
           4 |    9 |   16 |    1 |        3
           5 |    4 |    3 |    1 |        4
           6 |    3 |   -1 |    0 |        5
        (6 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 5
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |    2 |    4 |    1 |        0
           2 |    5 |   -1 |    0 |        1
        (2 rows)



Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 3,
                        false
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |    2 |    2 |    1 |        0
           2 |    3 |   -1 |    0 |        1
        (2 rows)



Examples for queries marked as ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, 3
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
        (0 rows)


Examples for queries marked as ``undirected`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`

.. code-block:: sql

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 3,
                        false
		);
        seq | node | edge | cost | agg_cost 
       -----+------+------+------+----------
          1 |    2 |    4 |    1 |        0
          2 |    5 |    8 |    1 |        1
          3 |    6 |    5 |    1 |        2
          4 |    3 |   -1 |    0 |        3
       (4 rows)


Equvalences between signatures
------------------------------

.. code-block:: sql


	SELECT * FROM pgr_dijkstra(
               'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2, 3,
                true     -- directed flag
	);


	SELECT * FROM pgr_dijkstra(
		'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2,3 
	);

       seq | node | edge | cost | agg_cost 
       -----+------+------+------+----------
          1 |    2 |    4 |    1 |        0
          2 |    5 |    8 |    1 |        1
          3 |    6 |    9 |    1 |        2
          4 |    9 |   16 |    1 |        3
          5 |    4 |    3 |    1 |        4
          6 |    3 |   -1 |    0 |        5
       (6 rows)


The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0 
* Added functionality for version 3.0.0 in version 2.1


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
