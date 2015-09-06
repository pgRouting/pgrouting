..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_points_to_vids:

pgr_pointsToVids
==============================================================================

.. index::
        single: pgr_pointsToVids(pnts geometry[], edges text, tol float8 DEFAULT(0.01))
        module: common


Name
------------------------------------------------------------------------------

``pgr_pointsToVids`` - Converts an array of point geometries into vertex ids.


Synopsis
------------------------------------------------------------------------------

Given an array of point geometries and an edge table and a max search tol distance the function converts points into vertex ids using pgr_pointtoedgenode().

The function returns:

  - ``integer[]`` - An array of ``vertex_id``.

.. code-block:: sql

        integer[] pgr_pointsToVids(pnts geometry[], edges text, tol float8 DEFAULT(0.01))


Description
-----------------------------------------------------------------------------

.. rubric:: Paramters

:pnts: ``geometry[]`` - An array of point geometries.
:edges: ``text`` - The edge table to be used for the conversion.
:tol: ``float8`` - The maximum search distance for locating the closest edge.

.. warning::

    You need to check the results for any vids=-1 which indicates if failed to locate an edge.


.. rubric:: History

* New in version 2.1.0


Examples
-----------------------------------------------------------------------------

.. code-block:: sql

        select * from pgr_pointstovids(
            pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0),
            'edge_table'
            -- tol is not specified, so we use the default of 0.01
        );
          pgr_pointstovids
        --------------------
         {1,2,3,5,4,9,10,6}
        (1 row)

This example uses the :ref:`sampledata` network.


See Also
-----------------------------------------------------------------------------

* :ref:`pgr_point_to_edgenode` - convert a point geometry to the closest vertex_id of an edge..

