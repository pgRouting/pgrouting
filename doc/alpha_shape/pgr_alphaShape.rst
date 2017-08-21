..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_alphashape:

pgr_alphaShape
===============================================================================

.. index::
	single: pgr_alphashape(text,float8)

Name
-------------------------------------------------------------------------------

``pgr_alphaShape`` — Core function for alpha shape computation.


Synopsis
-------------------------------------------------------------------------------

Returns a table with (x, y) rows that describe the vertices of an alpha shape.

.. code-block:: sql

	table() pgr_alphaShape(text sql [, float8 alpha]);


Description
-------------------------------------------------------------------------------

:sql: ``text`` a SQL query, which should return a set of rows with the following columns:

    .. code-block:: sql

        SELECT id, x, y FROM vertex_table

    :id: ``int4`` identifier of the vertex
    :x: ``float8`` x-coordinate
    :y: ``float8`` y-coordinate

:alpha: (optional) ``float8`` alpha value. If specified alpha value equals 0 (default), then optimal alpha value is used.
    For more information, see `CGAL - 2D Alpha Shapes <http://doc.cgal.org/latest/Alpha_shapes_2/group__PkgAlphaShape2.html>`_.

Returns a vertex record for each row:

:x: x-coordinate
:y: y-coordinate

If a result includes multiple outer/inner rings, return those with separator row (x=NULL and y=NULL).

.. rubric:: History

* Renamed in version 2.0.0
* Added alpha argument with default 0 (use optimal value) in version 2.1.0
* Supported to return multiple outer/inner ring coordinates with separator row (x=NULL and y=NULL) in version 2.1.0

Examples
-------------------------------------------------------------------------------
PgRouting's alpha shape implementation has no way to control the order of the output points, so the actual output might different for the same input data.
The first query, has the output ordered, he second query shows an example usage:


.. rubric:: Example: the (ordered) results

.. literalinclude:: doc-pgr_alphashape.queries
   :start-after: -- q1
   :end-before: -- q2

.. rubric:: Example: calculating the area

Steps:

- Calculates the alpha shape
    - the :code:`ORDER BY` clause is not used.
- constructs a polygon
- and computes the area

.. literalinclude:: doc-pgr_alphashape.queries
   :start-after: -- q2
   :end-before: -- q3



The queries use the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :ref:`pgr_drivingDistance` - Driving Distance
* :ref:`pgr_points_as_polygon` - Polygon around set of points

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
