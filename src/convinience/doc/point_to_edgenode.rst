.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_point_to_edgenode:

pgr_pointToEdgeNode
==============================================================================

.. index::
        single: pgr_pointToEdgeNode(edges text, pnt geometry, tol float8) --proposed


Name
------------------------------------------------------------------------------

``pgr_pointToEdgeNode`` - Converts a point to a ``vertex_id`` based on closest edge.

.. warning::  This is a proposed function

     - Is not officially in the release.
     - Name could change.
     - Signature could change.
     - Needs testing.
     - Functionality could change.


Synopsis
------------------------------------------------------------------------------

The function returns:

  - ``integer`` that is the vertex id of the closest edge in the ``edges`` table within the ``tol`` tolerance of ``pnt``. The vertex is selected by projection the ``pnt`` onto the edge and selecting which vertex is closer along the edge.

.. code-block:: sql

        integer pgr_pointToEdgeNode(edges text, pnt geometry, tol float8)


Description
-----------------------------------------------------------------------------

Given an table ``edges`` with a spatial index on ``the_geom`` and a point geometry search for the closest edge within ``tol`` distance to the edges then compute the projection of the point onto the line segment and select source or target based on whether the projected point is closer to the respective end and return the source or target value.

.. rubric:: Parameters

The function accepts the following parameters:

:edges: ``text`` The name of the edge table or view. (may contain the schema name AS well).
:pnt: ``geometry`` A point geometry object in the same SRID as ``edges``.
:tol: ``float8`` The maximum search distance for an edge.

.. warning::

    If no edge is within tol distance then return -1

The ``edges`` table must have the following columns:

    * ``source``
    * ``target``
    * ``the_geom``

.. rubric:: History

* New in version 2.1.0


Examples
-----------------------------------------------------------------------------

.. code-block:: sql

        select pgr_pointtoedgenode('edge_table', 'POINT(2 0)'::geometry, 0.02);
        pgr_pointtoedgenode
        -------------------
                         1
        (1 row)

The example uses the :ref:`sampledata` network.

See Also
-----------------------------------------------------------------------------

* :ref:`pgr_points_to_vids` - convert an array of point geometries into vertex ids.

