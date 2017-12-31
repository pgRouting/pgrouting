..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_points_as_polygon:

pgr_pointsAsPolygon
===============================================================================

.. index::
    single: pgr_pointsAsPolygon(text,float8)

Name
-------------------------------------------------------------------------------

``pgr_pointsAsPolygon`` — Draws an alpha shape around given set of points.


Synopsis
-------------------------------------------------------------------------------

Returns the alpha shape as (multi)polygon geometry.

.. code-block:: sql

    geometry pgr_pointsAsPolygon(text sql [, float8 alpha]);


Description
-------------------------------------------------------------------------------

:sql: ``text`` a SQL query, which should return a set of rows with the following columns:

    .. code-block:: sql

        SELECT id, x, y FROM vertex_result;

    :id: ``int4`` identifier of the vertex
    :x: ``float8`` x-coordinate
    :y: ``float8`` y-coordinate

:alpha: (optional) ``float8`` alpha value. If specified alpha value equals 0 (default), then optimal alpha value is used.
    For more information, see `CGAL - 2D Alpha Shapes <http://doc.cgal.org/latest/Alpha_shapes_2/group__PkgAlphaShape2.html>`_.

Returns a (multi)polygon geometry (with holes).


.. rubric:: History

* Renamed in version 2.0.0
* Added alpha argument with default 0 (use optimal value) in version 2.1.0
* Supported to return a (multi)polygon geometry (with holes) in version 2.1.0


Examples
-------------------------------------------------------------------------------
In the following query there is no way to control which point in the polygon is the first in the list, so you may get similar but different results than the following which are also correct.

.. literalinclude:: doc-pgr_pointsAsPolygon.queries
   :start-after: --q1
   :end-before: --q2


The query use the :doc:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_drivingDistance` - Driving Distance
* :ref:`pgr_alphashape` - Alpha shape computation

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
