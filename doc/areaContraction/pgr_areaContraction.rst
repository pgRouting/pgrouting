..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_areaContraction:

pgr_areaContraction
===============================================================================

``pgr_areaContraction`` — Returns the contracted graph of an area in a graph.
An area is defined by a set of vertices and edges connecting them. Some of these
vertices are defined as border vertices. Contracted graph is a set of shortest paths
between the border vertices. The set of shortest paths is calculated using Many-to-Many
Dijkstra.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

Synopsis
-------------------------------------------------------------------------------

Area contraction reduces the graph of an area to a contracted graph. This contracted
graph is the set of shortest paths between a pairs of border vertices. The shortest
paths are calculated using many to many dijkstra algorithm. Area contraction helps in
reducing the size of large graphs.


Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:
  - Process is done only on edges with positive costs.

  - The process involves calculation of shortest paths using many to many Dijkstra

  - Values returned include edges of the shortest paths.

  - The returned edges are categorized based on the number of overlapping edges.

  - For optimization purposes, any duplicated value in the border vertices are ignored.


Signature Summary
-----------------

.. code-block:: none

    pgr_areaContraction(edges_sql, border_vertices)
    pgr_areaContraction(edges_sql, border_vertices, directed)

    RETURNS SETOF (seq, type, id, contracted_vertices, source, target, cost)


Signatures
-------------------------------------------------------------------------------

.. index::
    single: areaContraction(Minimal Use)

Minimal signature
.......................................

.. code-block:: none

    pgr_areaContraction(edges_sql, border_vertices)
    RETURNS SETOF (seq, type, id, contracted_vertices, source, target, cost) or EMPTY SET

The minimal signature is for a **directed** graph with an array of border vertices

:Example:

.. literalinclude:: doc-pgr_areaContraction.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: areaContraction(Complete signature)

Complete Signature
.......................................

.. code-block:: none

    pgr_areaContraction(edges_sql, border_vertices, directed)
    RETURNS SETOF (seq, type, id, contracted_vertices, source, target, cost) or EMPTY SET

This signature finds the set of shortest path from every element to every other element of border vertices array:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

:Example:

.. literalinclude:: doc-pgr_areaContraction.queries
   :start-after: -- q2
   :end-before: -- q3



Description of the Signatures
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Description of the parameters of the signatures
-------------------------------------------------------------------------------

Receives  ``(edges_sql, border_vertices, directed)``

======================= ====================== =================================================
Column                  Type                   Description
======================= ====================== =================================================
**edges_sql**           ``TEXT``               SQL query as described above.
**border_vertices**     ``ARRAY[ANY-INTEGER]`` Identifiers of the border vertices of an area.
**directed**            ``BOOLEAN``            * When ``true`` the graph is considered as `Directed`.
                                               * When ``false`` the graph is considered as `Undirected`.
======================= ====================== =================================================


Description of the return values
-------------------------------------------------------------------------------

RETURNS SETOF (seq, type, id, contracted_vertices, source, target, cost)

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

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end


See Also
-------------------------------------------------------------------------------

* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
