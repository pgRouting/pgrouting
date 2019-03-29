..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_extractVertices -- Proposed
===============================================================================

``pgr_extractVertices`` — Extracts the vertices information based on the source and target.

.. rubric:: Availability

* New on v3.0.0

Description
-------------------------------------------------------------------------------

Extracts the vertex information of the edges of a graph

Signatures
-------------------------------------------------------------------------------

.. code-block:: sql

   pgr_extractVertices(Edges SQL)
   RETURNS SETOF (id, x, y, the_geom)

:Example: Extracting the vertex information

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q1
   :end-before: --q1.1


Parameters
-------------------------------------------------------------------------------

============== ==================  =================================================
Parameter      Type                Description
============== ==================  =================================================
**Edges SQL**  ``TEXT``            Inner SQL query as described below.
============== ==================  =================================================

Inner Query
-------------------------------------------------------------------------------

.. rubric:: Edges SQL

================= =================== =================================================
Column            Type                Description
================= =================== =================================================
**source**        ``ANY-INTEGER``     Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``     Identifier of the second end point vertex of the edge.
**the_geom**      ``LINESTRING``      (Optional) Geometry of the edge
================= =================== =================================================


Return Columns
-------------------------------------------------------------------------------

================= =============== =================================================
Column            Type                Description
================= =============== =================================================
**id**            ``BIGINT``      Identifier of the first end point vertex of the edge.
**x**             ``FLOAT``       X value of the POINT geometry
                                  - `NULL` When no LINESTRING geometry is provided
**y**             ``FLOAT``       Y value of the POINT geometry
                                  - `NULL` When no LINESTRING geometry is provided
**the_geom**      ``POINT``       Geometry of the POINT
                                  - `NULL` When no LINESTRING geometry is provided
================= =============== =================================================


Additional Examples
-------------------------------------------------------------------------------

:Example 1: Extracting the vertex information without geometry for the first :math:`4` edges

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q2
   :end-before: --q2.1

:Example 2: Creating a vertices table

.. literalinclude:: doc-pgr_extractVertices.queries
   :start-after: --q2
   :end-before: --q2.1


See Also
-------------------------------------------------------------------------------

* :doc:`topology-functions`  for an overview of a topology for routing algorithms.
* :doc:`pgr_createVerticesTable` to create a topology based on the geometry.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
