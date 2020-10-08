..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_binaryBreadthFirstSearch - Experimental
===============================================================================

``pgr_binaryBreadthFirstSearch`` — Returns the shortest path(s) in a binary graph.
Any graph whose edge-weights belongs to the set {0,X}, where 'X' is any non-negative real integer, is termed as a 'binary graph'.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/breadth_first_search.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* To-be experimental on v3.0.0

Description
-------------------------------------------------------------------------------

It is well-known that the shortest paths between a single source and all other vertices can be found using Breadth First Search in :math:`O(|E|)` in an unweighted graph, i.e. the distance is the minimal number of edges that you need to traverse from the source to another vertex. We can interpret such a graph also as a weighted graph, where every edge has the weight 1. If not all edges in graph have the same weight, that we need a more general algorithm, like Dijkstra's Algorithm  which runs in :math:`O(|E|log|V|)` time.

However if the weights are more constrained, we can use a faster algorithm. This algorithm, termed as 'Binary Breadth First Search' as well as '0-1 BFS', is a variation of the standard Breadth First Search problem to solve the SSSP (single-source shortest path) problem in :math:`O(|E|)`, if the weights of each edge belongs to the set {0,X}, where 'X' is any non-negative real integer.


**The main Characteristics are:**

  - Process is done only on 'binary graphs'. ('Binary Graph': Any graph whose edge-weights belongs to the set {0,X}, where 'X' is any non-negative real integer.)

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` are ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time: :math:`O(| start\_vids | * |E|)`

Signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_binaryBreadthFirstSearch(edges_sql, start_vid,  end_vid  [, directed])
    pgr_binaryBreadthFirstSearch(edges_sql, start_vid,  end_vids [, directed])
    pgr_binaryBreadthFirstSearch(edges_sql, start_vids, end_vid  [, directed])
    pgr_binaryBreadthFirstSearch(edges_sql, start_vids, end_vids [, directed])
    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
    OR EMPTY SET

.. code-block:: none

    pgr_binaryBreadthFirstSearch(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on a **directed** binary graph

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: binaryBreadthFirstSearch(One to One) - Experimental

One to One
...............................................................................

.. code-block:: none

    pgr_binaryBreadthFirstSearch(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid,
    BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on an **undirected** binary graph

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: Binary Breadth First Search(One to Many) - Experimental

One to many
...............................................................................

.. code-block:: none

    pgr_binaryBreadthFirstSearch(TEXT edges_sql, BIGINT start_vid, ARRAY[ANY_INTEGER] end_vids,
    BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 5\}` on an **undirected** binary graph

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: Binary Breadth First Search(Many to One) - Experimental

Many to One
...............................................................................

.. code-block:: none

    pgr_binaryBreadthFirstSearch(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, BIGINT end_vid,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertex :math:`5` on a **directed** binary graph

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: Binary Breadth First Search(Many to Many) - Experimental

Many to Many
...............................................................................

.. code-block:: none

    pgr_binaryBreadthFirstSearch(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, ARRAY[ANY_INTEGER] end_vids,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertices :math:`\{3, 5\}` on an **undirected** binary graph

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. pgr_binaryBreadthFirstSearch_parameters_start

============== ================== ======== =================================================
Parameter      Type               Default     Description
============== ================== ======== =================================================
**edges_sql**  ``TEXT``                    Inner SQL query as described below.
**start_vid**  ``BIGINT``                  Identifier of the starting vertex of the path.
**start_vids** ``ARRAY[BIGINT]``           Array of identifiers of starting vertices.
**end_vid**    ``BIGINT``                  Identifier of the ending vertex of the path.
**end_vids**   ``ARRAY[BIGINT]``           Array of identifiers of ending vertices.
**directed**   ``BOOLEAN``        ``true`` - When ``true`` Graph is considered `Directed`
                                           - When ``false`` the graph is considered as `Undirected`.
============== ================== ======== =================================================

.. pgr_binaryBreadthFirstSearch_parameters_end

Inner query
-------------------------------------------------------------------------------

.. rubric::edges_sql

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end


Example Data
-------------------------------------------------------------------------------

This type of data is used on the examples of this page.

Edwards-Moore Algorithm is best applied when trying to answer queries such as the following:
**“Find the path with the minimum number from Source to Destination”**
Here:
* *Source* = Source Vertex, *Destination* =  Any arbitrary destination vertex
* *X* is an event/property
* Each edge  in the graph is either “*X*” or “*Not X*” .

Example:
“Find the path with the minimum number of road works from Source to Destination”

Here, a road under work(aka **road works**)  means that part of the road is occupied for
construction work/maintenance.

Here:
* Edge ( *u* , *v* ) has weight = 0 if no road work is ongoing on the road from *u* to *v*.
* Edge ( *u*, *v*) has weight = 1 if road work is ongoing on the road from *u* to *v*.

Then, upon running the algorithm, we obtain the path with the minimum number of road works
from the given source and destination.

Thus, the queries used in the previous section can be interpreted in this manner.

Table Data
...............................................................................


The queries in the previous sections use the table 'roadworks'.
The data of the table:

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: --data-start
   :end-before: --data-end

See Also
-------------------------------------------------------------------------------

* https://cp-algorithms.com/graph/01_bfs.html
* https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Specialized_variants

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
