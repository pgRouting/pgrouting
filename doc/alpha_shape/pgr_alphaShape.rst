..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_alphaShape.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_alphaShape.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_alphaShape.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_alphaShape.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_alphaShape.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_alphaShape.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_alphaShape.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_alphaShape.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_alphaShape.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_alphaShape.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/alpha_shape/doc/pgr_alphaShape.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/alpha_shape/doc/pgr_alphaShape.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/driving_distance/doc/dd_alphashape.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/driving_distance/doc/dd_alphashape.html>`__

``pgr_alphaShape``
===============================================================================

``pgr_alphaShape`` — Polygon part of an alpha shape.

.. rubric:: Availability

* Version 3.0.0

  * Breaking change on signature
  * Old signature no longer supported
  * **Boost 1.54** & **Boost 1.55** are supported
  * **Boost 1.56+** is preferable

    * Boost Geometry is stable on Boost 1.56

* Version 2.1.0

  * Added alpha argument with default 0 (use optimal value)
  * Support to return multiple outer/inner ring

* Version 2.0.0

  * **Official** function
  * Renamed from version 1.x

.. rubric:: Support

Description
-------------------------------------------------------------------------------

Returns the polygon part of an alpha shape.

Characteristics

* Input is a *geometry* and returns a *geometry*
* Uses PostGis ST_DelaunyTriangles
* Instead of using CGAL's definition of `alpha` it use the ``spoon_radius``

  * :math:`spoon\_radius = \sqrt alpha`

* A Triangle area is considered part of the alpha shape when
  :math:`circumcenter\ radius < spoon\_radius`
* The ``alpha`` parameter is the **spoon radius**
* When the total number of points is less than 3, returns an EMPTY geometry


Signatures
-------------------------------------------------------------------------------
.. rubric:: Summary

.. index::
    single: alphaShape

.. admonition:: \ \
   :class: signatures

   | pgr_alphaShape(**geometry**, [alpha])

   | RETURNS ``geometry``


:Example: passing a geometry collection with spoon radius :math:`1.5` using the
          return variable ``geom``

.. literalinclude:: doc-pgr_alphashape.queries
   :start-after: -- q1
   :end-before: -- q2


Parameters
-------------------------------------------------------------------------------

============= ============= ======== ========================================
Parameter     Type          Default  Description
============= ============= ======== ========================================
**geometry**  ``geometry``           Geometry with at least :math:`3` points
``alpha``     ``FLOAT``        0     The radius of the spoon.
============= ============= ======== ========================================

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
