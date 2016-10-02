..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_points_to_dmatrix:

pgr_pointsToDMatrix
==============================================================================

.. index::
        single: pgr_pointsToDMatrix(pnts geometry[], OUT dmatrix double precision[], OUT ids integer[])
        module: common


Name
------------------------------------------------------------------------------

``pgr_pointsToDMatrix`` - Creates a distance matrix from an array of points.


Synopsis
------------------------------------------------------------------------------

Create a distance symmetric distance matrix suitable for TSP using Euclidean distances based on the st_distance(). You might want to create a variant of this the uses st_distance_sphere() or st_distance_spheriod() or some other function.

The function returns:

  - ``record`` - with two fields as describe here
        * :dmatrix: ``float8[]`` - the distance matrix suitable to pass to pgrTSP() function.
        * :ids: ``integer[]`` - an array of ids for the distance matrix.

.. code-block:: sql

        record pgr_pointsToDMatrix(pnts geometry[], OUT dmatrix double precision[], OUT ids integer[])


Description
-----------------------------------------------------------------------------

.. rubric:: Parameters

:pnts: ``geometry[]`` - An array of point geometries.

.. warning::

    The generated matrix will be symmetric as required for pgr_TSP.


.. rubric:: History

* New in version 2.1.0


Examples
-----------------------------------------------------------------------------

.. code-block:: sql

        select unnest(dmatrix) from pgr_pointsToDMatrix(
            pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0)
        ) limit 8;
              unnest
        ------------------
                        0
                        1
          1.4142135623731
                        2
         2.23606797749979
         2.82842712474619
                        3
         2.23606797749979
        (8 rows)

        select ids from pgr_pointstodmatrix(
            pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0)
        );
                ids
        -------------------
         {1,2,3,4,5,6,7,8}
        (1 row)



See Also
-----------------------------------------------------------------------------

* :ref:`pgr_vids_to_dmatrix` - convert a point geometry to the closest vertex_id of an edge..
* :ref:`pgr_tsp<pgr_tsp>` - Traveling Sales Person

