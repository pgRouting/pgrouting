..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_extractVertices -- Experimental
===============================================================================

``pgr_extractVertices`` — Extracts the vertices information based on the source and target.


.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_extractVertices.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_extractVertices.html>`__


Description
-------------------------------------------------------------------------------

This is an auxiliary function for extracting the vertex information of the set of
edges of a graph.

* When the edge identifier is given, then it will also calculate the in and out edges


Signatures
-------------------------------------------------------------------------------

.. index::
    single: pgr_extractVertices -- Experimental

.. code-block:: sql

   pgr_extractVertices(Edges SQL [, dryrun])
   RETURNS SETOF (id, in_edges, out_edges, x, y, geom)

:Example: Extracting the vertex information

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q1
   :end-before: --q1.1


Parameters
-------------------------------------------------------------------------------

============== ==================  =================================================
Parameter      Type                Description
============== ==================  =================================================
**Edges SQL**  ``TEXT``            The set of edges of the graph. It is an `Inner Query`_ as described below.
**dryrun**     ``TEXT``            Don't process and get in a NOTICE the resulting query.
============== ==================  =================================================

Inner Query
-------------------------------------------------------------------------------

.. rubric:: When line geometry is known

================= =================== =================================================
Column            Type                Description
================= =================== =================================================
**id**            ``BIGINT``          (Optional) identifier of the edge.
**geom**          ``LINESTRING``      LINESTRING geometry of the edge.
================= =================== =================================================

This inner query takes precedence over the next two inner query, therefore other columns are ignored when ``geom`` column appears.

* Ignored columns:

  * ``startpoint``
  * ``endpoint``
  * ``source``
  * ``target``



.. rubric:: When vertex geometry is known

To use this inner query the column ``geom`` should not be part of the set of columns.

================= =================== =================================================
Column            Type                Description
================= =================== =================================================
**id**            ``BIGINT``          (Optional) identifier of the edge.
**startpoint**    ``POINT``           POINT geometry of the starting vertex.
**endpoint**      ``POINT``           POINT geometry of the ending vertex.
================= =================== =================================================

This inner query takes precedence over the next inner query,
therefore other columns are ignored when ``startpoint`` and ``endpoint``  columns appears.

* Ignored columns:

  * ``source``
  * ``target``

.. rubric:: When identifiers of vertices are known

To use this inner query the columns ``geom``, ``startpoint`` and ``endpoint`` should not be part of the set of columns.

================= =================== =================================================
Column            Type                Description
================= =================== =================================================
**id**            ``BIGINT``          (Optional) identifier of the edge.
**source**        ``ANY-INTEGER``     Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``     Identifier of the second end point vertex of the edge.
================= =================== =================================================


Result Columns
-------------------------------------------------------------------------------

Rreturns set of (id, in_edges, out_edges, x, y, geom)

================= =============== =================================================
Column            Type                Description
================= =============== =================================================
**id**            ``BIGINT``      Identifier of the first end point vertex of the edge.
**in_edges**      ``BIGINT[]``    Array of identifiers of the edges that have the vertex ``id`` as *first end point*.
                                   * ``NULL`` When the ``id`` is not part of the inner query
**out_edges**     ``BIGINT[]``    Array of identifiers of the edges that have the vertex ``id`` as *second end point*.
                                   * ``NULL`` When the ``id`` is not part of the inner query
**x**             ``FLOAT``       X value of the POINT geometry
                                   * ``NULL`` When no geometry is provided
**y**             ``FLOAT``       Y value of the POINT geometry
                                   * ``NULL`` When no geometry is provided
**geom**          ``POINT``       Geometry of the POINT
                                   * ``NULL`` When no geometry is provided
================= =============== =================================================


Additional Examples
-------------------------------------------------------------------------------

:Example 1: Dryrun execution

To get the query generated used to get the vertex information, use ``dryrun := true``.

The results can be used as base code to make a refinement based on the backend development needs.

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q2
   :end-before: --q2.1

:Example 2: Creating a routing topology

#. Making sure the database does not have the ``vertices_table``

   .. literalinclude:: doc-pgr_extractVertices.queries
      :start-after: --q3
      :end-before: --q3.1

#. Cleaning up the columns of the rotuing topology to be created

   .. literalinclude:: doc-pgr_extractVertices.queries
      :start-after: --q3.1
      :end-before: --q3.2

#. Creating the vertices table

   .. literalinclude:: doc-pgr_extractVertices.queries
      :start-after: --q3.2
      :end-before: --q3.3

#. Inspection of the vertices table

   .. literalinclude:: doc-pgr_extractVertices.queries
      :start-after: --q3.3
      :end-before: --q3.4

#. Creating the routing topology on the edge table

   Updating the ``source`` information

   .. literalinclude:: doc-pgr_extractVertices.queries
      :start-after: --q3.4
      :end-before: --q3.5

   Updating the ``target`` information

   .. literalinclude:: doc-pgr_extractVertices.queries
      :start-after: --q3.5
      :end-before: --q3.6

#. Inspection of the routing topology

   .. literalinclude:: doc-pgr_extractVertices.queries
      :start-after: --q3.6
      :end-before: --q3.7

See Also
-------------------------------------------------------------------------------

* :doc:`topology-functions`  for an overview of a topology for routing algorithms.
* :doc:`pgr_createVerticesTable` to create a topology based on the geometry.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
