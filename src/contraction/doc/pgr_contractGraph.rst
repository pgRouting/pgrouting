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

.. _pgr_contractGraph:

pgr_contractGraph
===============================================================================

Name
-------------------------------------------------------------------------------

``pgr_contractGraph`` — [medium description of

.. warning::  This is a proposed function.

     - Is not officially in the current release

..
   keep if uses boost (this is a comment)

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Introduction
-------------

Contraction Hierarchies reduce the size of the graph by removing some of the vertices and edges(according to a priority)and adds some psuedo edges,such that the number of edges are reduced on a whole.This reduces the time and space complexity of many algorithms that make
 various operations on the graph.



Function:
---------

The pgr_contractgraph function has the following declaration:

.. code-block:: sql

        pgr_contractgraph(edges_sql, level, TBD)
        RETURNS SETOF  TBD


Description of the SQL query
-------------------------------------------------------------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   =================================================
Column            Type                  Description
================  ===================   =================================================
**id**            ``ANY-INTEGER``       Identifier of the edge.
**source**        ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``     Weight of the edge `(source, target)`, If negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``     (optional) Weight of the edge `(target, source)`, If negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


Description of the parameters of the signatures
-------------------------------------------------------------------------------

============== ====================== =================================================
Column         Type                   Description
============== ====================== =================================================
**edges_sql**  ``TEXT``               SQL query as decribed above.
**level**      ``INTEGER``            Level of contraction.
============== ====================== =================================================


Output:
------- 

The function returns a single row. The columns of the row are:

* contracted_graph_name: name of the contracted graph.
* contracted_graph_blob: edges of the contracted graph in text format.
* removedVertices      : vertices removed during contraction. 
* removedEdges             : edges removed during contraction.
* psuedoEdges              : shortcuts introduced during contraction.


Examples:
---------

Each row in the edge table should be of this format

.. code-block:: sql

         gid | source | target | cost | reverse_cost 
        -----+--------+--------+------+--------------
          12 |      3 |     10 |    2 |         -1 


means that the cost of going from edge 3 to edge 10 is 2.

Let us consider the edge table of a sample graph,and generate a contracted version of it.
 
.. code-block:: sql

         id  | source   | target | cost | reverse_cost 
    -----+----------+--------+------+-------------
       1 |        1 |    2       |    1 |       -1  
       2 |        2 |    3   |    2 |            3
       3 |        2 |    4   |    1 |            1   
       4 |        3 |    4   |    3 |           -1 
       5 |        4 |    5   |    1 |            2   
       6 |        5 |    6   |    4 |           -1   


To contract a graph:

Level 0:

        SELECT * FROM pgr_contractgraph('SELECT id, source, target, cost, 
                     reverse_cost as revcost FROM edges',0,false);

.. code-block:: sql

        contracted_graph_name | contracted_graph_blob           | removedvertices                      |           removededges             | psuedoedges 
        ----------------------+---------------------------------+--------------------------------------+------------------------------------+-------------
         contracted_graph_0   | 2,2,3,2,0$3,2,4,1,0$4,3,4,3,0$  | 1,1,1,2,1,0$5,5,4,5,1,0$6,6,5,6,4,0$ |  1,1,2,1,-1$5,5,4,1,-1$6,6,5,4,-1$ | 

        (1 row)


Level 1:

        SELECT * FROM pgr_contractgraph('SELECT id, source, target, cost, 
                     reverse_cost as revcost FROM edges',1,false);

.. code-block:: sql

        contracted_graph_name | contracted_graph_blob           | removedvertices                                              |         removededges                                     | psuedoedges 
        ----------------------+---------------------------------+--------------------------------------------------------------+----------------------------------------------------------+---------------------
         contracted_graph_1   |           3,2,4,1,0$            | 1,1,1,2,1,0$2,4,3,4,3,0$2,2,2,3,2,0$5,5,4,5,1,0$6,6,5,6,4,0$ |  1,1,2,1,-1$2,3,2,2,-1$4,3,4,3,-1$5,5,4,1,-1$6,6,5,4,-1$ | $3,2,4


        (1 row)




Description of the SQL query
-------------------------------------------------------------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   =================================================
Column            Type                  Description
================  ===================   =================================================
**id**            ``ANY-INTEGER``       Identifier of the edge.
**source**        ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``     Weight of the edge `(source, target)`, If negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``     (optional) Weight of the edge `(target, source)`, If negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   =================================================

Description of the Points SQL query
-------------------------------------------------------------------------------

:points_sql: an SQL query, which should return a set of rows with the following columns:

============ ================= =================================================
Column            Type              Description
============ ================= =================================================
**pid**      ``ANY-INTEGER``   (optional) Identifier of the point. Can not be NULL. If column not present, a sequential identifier will be given automatically.
**eid**      ``ANY-INTEGER``   Identifier of the "closest" edge to the point.
**fraction** ``ANY-NUMERICAL`` Value in [0,1] that indicates the relative postition from the first end point of the edge.
**side**     ``CHAR``          (optional) Value in ['b', 'r', 'l', NULL] indicating if the point is:
                                 - In the right, left of the edge or
                                 - If it doesn't matter with 'b' or NULL.
                                 - If column not present 'b' is considered.

                               Can be in upper or lower case.
============ ================= =================================================


Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


Description of the parameters of the signatures
-------------------------------------------------------------------------------

============== ====================== =================================================
Column         Type                   Description
============== ====================== =================================================
**edges_sql**  ``TEXT``               SQL query as decribed above.
**points_sql** ``TEXT``               Points SQL query as decribed above.
**start_vid**  ``BIGINT``             Identifier of the starting vertex of the path.
**start_vids** ``ARRAY[ANY-INTEGER]`` Array of identifiers of starting vertices.
**end_vid**    ``BIGINT``             Identifier of the ending vertex of the path.
**end_vids**   ``ARRAY[ANY-INTEGER]`` Array of identifiers of ending vertices.
**directed**   ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
============== ====================== =================================================


Examples
========

The examples of this section are based on the :ref:`sampledata` network.



[put as many examples as needed and use the documentation data for the examples]

:Example:

.. literalinclude:: doc-contractGraph.queries
   :start-after: --q2
   :end-before: --q3

..
   If needed here are some subtitles  

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
--------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

:Example: This example is in a subtitle

.. literalinclude:: doc-contractGraph.queries
   :start-after: --q2
   :end-before: --q3


Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`



Examples for queries marked as ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`


Examples for queries marked as ``undirected`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`



The queries use the :ref:`sampledata` network.

.. rubric:: History

* Official in version X.X
* Proposed in version Y.Y 


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

