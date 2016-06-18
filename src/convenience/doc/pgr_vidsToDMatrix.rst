.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_vids_to_dmatrix:

pgr_vidsToDMatrix - Deprecated Function
==============================================================================


Name
------------------------------------------------------------------------------

``pgr_vidsToDMatrix`` - Creates a distances matrix from an array of ``vertex_id``.

.. warning::  This is a deprecated function

     - Is not officially in the release.
     - Name could change.
     - Signature could change.
     - Needs testing.
     - Functionality could change.


Synopsis
------------------------------------------------------------------------------

This function takes an array of ``vertex_id``, the original array of points used to generate the array of ``vertex_id``, an edge table name and a tol. It then computes kdijkstra() distances for each vertex to all the other vertices and creates a symmetric distance matrix suitable for TSP. The pnt array and the tol are used to establish a BBOX for limiting selection of edges. The extents of the points is expanded by tol.

The function returns:

  - ``record`` - with two fields as describe here
        * :dmatrix: ``float8[]`` - the distance matrix suitable to pass to pgrTSP() function.
        * :ids: ``integer[]`` - an array of ids for the distance matrix.
                  

.. code-block:: sql

        record pgr_vidsToDMatrix(IN vids integer[], IN pnts geometry[], IN edges text, tol float8 DEFAULT(0.1), OUT dmatrix double precision[], OUT ids integer[])


Description
-----------------------------------------------------------------------------

.. rubric:: Paramters

:vids: ``integer[]`` - An array of ``vertex_id``.
:pnts: ``geometry[]`` - An array of point geometries that approximates the extents of the ``vertex_id``.
:edges: ``text`` - The edge table to be used for the conversion.
:tol: ``float8`` - The amount to expand the BBOX extents of ``pnts`` when building the graph.

.. warning::

    * we compute a symmetric matrix because TSP requires that so the distances are better the Euclidean but but are not perfect
    * kdijkstra() can fail to find a path between some of the vertex ids. We to not detect this other than the cost might get set to -1.0, so the dmatrix shoule be checked for this as it makes it invalid for TSP

.. rubric:: History

* Proposed in version 2.1.0


Examples
-----------------------------------------------------------------------------

This example uses existing data of points.

.. literalinclude:: doc-matrix.queries
   :start-after: --q2
   :end-before: --q2.1

This example uses points that are not part of the graph.
    - :ref:`pgr_text_to_points` - is used to convert the locations into point geometries.
    - :ref:`pgr_points_to_vids` - to convert the array of point geometries into vertex ids.


.. literalinclude:: doc-matrix.queries
   :start-after: --q2.1
   :end-before: --q2.2


This example shows how this can be used in the context of feeding the results into pgr_tsp() function.

.. literalinclude:: doc-matrix.queries
   :start-after: --q2.2
   :end-before: --q3

This example uses the :ref:`sampledata` network.


See Also
-----------------------------------------------------------------------------

* :ref:`pgr_text_to_points` - Create an array of points from a text string.
* :ref:`pgr_tsp<pgr_tsp>` - Traveling Sales Person

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
