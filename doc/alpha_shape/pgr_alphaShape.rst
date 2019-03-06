..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_alphaShape
===============================================================================

``pgr_alphaShape`` — Polygon part of an alpha shape.

.. rubric:: Availability

* Version 3.0.0

  * Breaking change on signature

* Version 2.1.0

  * Added alpha argument with default 0 (use optimal value)
  * Support to return multiple outer/inner ring

* Renamed in v2.0.0

Description
-------------------------------------------------------------------------------

Returns the polygon part of an alpha shape.

Characteristics

* Input is a *geometry* and returns a *geometry*
* Uses PostGis ST_DelaunyTriangles
* Instead of using CGAL's definition of `alpha` it use the ``spoon_radius``

  * :math:`spoon\_radius = \sqrt alpha`

* A Triangle area is considered part of the alpha shape when :math:`circumcenter\ radius < spoon\_radius`
* When the total number of points is less than 3, returns an EMPTY geometry


Signatures
-------------------------------------------------------------------------------
.. rubric:: Summary

.. index::
    single: alphaShape(geometry)

.. code-block:: none

   pgr_alphaShape(geometry,   [spoon_radius])
   RETURNS geometry


.. rubric:: Example: passing a geometry collection with spoon radius :math:`1.5` using the return variable ``geom``

.. literalinclude:: doc-pgr_alphashape.queries
   :start-after: -- q1
   :end-before: -- q2


Parameters
-------------------------------------------------------------------------------

================= ================== ======== =================================================
Parameter         Type               Default     Description
================= ================== ======== =================================================
**geometry**      ``geometry``                Geometry with at least :math:`3` points
**spoon_radius**  ``FLOAT``                   The radius of the spoon
================= ================== ======== =================================================

Return Value
-------------------------------------------------------------------------------

==================== ========================
Kind of geometry     Description
==================== ========================
GEOMETRY COLLECTION  A Geometry collection of Polygons
==================== ========================





See Also
-------------------------------------------------------------------------------

* :doc:`pgr_drivingDistance`
* :doc:`sampledata` network.
* `ST_ConcaveHull <https://postgis.net/docs/ST_ConcaveHull.html>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
