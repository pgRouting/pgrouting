..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_dijkstraNear.html>`__
  (`3.2 <https://docs.pgrouting.org/3.2/en/pgr_dijkstraNear.html>`__)

pgr_dijkstraNear - Experimental
===============================================================================

``pgr_dijkstraNear`` — Using dijkstra algorithm, finds the route that leads to
the nearest vertex.

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.2.0

  * New **experimental** function


Description
-------------------------------------------------------------------------------

Given a graph, a starting vertex and a set of ending vertices,
this function finds the shortest path from the starting vertex to the nearest
ending vertex.

Characteristics
...............................................................................

.. dijkstraNear characteristics start

* Uses Dijkstra algorithm.
* Works for **directed** and **undirected** graphs.
* When there are more than one path to the same vertex with same cost:

  * The algorithm will return just one path

* Optionally allows to find more than one path.

  * When more than one path is to be returned:

    * Results are sorted in increasing order of:

      * aggregate cost
      * Within the same value of aggregate costs:

        * results are sorted by (source, target)

* Running time: Dijkstra running time: :math:`drt = O((|E| + |V|)log|V|)`

  * One to Many; :math:`drt`
  * Many to One: :math:`drt`
  * Many to Many: :math:`drt * |Starting vids|`
  * Combinations: :math:`drt * |Starting vids|`

.. dijkstraNear characteristics end

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_dijkstraNear(Edges SQL, Start vid,  End vids  [, directed] [, cap])
    pgr_dijkstraNear(Edges SQL, Start vids, End vid   [, directed] [, cap])
    pgr_dijkstraNear(Edges SQL, Start vids, End vids  [, directed] [, cap], [global])
    pgr_dijkstraNear(Edges SQL, Combinations SQL  [, directed] [, cap], [global])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

.. index::
    single: dijkstraNear(One to Many) - Experimental

One to Many
...............................................................................

.. code-block:: none

    pgr_dijkstraNear(Edges SQL, Start vid,  End vids [, directed] [, cap])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: Departing on car from vertex :math:`2` find the nearest subway station.

* Using a **directed** graph for car routing.
* The subway stations are on the following vertices :math:`\{ 3, 6, 7\}`
* The defaults used:

  * `directed => true`
  * `cap => 1`


.. literalinclude:: doc-pgr_dijkstraNear.queries
    :start-after: -- q1
    :end-before: -- q2
    :linenos:

The result shows that station at vertex :math:`6` is the nearest.

.. index::
    single: dijkstraNear(Many to One)  - Experimental

Many to One
...............................................................................

.. code-block:: none

    pgr_dijkstraNear(Edges SQL, Start vids, End vid  [, directed] [, cap])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: Departing on a car from a subway station find the nearest **two**
          stations to vertex :math:`2`

* Using a **directed** graph for car routing.
* The subway stations are on the following vertices :math:`\{ 3, 6, 7\}`
* On line `4`: using the positional parameter: `directed` set to ``true``
* In line `5`: using named parameter `cap => 2`

.. literalinclude:: doc-pgr_dijkstraNear.queries
    :start-after: -- q2
    :end-before: -- q3
    :linenos:

The result shows that station at vertex :math:`3` is the nearest and the next best
is :math:`6`.

.. index::
    single: dijkstraNear(Many to Many)  - Experimental

Many to Many
...............................................................................

.. code-block:: none

    pgr_dijkstraNear(Edges SQL, Start vids, End vids [, directed] [, cap], [global])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: Find the best pedestrian connection between two lines of buses

* Unsing an **undirected** graph for pedestrian routing
* The first subway line stations stops are at :math:`\{3, 6, 7\}`
* The second subway line stations are at :math:`\{4, 9\}`
* On line `4`: using the named parameter: `directed => false`
* The defaults used:

  * `cap => 1`
  * `global => true`

.. literalinclude:: doc-pgr_dijkstraNear.queries
    :start-after: -- q3
    :end-before: -- q4
    :linenos:

For a pedestrian the best connection is to get on/off is at vertex :math:`3` of the
first subway line and at vertex :math:`4` of the second subway line.

Only `one` route is returned because `global` is ``true`` and `cap` is ``1``

