..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

   INSTRUCTIONS
   - if section consists of only one value then use this file as index.rst
   - change [...] (including the square braquets) to appropriate values
   - one file / function,  may signatures of the same function go in the same file

.. _pgr_contractGraph:

pgr_contractGraph - Proposed
===============================================================================

``pgr_contractGraph`` — Performs graph contraction and returns the contracted vertices and edges. 


.. include:: ../../proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------

Contraction reduces the size of the graph by removing some of the vertices and edges(according to a priority) and adds some pseudo edges,such that the number of edges are reduced on a whole.This reduces the time and space complexity of many algorithms that make various operations on the graph.

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

The pgr_contractGraph function has the following signatures:

.. code-block:: none
    
    pgr_contractGraph(edges_sql, contraction_order)
    pgr_contractGraph(edges_sql, contraction_order, max_cycles, forbidden_vertices, directed)

    RETURNS SETOF (seq, type, id, contracted_vertices, source, target, cost)


Signatures
-------------------------------------------------------------------------------

.. index::
    single: contractGraph(Minimal Use)

Minimal signature
.......................................

.. code-block:: none
       
    pgr_contractGraph(edges_sql, contraction_order)

:Example: Making a dead end contraction and a linear contraction.

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q1
   :end-before: -- q2

Complete signature
.......................................

.. code-block:: none
       
    pgr_contractGraph(edges_sql, contraction_order, max_cycles, forbidden_vertices, directed)

:Example: Making a dead end contraction and a linear contraction and vertex 2 is forbidden from contraction

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q2
   :end-before: -- q3


.. include:: ../../common/src/edges_input.h
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end


Description of the parameters of the signatures
-------------------------------------------------------------------------------

======================= ====================== =================================================
Column                  Type                   Description
======================= ====================== =================================================
**edges_sql**           ``TEXT``               SQL query as described above.
**contraction_order**   ``ARRAY[ANY-INTEGER]`` Ordered contraction operations.
                                                -  1 = Dead end contraction
                                                -  2 = Linear contraction
**forbidden_vertices**  ``ARRAY[ANY-INTEGER]`` (optional). Identifiers of vertices forbidden from contraction. Default is an empty array.
**max_cycles**          ``INTEGER``            (optional). Number of times the contraction operations on `contraction_order` will be performed. Default is 1.
**directed**            ``BOOLEAN``            * When ``true`` Graph is considered `Directed`.
                                               * When ``false`` the graph is considered as `Undirected`.
======================= ====================== =================================================


Description of the return values
-------------------------------------------------------------------------------

RETURNS SETOF  ( seq, type, id, contracted_vertices, source, target, cost)

The function returns a single row. The columns of the row are:

============================ =================   ===================================================================
Column                       Type                Description
============================ =================   ===================================================================
**seq**                      ``INTEGER``         Sequential value starting from **1**.
**type**                     ``TEXT``            Type of the `id`. 
                                                  - 'v' when `id` is an identifier of a vertex.
                                                  - 'e' when `id` is an identifier of an edge.
**id**                       ``BIGINT``          Identifier of:
                                                  * the  vertex when `type = 'v'`.

                                                    - The vertex belongs to the edge_table passed as a parameter.
                                                  * the edge when `type = 'e'`.
                                                  
                                                    - The `id` is a decreasing sequence starting from **-1**.

                                                    - Representing a pseudo `id` as is not incorporated into the edge_table.
**contracted_vertices**      ``ARRAY[BIGINT]``   Array of contracted vertex identifiers.
**source**                   ``BIGINT``          Identifier of the source vertex of the current edge `id`. Valid values when `type = 'e'`.
**target**                   ``BIGINT``          Identifier of the target vertex of the current edge `id`. Valid values when `type = 'e'`.
**cost**                     ``FLOAT``           Weight of the edge (`source`, `target`). Valid values when `type = 'e'`.
============================ =================   ===================================================================

Examples
========

:Example: Only dead end contraction

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q3
   :end-before: -- q4

:Example: Only linear contraction

.. literalinclude:: doc-contractGraph.queries
   :start-after: -- q4
   :end-before: -- q5


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

