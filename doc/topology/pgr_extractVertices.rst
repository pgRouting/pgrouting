..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_extractVertices.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_extractVertices.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_extractVertices.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_extractVertices.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_extractVertices.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_extractVertices.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_extractVertices.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_extractVertices.html>`__

pgr_extractVertices -- Proposed
===============================================================================

``pgr_extractVertices`` — Extracts the vertices information


.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. rubric:: Availability

* Version 3.3.0

  * Classified as **proposed** function

* Version 3.0.0

  * New **experimental** function


Description
-------------------------------------------------------------------------------

This is an auxiliary function for extracting the vertex information of the set
of edges of a graph.

* When the edge identifier is given, then it will also calculate the in and out
  edges


Signatures
-------------------------------------------------------------------------------

.. index::
    single: pgr_extractVertices - Proposed on v3.3

.. admonition:: \ \
   :class: signatures

   | pgr_extractVertices(`Edges SQL`_, [``dryrun``])

   | RETURNS SETOF |result-extract|
   | OR EMTPY SET

:Example: Extracting the vertex information

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q1
   :end-before: --q1.1


Parameters
-------------------------------------------------------------------------------

============== ========  ================================
Parameter      Type      Description
============== ========  ================================
`Edges SQL`_   ``TEXT``  `Edges SQL`_ as described below
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

When line geometry is known
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

================= =================== ===================================
Column            Type                Description
================= =================== ===================================
``id``            ``BIGINT``          (Optional) identifier of the edge.
``geom``          ``LINESTRING``      Geometry of the edge.
================= =================== ===================================

This inner query takes precedence over the next two inner query, therefore other
columns are ignored when ``geom`` column appears.

* Ignored columns:

  * ``startpoint``
  * ``endpoint``
  * ``source``
  * ``target``

When vertex geometry is known
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

To use this inner query the column ``geom`` should not be part of the set of
columns.

================= =================== =======================================
Column            Type                Description
================= =================== =======================================
``id``            ``BIGINT``          (Optional) identifier of the edge.
``startpoint``    ``POINT``           POINT geometry of the starting vertex.
``endpoint``      ``POINT``           POINT geometry of the ending vertex.
================= =================== =======================================

This inner query takes precedence over the next inner query, therefore other
columns are ignored when ``startpoint`` and ``endpoint`` columns appears.

* Ignored columns:

  * ``source``
  * ``target``

When identifiers of vertices are known
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

To use this inner query the columns ``geom``, ``startpoint`` and ``endpoint``
should not be part of the set of columns.

============= =============== ==========================================
Column        Type            Description
============= =============== ==========================================
``id``        ``BIGINT``      (Optional) identifier of the edge.
``source``    ``ANY-INTEGER`` Identifier of the first end point vertex
                              of the edge.
``target``    ``ANY-INTEGER`` Identifier of the second end point vertex
                              of the edge.
============= =============== ==========================================


Result Columns
-------------------------------------------------------------------------------

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``id``
     - ``BIGINT``
     - Vertex identifier
   * - ``in_edges``
     - ``BIGINT[]``
     - Array of identifiers of the edges that have the vertex ``id`` as *first
       end point*.

       * ``NULL`` When the ``id`` is not part of the inner query
   * - ``out_edges``
     - ``BIGINT[]``
     - Array of identifiers of the edges that have the vertex ``id`` as *second
       end point*.

       * ``NULL`` When the ``id`` is not part of the inner query
   * - ``x``
     - ``FLOAT``
     - X value of the point geometry

       * ``NULL`` When no geometry is provided
   * - ``y``
     - ``FLOAT``
     - X value of the point geometry

       * ``NULL`` When no geometry is provided
   * - ``geom``
     - ``POINT``
     - Geometry of the point

       * ``NULL`` When no geometry is provided

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Dryrun execution
...............................................................................

To get the query generated used to get the vertex information, use ``dryrun :=
true``.

The results can be used as base code to make a refinement based on the backend
development needs.

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q2
   :end-before: --q2.1


Create a routing topology
...............................................................................

.. create_routing_topology_start

Make sure the database does not have the ``vertices_table``
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q3
   :end-before: --q3.1

Clean up the columns of the routing topology to be created
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q3.1
   :end-before: --q3.2

Create the vertices table
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* When the ``LINESTRING`` has a SRID then use ``geom::geometry(POINT, <SRID>)``
* For big edge tables that are been prepared,

  * Create it as ``UNLOGGED`` and
  * After the table is created ``ALTER TABLE .. SET LOGGED``

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q3.2
   :end-before: --q3.3

Inspect the vertices table
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q3.3
   :end-before: --q3.4

Create the routing topology on the edge table
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Updating the ``source`` information

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q3.4
   :end-before: --q3.5

Updating the ``target`` information

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q3.5
   :end-before: --q3.6

Inspect the routing topology
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q3.6
   :end-before: --q3.7

.. figure:: /images/Fig1-originalData.png
   :scale: 50%

   **Generated topology**

.. create_routing_topology_end


Crossing edges
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: cross_edges_start
   :end-before: cross_edges_end

Graphs without geometries
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: no_geometry_start
   :end-before: no_geometry_end

See Also
-------------------------------------------------------------------------------

* :doc:`topology-functions`
* :doc:`pgr_createVerticesTable`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