.. index::
    single: dijkstraNear(Combinations)  - Experimental

Combinations
...............................................................................

.. code-block:: none

    pgr_dijkstraNear(Edges SQL, Combinations SQL  [, directed] [, cap], [global])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: Find the best car connection between all the stations of two subway lines

* Using a **directed** graph for car routing.
* The first subway line stations stops are at :math:`\{3, 6, 7\}`
* The second subway line stations are at :math:`\{4, 9\}`
* line `3` sets the start vertices to be from the fisrt subway line and the ending
  vertices to be from the second subway line
* line `5` sets the start vertices to be from the first subway line and the ending
  vertices to be from the first subway line
* On line `6`: using the named parameter is `global => false`
* The defaults used:

  * `directed => true`
  * `cap => 1`

.. literalinclude:: doc-pgr_dijkstraNear.queries
    :start-after: -- q4
    :end-before: -- q5
    :linenos:

From the results:

* making a connection from the first subway line to the second:

  * :math:`{(3 -> 9) (6 -> 9) (7 -> 9)}` and the best one is :math:`(6 -> 9)` with a
    cost of :math:`1` (lines: `12` and `13`)

* making a connection from the second subway line to the first:

  * :math:`{(4 -> 3) (9 -> 6)}` and both are equaly good as they have the same
    cost. (lines: `10` and `11` and lines: `14` and `15`)


Parameters
-------------------------------------------------------------------------------

====================== ================== ======== =================================================
Parameter              Type               Default     Description
====================== ================== ======== =================================================
**Edges SQL**          ``TEXT``                    `Edges query`_ as described below
**Combinations SQL**   ``TEXT`` 	  	               `Combinations query` as described below
**Start vid**          ``BIGINT``                  Identifier of the starting vertex of the path.
**Start vids**         ``ARRAY[BIGINT]``           Array of identifiers of starting vertices.
**End vid**            ``BIGINT``                  Identifier of the ending vertex of the path.
**End vids**           ``ARRAY[BIGINT]``           Array of identifiers of ending vertices.
**directed**           ``BOOLEAN``        ``true`` - When ``true`` the graph is considered `Directed`
                                                   - When ``false`` the graph is considered as `Undirected`.
**cap**                ``BIGINT``           1      Find at most ``cap`` number of nearest shortest paths
**global**             ``BOOLEAN``        ``true`` - When ``true``: only ``cap`` limit results will be returned
                                                   - When ``false``: ``cap`` limit per ``Start vid`` will be returned
====================== ================== ======== =================================================


Inner query
-------------------------------------------------------------------------------

Edges query
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Combinations query
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Return Columns
-------------------------------------------------------------------------------

RETURNS SET OF ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``
OR EMPTY SET

================== ============= =================================================
Column             Type          Description
================== ============= =================================================
**seq**            ``BIGINT``    Sequential value starting from 1.
**path_seq**       ``BIGINT``    Sequential value starting from 1 for each :math:`(start\_vid \to end\_vid)` path.
**start_vid**      ``BIGINT``    Identifier of the starting vertex of the path.
**end_vid**        ``BIGINT``    Identifier of the ending vertex of the path.
**node**           ``BIGINT``    Identifier of the node at position ``path_seq`` in the :math:`(start\_vid \to end\_vid)` path.
**edge**           ``BIGINT``    Identifier of the edge used to go from node at ``path_seq`` to the node at ``path_seq + 1`` in the
                                 :math:`(start\_vid \to end\_vid)` path.

                                 * :math:`-1` for the last node of the path.

**cost**           ``FLOAT``     Cost to traverse from ``node`` using ``edge`` to the next node in the route sequence.

                                 * :math:`0` for the last row of the path.

**agg_cost**       ``FLOAT``     Total cost of traversing :math:`(start\_vid \to node)` section of the :math:`(start\_vid \to end\_vid)` path.
================== ============= =================================================


See Also
-------------------------------------------------------------------------------

* :doc:`dijkstra-family`
* :doc:`pgr_dijkstraNearCost`
* :doc:`sampledata` network.
* boost: https://www.boost.org/libs/graph/doc/table_of_contents.html
* Wikipedia: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

