..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Metrics Family ; pgr_degree
   single: degree

|

``pgr_degree``
===============================================================================

``pgr_degree`` — For each vertex in an undirected graph, return the count of
edges incident to the vertex.


.. include:: proposed.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 3.8.0

* Error messages adjustment.
* New signature with only Edges SQL.
* Function promoted to official.

.. rubric:: Version 3.4.0

* New proposed function.


Description
-------------------------------------------------------------------------------

Calculates the degree of the vertices of an undirected graph

The degree (or valency) of a vertex of a graph is the number of edges that are
incident to the vertex.

- Works for **undirected** graphs.
- A loop contributes 2 to a vertex's degree.
- A vertex with degree 0 is called an isolated vertex.

  - Isolated vertex is not part of the result

- Vertex not participating on the subgraph is considered and isolated vertex.
- There can be a ``dryrun`` execution and the code used to get the answer will
  be shown in a PostgreSQL ``NOTICE``.

  - The code can be used as base code for the particular application
    requirements.

- No ordering is performed.

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_degree(`Edges SQL`_ , [``dryrun``])
   | pgr_degree(`Edges SQL`_ , `Vertex SQL`_, [``dryrun``])

   | RETURNS SETOF |result-degree|
   | OR EMPTY SET

.. index::
    single: degree ; Edges and Vertices

Edges
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_degree(`Edges SQL`_ , [``dryrun``])

   | RETURNS SETOF |result-degree|
   | OR EMPTY SET

:example: Get the degree of the vertices defined on the edges table

.. literalinclude:: degree.queries
   :start-after: -- q1
   :end-before: -- q2

Edges and Vertices
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_degree(`Edges SQL`_ , `Vertex SQL`_, [``dryrun``])

   | RETURNS SETOF |result-degree|
   | OR EMPTY SET

:Example: Extracting the vertex information

``pgr_degree`` can use :doc:`pgr_extractVertices` embedded in the call.

For decent size networks, it is best to prepare your vertices table before hand
and use it on ``pgr_degree`` calls. (See `Using a vertex table`_)

Calculate the degree of the nodes:

.. literalinclude:: degree.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: degree ; Edges


Parameters
-------------------------------------------------------------------------------

============== ========  ================================
Parameter      Type      Description
============== ========  ================================
`Edges SQL`_   ``TEXT``  `Edges SQL`_ as described below
`Vertex SQL`_  ``TEXT``  `Vertex SQL`_ as described below
============== ========  ================================

Optional parameters
-------------------------------------------------------------------------------

=========== =============  ========== =======================================
Parameter    Type          Default    Description
=========== =============  ========== =======================================
``dryrun``  ``BOOLEAN``    ``false``  * When true do not process and get in a
                                        NOTICE the resulting query.
=========== =============  ========== =======================================

Inner Queries
-------------------------------------------------------------------------------

.. contents::
   :local:

Edges SQL
...............................................................................

.. rubric:: For the `Edges and Vertices`_ signature:

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``id``
     - ``BIGINT``
     - Identifier of the edge.

For the `Edges`_ signature:

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``id``
     - ``BIGINT``
     - Identifier of the edge.
   * - ``source``
     - ``BIGINT``
     - Identifier of the first end point vertex of the edge.
   * - ``target``
     - ``BIGINT``
     - Identifier of the second end point vertex of the edge.

Vertex SQL
...............................................................................

.. rubric:: For the `Edges and Vertices`_ signature:

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``id``
     - ``BIGINT``
     - Identifier of the first end point vertex of the edge.
   * - ``in_edges``
     - ``BIGINT[]``
     - Array of identifiers of the edges that have the vertex ``id`` as *first
       end point*.

       * When missing, ``out_edges`` must exist.
   * - ``out_edges``
     - ``BIGINT[]``
     - Array of identifiers of the edges that have the vertex ``id`` as *second
       end point*.

       * When missing, ``in_edges`` must exist.


