.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_points_to_dmatrix:

pgr_pointsToDMatrix -- deprecated
==============================================================================

.. warning:: This function is deprecated!!!

    * This function is no longer supported, and may be removed from future versions.
    * No replacement exists.
    * See :ref:`tsp`.


Name
------------------------------------------------------------------------------

``pgr_pointsToDMatrix`` - Creates a distance matrix from an array of points.


Synopsis
------------------------------------------------------------------------------

Create a distance symetric distance matrix suitable for TSP using Euclidean distances based on the st_distance(). You might want to create a variant of this the uses st_distance_sphere() or st_distance_spheriod() or some other function.

The function returns:

  - ``record`` - with two fields as describe here
        * :dmatrix: ``float8[]`` - the distance matrix suitable to pass to pgrTSP() function.
        * :ids: ``integer[]`` - an array of ids for the distance matrix.

.. code-block:: sql

        record pgr_pointsToDMatrix(pnts geometry[], OUT dmatrix double precision[], OUT ids integer[])


Description
-----------------------------------------------------------------------------

.. rubric:: Paramters

:pnts: ``geometry[]`` - An array of point geometries.

.. warning::

    The generated matrix will be symmetric as required for pgr_TSP.

.. rubric:: History

* Proposed in version 2.1.0


Examples
-----------------------------------------------------------------------------

.. literalinclude:: doc-matrix.queries
   :start-after: --q1
   :end-before: --q1.1

This example shows how this can be used in the context of feeding the results into pgr_tsp() function.

.. literalinclude:: doc-matrix.queries
   :start-after: --q1.1
   :end-before: --q2


See Also
-----------------------------------------------------------------------------

* :ref:`pgr_vids_to_dmatrix` - convert a point geometry to the closest vertex_id of an edge..
* :ref:`pgr_tsp<pgr_tsp>` - Traveling Sales Person


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

