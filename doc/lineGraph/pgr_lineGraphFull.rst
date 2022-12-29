..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_lineGraphFull.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_lineGraphFull.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_lineGraphFull.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_lineGraphFull.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_lineGraphFull.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_lineGraphFull.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_lineGraphFull.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_lineGraphFull.html>`__

``pgr_lineGraphFull`` - Experimental
===============================================================================

``pgr_lineGraphFull`` — Transforms a given graph into a new graph where all of
the vertices from the original graph are converted to line graphs.

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 2.6.0

  * New **Experimental** function


Description
-------------------------------------------------------------------------------

``pgr_lineGraphFull``, converts original directed graph to a directed line graph
by converting each vertex to a complete graph and keeping all the original
edges.
The new connecting edges have a cost 0 and go between the adjacent original
edges, respecting the directionality.

A possible application of the resulting graph is **"routing with two edge
restrictions"**:

- Setting a cost of using the vertex when routing between edges on the
  connecting edge
- Forbid the routing between two edges by removing the connecting edge

This is possible because each of the intersections (vertices) in the original
graph are now complete graphs that have a new edge for each possible turn across
that intersection.

The main characteristics are:

- This function is for **directed** graphs.
- Results are undefined when a negative vertex id is used in the input graph.
- Results are undefined when a duplicated edge id is used in the input graph.
- Running time: TBD

.. index::
    single: lineGraphFull - Experimental on v2.6

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_lineGraphFull(`Edges SQL`_)

   | RETURNS SET OF |result-linegf|
   | OR EMPTY SET

:Example: Full line graph of subgraph of edges :math:`\{4, 7, 8, 10\}`

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: only_edge_param_start
   :end-before: only_edge_param_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

RETURNS SET OF |result-linegf|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INTEGER``
     - Sequential value starting from **1**.

       - Gives a local identifier for the edge
   * - ``source``
     - ``BIGINT``
     - Identifier of the source vertex of the current edge.

       * When `negative`: the source is the reverse edge in the original graph.
   * - ``target``
     - ``BIGINT``
     - Identifier of the target vertex of the current edge.

       * When `negative`: the target is the reverse edge in the original graph.
   * - ``cost``
     - ``FLOAT``
     - Weight of the edge (``source``, ``target``).

       * When `negative`: edge (``source``, ``target``) does not exist,
         therefore it’s not part of the graph.
   * - ``reverse_cost``
     - ``FLOAT``
     - Weight of the edge (``target``, ``source``).

       * When `negative`: edge (``target``, ``source``) does not exist,
         therefore it’s not part of the graph.

Additional Examples
-------------------------------------------------------------------------------


.. contents::
   :local:

The examples of this section are based on the :doc:`sampledata` network.
The examples include the subgraph including edges 4, 7, 8, and 10 with
``reverse_cost``.

The data
...............................................................................

This example displays how this graph transformation works to create additional
edges for each possible turn in a graph.

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q0
   :end-before: -- q1

| |first|

.. TODO fix image

.. |first| image:: images/original.png
   :align: middle

The transformation
...............................................................................

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q1
   :end-before: -- q2

.. TODO fix image

| |second|

.. |second| image:: images/transformation.png
   :align: middle

In the transformed graph, all of the edges from the original graph are still
present (yellow), but we now have additional edges for every turn that could be
made across vertex 7 (orange).

Creating table that identifies transformed vertices
...............................................................................

The vertices in the transformed graph are each created by splitting up the
vertices in the original graph.
Unless a vertex in the original graph is a leaf
vertex, it will generate more than one vertex in the transformed graph.
One of the newly created vertices in the transformed graph will be given the
same vertex identifier as the vertex that it was created from in the original
graph, but the rest of the newly created vertices will have negative vertex ids.

Following is an example of how to generate a table that maps the ids of the
newly created vertices with the original vertex that they were created from

Store edge results
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The first step is to store the results of the ``pgr_lineGraphFull`` call into a
table

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q2
   :end-before: -- q3

Create the mapping table
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

From the original graph's vertex information

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q3
   :end-before: -- q4

Add the new vertices

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q4
   :end-before: -- q5

Filling the mapping table
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The positive vertex identifiers are the original identifiers

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q5
   :end-before: -- q6

Inspecting the vertices map

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q6
   :end-before: -- q7

The self loops happen when there is no cost traveling to the ``target`` and the
source has an original value.

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q7
   :end-before: -- q8

Updating values from self loops

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q8
   :end-before: -- q9

Inspecting the vertices table

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q9
   :end-before: -- q10

Updating from inner self loops

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q10
   :end-before: -- q11

Inspecting the vertices map

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q11
   :end-before: -- q12

Adding a soft restriction
...............................................................................

A soft restriction going from vertex 6 to vertex 3 using edges 4 -> 7 is wanted.

Idenifying the restriction
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Running a :doc:`pgr_dijkstraNear` the edge with cost 0, edge 8, is where the
cost will be increased

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q12
   :end-before: -- q13

The edge to be altered is ``WHERE cost = 0 AND seq != 1 AND edge != -1`` from
the previus query:

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q13
   :end-before: -- q14

Adding a value to the restriction
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Updating the cost to the edge:

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q14
   :end-before: -- q15

:Example: Routing from :math:`6` to :math:`3`

Now the route does not use edge 8 and does a U turn on a leaf vertex.

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q15
   :end-before: -- q16

Simplifying leaf vertices
...............................................................................

In this example, there is no additional cost for traversing a leaf vertex.

Using the vertex map give the leaf verices their original value.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

On the source column

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q16
   :end-before: -- q17

On the target column

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q17
   :end-before: -- q18

Removing self loops on leaf nodes
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The self loops of the leaf nodes are

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q18
   :end-before: -- q19

Which can be removed

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q19
   :end-before: -- q20

:Example: Routing from :math:`6` to :math:`3`

Routing can be done now using the original vertices id using :doc:`pgr_dijkstra`

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q20
   :end-before: -- q21

Complete routing graph
...............................................................................

Add edges from the original graph
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Add all the edges that are not involved in the line graph process to the new
table

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q21
   :end-before: -- q22

Some administrative tasks to get new identifiers for the edges

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q22
   :end-before: -- q23

Add the newly calculated edges
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q23
   :end-before: -- q24

Using the routing graph
...............................................................................

When using this method for routing with soft restrictions there will be uturns

:Example: Routing from :math:`6` to :math:`3`

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q24
   :end-before: -- q25

:Example: Routing from :math:`5` to :math:`1`

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q25
   :end-before: -- q26

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Line_graph
* https://en.wikipedia.org/wiki/Complete_graph

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