Result columns
-------------------------------------------------------------------------------

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``node``
     - ``BIGINT``
     - Vertex identifier
   * - ``degree``
     - ``BIGINT``
     - Number of edges that are incident to the vertex ``id``

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Degree of a loop
...............................................................................

A loop contributes 2 to a vertex's degree.

.. graphviz::

   graph G {
     2 [shape=circle;style=filled;color=green;fontsize=8;width=0.3;fixedsize=true];
     2 -- 2 [label="1",fontsize=8];
   }

.. rubric:: Using the `Edges`_ signature.

.. literalinclude:: degree.queries
   :start-after: -- q3
   :end-before: -- q4

.. rubric:: Using the `Edges and Vertices`_ signature.

.. literalinclude:: degree.queries
   :start-after: -- q4
   :end-before: -- q5

Degree of a sub graph
...............................................................................

For the following is a subgraph of the :doc:`sampledata`:

- :math:`E = \{(1, 5 \leftrightarrow 6), (1, 6 \leftrightarrow 10)\}`
- :math:`V = \{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17\}`


.. graphviz::

   graph G {
     5,6,10 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     1,2,3,4,7,8,9,11,12,13,14,15,16,17 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     5 -- 6 [label="1",fontsize=8];
     10 -- 6 [label="2",fontsize=8];

     1 [pos="0,2!"];
     2 [pos="0.5,3.5!"];
     3 [pos="1,2!"];
     4 [pos="2,3.5!"];
     5 [pos="2,0!"];
     6 [pos="2,1!"];
     7 [pos="2,2!"];
     8 [pos="2,3!"];
     9 [pos="2,4!"];
     10 [pos="3,1!"];
     11 [pos="3,2!"];
     12 [pos="3,3!"];
     13 [pos="3.5,2.3!"];
     14 [pos="3.5,4!"];
     15 [pos="4,1!"];
     16 [pos="4,2!"];
     17 [pos="4,3!"];
   }

The vertices not participating on the edge are considered isolated

- their degree is 0 in the subgraph and
- their degree is not shown in the output.

.. rubric:: Using the `Edges`_ signature.

.. literalinclude:: degree.queries
   :start-after: -- q5
   :end-before: -- q6

.. rubric:: Using the `Edges and Vertices`_ signature.

.. literalinclude:: degree.queries
   :start-after: -- q6
   :end-before: -- q7

Using a vertex table
...............................................................................

For decent size networks, it is best to prepare your vertices table before hand
and use it on ``pgr_degree`` calls.

Extract the vertex information and save into a table:

.. literalinclude:: degree.queries
   :start-after: -- q8
   :end-before: -- q9

Calculate the degree of the nodes:

.. literalinclude:: degree.queries
   :start-after: -- q9
   :end-before: -- q10

Dry run execution
...............................................................................

To get the query generated used to get the vertex information, use ``dryrun =>
true``.

The results can be used as base code to make a refinement based on the backend
development needs.

.. literalinclude:: degree.queries
   :start-after: -- q10
   :end-before: -- q11

Finding dead ends
...............................................................................

.. finding_dead_ends_start

If there is a vertices table already built using ``pgr_extractVertices``
and want the degree of the whole graph rather than a subset, it can be forgo using
``pgr_degree`` and work with the ``in_edges`` and ``out_edges`` columns
directly.

The degree of a dead end is 1.

.. include:: pgRouting-concepts.rst
   :start-after: degree_from_table_start
   :end-before: degree_from_table_end

.. finding_dead_ends_end

Finding linear vertices
...............................................................................

The degree of a linear vertex is 2.

If there is a vertices table already built using the ``pgr_extractVertices``

.. include:: pgRouting-concepts.rst
   :start-after: linear_degree_from_table_start
   :end-before: linear_degree_from_table_end

See Also
-------------------------------------------------------------------------------

* :doc:`utilities-family`
* :doc:`pgr_extractVertices`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
