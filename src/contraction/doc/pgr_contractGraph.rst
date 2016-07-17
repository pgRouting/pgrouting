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

``pgr_contractGraph`` — Performs graph contraction and returns the contracted vertices and edges. 

.. warning::  This is a proposed function.

     - Is not officially in the current release

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------

Contraction reduces the size of the graph by removing some of the vertices and edges(according to a priority) and adds some psuedo edges,such that the number of edges are reduced on a whole.This reduces the time and space complexity of many algorithms that make various operations on the graph.

Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:
  - Process is done only on edges with positive costs.
  
  - There are two types of contraction methods used namely,
    - Dead End Contraction
    - Linear Contraction
  
  - The values returned include the added edges and contracted vertices.

  - The returned values are ordered:

    - `id` ascending



Signature Summary:
------------------

The pgr_contractGraph function has the following declaration:

.. code-block:: none

    pgr_contractGraph(edges_sql, contraction_order)
    pgr_contractGraph(edges_sql, contraction_order, forbidden_vertices)
    pgr_contractGraph(edges_sql, contraction_order, forbidden_vertices,
        max_cycles)
    pgr_contractGraph(edges_sql, contraction_order, forbidden_vertices,
        max_cycles, directed:=true)
        

    RETURNS SETOF ( seq, id, type, source, target, cost,
             contracted_vertices, contracted_vertices_size)


Signatures
-------------------------------------------------------------------------------


pgr_contractGraph Dead end Contraction
---------------------------------------

.. code-block:: none

    pgr_contractGraph(TEXT edges_sql, ARRAY[]::integer[], ARRAY[0]::bigint[], 1, directed:= true);
    RETURNS SETOF ( seq, id, type, source, target, cost,
             contracted_vertices, contracted_vertices_size)

This signature performs one cycle of dead end contraction on the graph:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

:Example:

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q1
   :end-before: -- q2


pgr_contractGraph Linear Contraction
-------------------------------------

.. code-block:: none

    pgr_contractGraph(TEXT edges_sql, ARRAY[]::integer[], ARRAY[1]::bigint[], 1, directed:= true);
    RETURNS SETOF ( seq, id, type, source, target, cost,
             contracted_vertices, contracted_vertices_size)

This signature performs one cycle of linear contraction on the graph:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

:Example:

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q2
   :end-before: -- q3



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

======================= ====================== =================================================
Column                  Type                   Description
======================= ====================== =================================================
**edges_sql**           ``TEXT``               SQL query as decribed above.
**contraction_order**   ``BIGINT[]``           Order of contraction operations.
**forbidden_vertices**  ``BIGINT[]``           (optional). Identifiers of vertices forbidden from contraction. Default is an empty array.
**max_cycles**          ``INTEGER``            (optional). Number of cycles of contraction. Default is **1**.
**directed**            ``BOOLEAN``            (optional). When false the graph is considered as Undirected. Default is true which considers the graph as Directed.
======================= ====================== =================================================


Description of the return values
-------------------------------------------------------------------------------

RETURNS SETOF  ( seq, id, type, source, target, cost,
             contracted_vertices, contracted_vertices_size)

The function returns a single row. The columns of the row are:

============================ =============   ==================================================
Column                       Type            Description
============================ =============   ==================================================
**seq**                      ``INTEGER``     Sequential value starting from **1**.
**id**                       ``BIGINT``      Identifier of the edge/vertex
**type**                     ``TEXT``        Type(edge/vertex). **v** for vertex. **e** for edge 
**source**                   ``BIGINT``      Identifier of the source vertex. Negative for **type** : **v** 
**target**                   ``BIGINT``      Identifier of the target vertex. Negative for **type** : **v**
**cost**                     ``FLOAT``       Weight of the shortcut (source, target). Negative for **type** : **v**
**contracted_vertices**      ``BIGINT[]``    Array of contracted vertex identifiers.
**contracted_vertices_size** ``INTEGER``     Size of **contracted_vertices** array.
============================ =============   ==================================================

Examples
========

Dead End Contraction
-------------------------------------

The examples of this section are based on the :ref:`sampledata` network.

:Example:

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q1
   :end-before: -- q2
 

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns for one cycle of dead end contraction
----------------------------------------------------------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q1
   :end-before: -- q2


Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns for one cycle of dead end contraction
----------------------------------------------------------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q3
   :end-before: -- q4




Examples for queries marked as ``directed`` with ``cost`` column for one cycle of dead end contraction
----------------------------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q5
   :end-before: -- q6


Examples for queries marked as ``undirected`` with ``cost`` column for one cycle of dead end contraction
---------------------------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q7
   :end-before: -- q8

Linear Contraction
-------------------------------------

The examples of this section are based on the :ref:`sampledata` network.

:Example:

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q2
   :end-before: -- q3
 

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns for one cycle of linear contraction
--------------------------------------------------------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q2
   :end-before: -- q3


Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns for one cycle of linear contraction
--------------------------------------------------------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q4
   :end-before: -- q5



Examples for queries marked as ``directed`` with ``cost`` column for one cycle of linear contraction
-------------------------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q6
   :end-before: -- q7


Examples for queries marked as ``undirected`` with ``cost`` column for one cycle of linear contraction
-------------------------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q8
   :end-before: -- q9

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

